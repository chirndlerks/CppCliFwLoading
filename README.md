# CppCliFwLoading
Example repo to demonstrate the problem when loading a higher version of a framework assembly from C++/CLI.

This Repo example demonstrates that a .NET8 C# executable can load System.Collections.Immutable, Version=9.0.0.0 via a C++/CLI assembly.
However, this is not possible with a C++ executable via the same C++/CLI assembly.
This results in a System.IO.FileLoadException exception.

```
System.IO.FileLoadException: Could not load file or assembly 'System.Collections.Immutable, Version=9.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a'. Could not find or load a specific file. (0x80131621)
File name: 'System.Collections.Immutable, Version=9.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a'
 ---> System.IO.FileLoadException: Could not load file or assembly 'System.Collections.Immutable, Version=9.0.0.0, Culture=neutral, PublicKeyToken=b03f5f7f11d50a3a'.
   at System.Runtime.Loader.AssemblyLoadContext.LoadFromAssemblyPath(String assemblyPath)
   at System.Runtime.Loader.AssemblyLoadContext.GetFirstResolvedAssemblyFromResolvingEvent(AssemblyName assemblyName)
   at System.Runtime.Loader.AssemblyLoadContext.ResolveUsingEvent(AssemblyName assemblyName)
   at System.Runtime.Loader.AssemblyLoadContext.ResolveUsingResolvingEvent(IntPtr gchManagedAssemblyLoadContext, AssemblyName assemblyName)
   at CSLib.Class1.SystemCollectionsImmutableCall()
```
# Side note
System.Collections.Immutable, Version=8.0.0.0 can be loaded without any problems in both cases.