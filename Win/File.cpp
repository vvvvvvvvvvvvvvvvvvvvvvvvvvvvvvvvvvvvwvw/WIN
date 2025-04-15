#include "pch.h"

module File;

import String;
import Error;

namespace win::io
{
    File::File() : m_handle(), m_name() {}

    File::File(const String& name) : m_handle(), m_name(name) {}

    File::File(const String& name, bool auto_mode) : m_name(name)
    {
        if (auto_mode) Open();
    }

    File::File(Handle stream) : m_handle(stream), m_name(L"") {}

    File::File(const File& file) : m_handle(file.m_handle), m_name(file.m_name) {}

    File::File(File&& file) noexcept : m_handle(std::move(file.m_handle)), m_name(std::move(file.m_name)) {}

    File& File::operator=(const File& file)
    {
        if (this != &file)
        {
            m_handle = file.m_handle;
            m_name = file.m_name;
        }
        return *this;
    }

    File& File::operator=(File&& file) noexcept
    {
        if (this != &file)
        {
            m_handle = std::move(file.m_handle);
            m_name = std::move(file.m_name);
        }
        return *this;
    }

    File::~File()
    {
        Close();
    }

    Handle& File::GetStream()
    {
        return m_handle;
    }

    Handle File::GetStream() const
    {
        return m_handle;
    }

    void File::Close()
    {
        m_handle.Close();
    }

    void File::Open()
    {
        m_handle = CreateFileW(
            m_name.data(),
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ,
            nullptr,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        if (!m_handle.IsValid())
        {
            throw debug::SystemError("Canot open file");
        }
    }

    void File::Rename(const String& newname)
    {
        Move(newname);
    }

    void File::Delete()
    {
        if (!m_handle.IsValid())
            throw debug::Error("Invalid handle");

        Close();

        if (!DeleteFileW(m_name.data()))
        {
            throw debug::SystemError("Canot delete file");
        }
    }

    void File::Move(const String& to)
    {
        if (!m_handle.IsValid())
            throw debug::Error("Invalid handle");

        Close();

        if (!MoveFileW(m_name.data(), to.data()))
        {
            throw debug::SystemError("Canot move file");
        }
        m_name = to;

        Open();
    }

    void File::Copy(const String& to)
    {
        if (!m_handle.IsValid())
            throw debug::Error("Invalid handle");

        Close();

        if (!CopyFileW(m_name.data(), to.data(), FALSE))
        {
            throw debug::SystemError("Copy failed");
        }

        Open();
    }

    bool File::Exists() const
    {
        return m_handle.IsValid();
    }

    String File::Name() const
    {
        return m_name;
    }
    l_int File::Size() const
    {
        return Size(m_handle);
    }
    l_int File::Size(Handle stream)
    {
        l_int size;
        GetFileSizeEx(stream, &size);
        return size;
    }
}
