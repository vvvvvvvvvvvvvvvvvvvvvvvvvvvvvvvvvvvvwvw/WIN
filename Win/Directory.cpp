#include "pch.h"

module Directory;

import Handle;
import String;
import File;
import Path;
import Hresult;
import Error;

namespace win::io
{
	void Directory::LoadFiles()
	{
		m_files.clear(); 

		WIN32_FIND_DATAW findData{ 0 };
		Handle hFind = FindFirstFileW((m_name + L"\\*").data(), &findData);

		if (!hFind.IsValid())
		{
			Hresult hr(GetLastError());
			hr.ThrowIfFailed("Failed to list directory");
		}

		do
		{
			String filename = findData.cFileName;
			if (filename == L"." || filename == L"..") continue;

			if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				File file(Path::Combine(m_name, filename), true);
				m_files.push_back(file);
			}

		} while (FindNextFileW(hFind, &findData));
	}

	Directory::Directory() {}

	Directory::Directory(String name) : m_name(name) {}

	Directory::Directory(String name, bool auto_mode) : m_name(name)
	{
		if (auto_mode)
		{
			if (!Exists(name)) Create();
			else LoadFiles();
		}
	}

	Directory::Directory(const Directory& other) : m_name(other.m_name), m_files(other.m_files) {}

	Directory::Directory(Directory&& other) noexcept : m_name(std::move(other.m_name)), m_files(std::move(other.m_files)) {}

	std::list<File>& Directory::GetFiles()
	{
		return m_files;
	}

	void Directory::Delete()
	{
		for (File& f : m_files)
			f.Delete();

		Delete(m_name);
	}

	void Directory::Create()
	{
		if (!CreateDirectoryW(m_name.data(), NULL))
		{
			Hresult hr(GetLastError());
			hr.ThrowIfFailed("Failed to create directory");
		}
	}

	void Directory::Add(File& file)
	{
		if (!Exists() || !file.Exists()) throw debug::Error("Pls Create Dir");

		file.Move(Path::Combine(m_name, file.Name()));
		m_files.push_back(file);
	}

	bool Directory::Exists() const
	{
		return Exists(m_name);
	}

	bool Directory::Exists(String name)
	{
		DWORD attr = GetFileAttributesW(name.data());
		return attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY);
	}

	void Directory::Delete(String name)
	{
		if (!RemoveDirectoryW(name.data()))
		{
			Hresult hr(GetLastError());
			hr.ThrowIfFailed("Failed to remove directory");
		}
	}

	Directory::~Directory() {}
}
