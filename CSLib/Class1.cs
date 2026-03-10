namespace CSLib;

using System.Runtime.Serialization.Formatters.Binary;

public static class Class1
{
	public static int Test(int x)
	{
		BinaryFormatterCall();
		return x * 2;
	}

	public static void BinaryFormatterCall()
	{
#pragma warning disable SYSLIB0011
		var test = new BinaryFormatter();
		var resourceStream = new MemoryStream();
		test.Serialize(resourceStream, "");
#pragma warning restore SYSLIB0011
	}
}