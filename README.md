Repo for Issue https://github.com/dotnet/runtime/issues/118115

# CppCliFwLoading
Example repo to demonstrate the problem when loading a higher version of a framework assembly from C++/CLI.

This Repo example demonstrates that a .NET10 C# executable can load ystem.Runtime.Serialization.Formatters, Version=10.0.0.0 via a C++/CLI assembly.
However, this is not possible with a C++ executable via the same C++/CLI assembly.
This results in a System.IO.FileLoadException exception.

This makes it impossible to use the BinaryFormatter compatibility package from a C++/CLI context.
https://learn.microsoft.com/en-us/dotnet/standard/serialization/binaryformatter-migration-guide/compatibility-package


```
System.IO.FileLoadException: Could not load file or assembly 'System.Runtime.Serialization.Formatters, Version=10.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a'. The located assembly's manifest definition does not match the assembly reference. (0x80131040)
File name: 'System.Runtime.Serialization.Formatters, Version=10.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a'
 ---> System.IO.FileLoadException: Could not load file or assembly 'D:\Git\CppCliFwLoading\Debug\System.Runtime.Serialization.Formatters.dll'. The located assembly's manifest definition does not match the assembly reference. (0x80131040)
File name: 'D:\Git\CppCliFwLoading\Debug\System.Runtime.Serialization.Formatters.dll'
   at System.Runtime.Loader.AssemblyLoadContext.LoadFromAssemblyPath(String assemblyPath)
   at System.Runtime.Loader.AssemblyLoadContext.GetFirstResolvedAssemblyFromResolvingEvent(AssemblyName assemblyName)
   at System.Runtime.Loader.AssemblyLoadContext.ResolveUsingEvent(AssemblyName assemblyName)
   at CSLib.Class1.BinaryFormatterCall() in D:\Git\CppCliFwLoading\CSLib\Class1.cs:line 14
   at CSLib.Class1.BinaryFormatterCall() in D:\Git\CppCliFwLoading\CSLib\Class1.cs:line 14
   at CSLib.Class1.Test(Int32 x) in D:\Git\CppCliFwLoading\CSLib\Class1.cs:line 9
   at MyTest(Int32 test) in D:\Git\CppCliFwLoading\CppCliLib\Test.cpp:line 4
```
