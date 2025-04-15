#include "pch.h"

module StreamWriter;

import Error;

namespace win::io
{
	StreamWriter::StreamWriter(Stream stream) : stream(stream)
	{
		if (!this->stream.IsValid()) throw debug::Error("Invalid stream");;
	}
	
	bool StreamWriter::Write(String content)
	{
		DWORD written = 0;
		return WriteFile(
			stream,
			content.data(),
			content.size(),
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
