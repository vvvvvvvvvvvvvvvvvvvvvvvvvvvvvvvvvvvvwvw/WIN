#include <Windows.h>

module StreamWriter;

namespace win::io
{
	StreamWriter::StreamWriter(Stream stream) : stream(stream)
	{
		if (!this->stream.IsValid()) throw 1;
	}

	bool StreamWriter::Write(String content)
	{
		DWORD written = 0;
		return WriteFile(
			stream,
			content.c_wstr().c_str(),                        
			static_cast<DWORD>(content.c_wstr().length() * sizeof(wchar_t)),
			&written,
			nullptr
		);
	}

	bool StreamWriter::WriteLine(String content)
	{
		String with_newline = content + L"\r\n";
		return Write(with_newline);
	}

	void StreamWriter::Dispose()
	{
		stream.Close();
	}

	StreamWriter::~StreamWriter()
	{
		Dispose();
	}
}
