int state = 42;
LoaderWrapper.Set(state);
int currentState = LoaderWrapper.Get();

Console.WriteLine(state);
Console.WriteLine(currentState);
Console.ReadLine();