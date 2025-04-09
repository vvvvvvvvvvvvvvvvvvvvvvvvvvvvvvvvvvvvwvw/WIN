export module StreamReader;

import String;
import Object;
import Handle;
import IDisposable;

export namespace win::io
{
	using Stream = Handle;

	class StreamReader : public interfaces::IDisposable, public Object<StreamReader>
	{
	private:
		Stream stream;

	public:
		StreamReader() = delete;
		StreamReader(const StreamReader& other) = delete;
		StreamReader(StreamReader&& other) = delete;
		StreamReader(Stream stream);

		String ReadToEnd();
		String ReadLine();
		String TryRead();

		void Dispose() override;

		~StreamReader();
	};
}
