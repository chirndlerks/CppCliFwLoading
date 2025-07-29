int state = 42;
SetGlobalStateWrapper.Set(state);
int currentState = GetGlobalStateWrapper.Get();

Console.WriteLine(state);
Console.WriteLine(currentState);
Console.ReadLine();