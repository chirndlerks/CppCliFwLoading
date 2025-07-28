namespace CSLib
{
	public static class Class1
	{
		public static int Test(int x)
		{
			SystemCollectionsImmutableCall();
			return x * 2;
		}

		public static void SystemCollectionsImmutableCall()
		{
			var test = System.Collections.Immutable.ImmutableHashSet<int>.Empty;
		}
	}
}