import IDisposable;
import Object;
import String;
import Handle;

export module StreamWriter;

export namespace win::io
{
	using Stream = Handle;

	class StreamWriter : public interfaces::IDisposable, public Object<StreamWriter>
	{
	private:
		Stream stream;

	public:
		StreamWriter() = delete;
		StreamWriter(const StreamWriter& other) = delete;
		StreamWriter(StreamWriter&& other) = delete;
		StreamWriter(Stream stream);

		bool Write(String content);
		bool WriteLine(String content);

		void Dispose() override;

		~StreamWriter();
	};
}
