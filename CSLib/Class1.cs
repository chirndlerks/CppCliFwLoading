namespace CSLib
{
	public static class Class1
	{
		static Class1()
		{
			SystemCollectionsImmutableCall();
		}

		public static int MyGlobalState { get; private set; }

		public static void SetGlobalState(int state)
		{
			MyGlobalState = state;
		}

		public static void SystemCollectionsImmutableCall()
		{
			var test = System.Collections.Immutable.ImmutableHashSet<int>.Empty;
		}
	}
}