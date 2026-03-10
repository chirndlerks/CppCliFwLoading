#include <Windows.h>
#include "inc/nethost.h"
#include "inc/hostfxr.h"
#include <cwctype>
#include <vector>
#include <ranges>
#include <string>
#include <string_view>
#include <filesystem>

namespace InitDotnet {
    constexpr const char* INIT_FUNC_NAME = "hostfxr_initialize_for_runtime_config";
    constexpr const char* CLOSE_FUNC_NAME = "hostfxr_close";

    constexpr const char* GET_PROPERTY_FUNC_NAME = "hostfxr_get_runtime_property_value";
    constexpr const char* SET_PROPERTY_FUNC_NAME = "hostfxr_set_runtime_property_value";

    constexpr const char* GET_DELEGATE_FUNC_NAME = "hostfxr_get_runtime_delegate";

    constexpr const char_t* TPA_PROPERTY = L"TRUSTED_PLATFORM_ASSEMBLIES";
    HMODULE hostfxr{ nullptr };

    hostfxr_initialize_for_runtime_config_fn hostfxr_init{ nullptr };
    hostfxr_get_runtime_property_value_fn hostfxr_get_runtime_property_value{ nullptr };
    hostfxr_set_runtime_property_value_fn hostfxr_set_runtime_property_value{ nullptr };
    hostfxr_get_runtime_delegate_fn hostfxr_create_delegate{ nullptr };
    hostfxr_close_fn hostfxr_close{ nullptr };

    // load_assembly_and_get_function_pointer_fn hostfxr_load_assembly_and_get_function_pointer_func{ nullptr };
    hostfxr_handle host_context{ nullptr };


#define ASSERT_MSG_RETURN_FALSE(x,y) if (!x) return false;


    [[nodiscard]] bool load_hostfxr_functions(const HMODULE hostfxr_module) {
        hostfxr_init = hostfxr_initialize_for_runtime_config_fn(GetProcAddress(hostfxr_module, INIT_FUNC_NAME));
        hostfxr_close = hostfxr_close_fn(GetProcAddress(hostfxr_module, CLOSE_FUNC_NAME));

        hostfxr_get_runtime_property_value = hostfxr_get_runtime_property_value_fn(GetProcAddress(hostfxr, GET_PROPERTY_FUNC_NAME));
        hostfxr_set_runtime_property_value = hostfxr_set_runtime_property_value_fn(GetProcAddress(hostfxr_module, SET_PROPERTY_FUNC_NAME));

        hostfxr_create_delegate = hostfxr_get_runtime_delegate_fn(GetProcAddress(hostfxr_module, GET_DELEGATE_FUNC_NAME));

        if (hostfxr_init == nullptr) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR initialize function"));
        }

        if (hostfxr_close == nullptr) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR close function"));
        }

        if (hostfxr_get_runtime_property_value == nullptr) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR get property function"));
        }

        if (hostfxr_set_runtime_property_value == nullptr) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR set property function"));
        }

        if (hostfxr_create_delegate == nullptr) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR create delegate function"));
        }

        return true;
    }
    std::vector<std::wstring> splitWString(const std::wstring& str, wchar_t delimiter = L';')
    {
        std::vector<std::wstring> result;
        std::wstringstream wss(str);
        std::wstring token;

        while (std::getline(wss, token, delimiter))
        {
            result.push_back(token);
        }

        return result;
    }

    bool icontains(std::wstring_view haystack, std::wstring_view needle)
    {
        if (needle.empty()) return true;

        auto it = std::search(
            haystack.begin(), haystack.end(),
            needle.begin(), needle.end(),
            [](wchar_t ch1, wchar_t ch2) {
                // Compare characters as lowercase
                return std::towlower(ch1) == std::towlower(ch2);
            }
        );

        // If the iterator doesn't reach the end, the needle was found
        return it != haystack.end();
    }

    [[nodiscard]] bool fix_tpa_list() {
        const char_t* tpa_list;
        auto result = hostfxr_get_runtime_property_value(host_context, TPA_PROPERTY, &tpa_list);
        if (result != 0) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not get TPA list"));
        }
        std::wstring_view tpa_list_view(tpa_list);
        std::wstring new_tpa_list;
        new_tpa_list.reserve(tpa_list_view.size() + 1);
        auto parts = std::views::split(tpa_list_view, std::wstring(L";"));
        for (auto part : parts) {
            // You don't strictly need the explicit std::wstring_view assignment on the right
            std::wstring_view str_part(part.data(), part.size());

            // Using our new STL icontains function and fixing the L"..." string literal syntax
            if (!icontains(str_part, L"System.Runtime.Serialization.Formatters.dll")) {
                new_tpa_list += str_part;
                new_tpa_list += L";"; // Replaced MFC's _T(";") with pure STL wide literal L";"
            }
        }
        result = hostfxr_set_runtime_property_value(host_context,
            TPA_PROPERTY,
            new_tpa_list.data());
        if (result != 0) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not add managed assembly to TPA list"));
        }
        return true;
    }

    std::wstring GetModulePath()
    {
        // 1. Use a vector to handle potentially long paths (avoiding MAX_PATH limits)
        std::vector<wchar_t> buffer(MAX_PATH);
        DWORD length = ::GetModuleFileNameW(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));

        // 2. If the path was longer than MAX_PATH, resize and try again
        while (length == buffer.size() && GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
            buffer.resize(buffer.size() * 2);
            length = ::GetModuleFileNameW(NULL, buffer.data(), static_cast<DWORD>(buffer.size()));
        }

        if (length == 0) {
            return L""; // Handle error appropriately
        }

        // 3. Create a filesystem path object from the buffer
        std::filesystem::path modulePath(buffer.begin(), buffer.begin() + length);

        // 4. Use STL filesystem to easily extract the directory
        if (modulePath.has_parent_path()) {
            return modulePath.parent_path().wstring();
        }

        return modulePath.wstring();
    }

    [[nodiscard]] bool InitDotNet(void) {

        char_t buffer[MAX_PATH];
        auto buffer_size = static_cast<size_t>(MAX_PATH);
        auto result = get_hostfxr_path(buffer, &buffer_size, nullptr);
        if (result != 0) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not get HostFXR path"));
        }

        hostfxr = LoadLibraryExW(buffer, nullptr, 0);
        if (hostfxr == nullptr) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR library"));
        }

        if (!load_hostfxr_functions(hostfxr)) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load HostFXR functions"));
        }


        const std::wstring runtime_config_path = GetModulePath() + L"\\cppclilib.runtimeconfig.json";

        result = hostfxr_init(runtime_config_path.c_str(), nullptr, &host_context);
        if (result != 0) {
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not initialize the HostFXR context"));
        }

        if (!fix_tpa_list()) {
            hostfxr_close(host_context);
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not fix TPA list"));
        }


        void* func_ptr{ nullptr };
        result = hostfxr_create_delegate(host_context, hdt_load_assembly_and_get_function_pointer, &func_ptr);
        if (result != 0 || func_ptr == nullptr) {
            hostfxr_close(host_context);
            ASSERT_MSG_RETURN_FALSE(0, _T("Could not load the function to load an assembly and get a function pointer from it"));
        }
        /*hostfxr_load_assembly_and_get_function_pointer_func = load_assembly_and_get_function_pointer_fn(func_ptr);
        */
        return true;
    }
}