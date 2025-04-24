export module Path;

import String;

export namespace win::io
{
	class Path
	{
	public:

		static String Combine(String path1, String path2)
		{
			return path1 + "\\" + path2;
		}

	};
}