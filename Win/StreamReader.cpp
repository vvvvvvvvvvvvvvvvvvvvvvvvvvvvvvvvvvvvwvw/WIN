#include <Windows.h>

import String;
import Handle;

module StreamReader;

namespace win::io
{
	StreamReader::StreamReader(Stream stream) : stream(stream)
	{
		if (!this->stream.IsValid()) throw 1;
	}

	String StreamReader::ReadToEnd()
	{
		SetFilePointer(stream, 0, NULL, FILE_BEGIN);

		wchar_t buffer[1024]{};
		DWORD bytesRead{};
		ReadFile(stream, buffer, sizeof(buffer), &bytesRead, nullptr);

		buffer[bytesRead / sizeof(wchar_t)] = L'\0';
		return String(buffer);
	}

	String StreamReader::TryRead()
	{
		SetFilePointer(stream, 0, NULL, FILE_BEGIN);

		wchar_t buffer[1024]{};
		DWORD bytesRead{};
		if (ReadFile(stream, buffer, sizeof(buffer), &bytesRead, nullptr))
		{
			buffer[bytesRead / sizeof(wchar_t)] = L'\0';
			return String(buffer);
		}
		return String();
	}

	void StreamReader::Dispose()
	{
		this->stream.Close();
	}

	StreamReader::~StreamReader()
	{
		Dispose();
	}
}
