#include "pch.h"

import TypeDef;

module MemoryMappedFile;

namespace win::io
{
    MemoryMappedFile::MemoryMappedFile(const Handle& stream)
    {
        if (!stream.IsValid()) {
            throw std::runtime_error("Invalid stream handle");
        }

        l_int mappingSize = File::Size(stream);

        Handle hMapping = CreateFileMappingW(stream, 0, PAGE_READWRITE, mappingSize.HighPart, mappingSize.LowPart, nullptr);
        if (!hMapping.IsValid()) {
            throw std::runtime_error("Failed to create file mapping");
        }
        mappedSize = mappingSize.QuadPart;

        pFile = MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        if (!pFile) {
            hMapping.Close();
            throw std::runtime_error("Failed to map view of file");
        }
    }

    MemoryMappedFile::MemoryMappedFile(const File& file)
    {
        if (!file.GetStream().IsValid()) {
            throw std::runtime_error("Invalid stream handle");
        }

        l_int mappingSize = file.Size();

        Handle hMapping = CreateFileMappingW(file.GetStream(), 0, PAGE_READWRITE, mappingSize.HighPart, mappingSize.LowPart, nullptr);
        if (!hMapping.IsValid()) {
            throw std::runtime_error("Failed to create file mapping");
        }
        mappedSize = mappingSize.QuadPart;

        pFile = MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
        if (!pFile) {
            hMapping.Close();
            throw std::runtime_error("Failed to map view of file");
        }
    }

    MemoryMappedFile::MemoryMappedFile(MemoryMappedFile&& other) noexcept
        : pFile(other.pFile), mappedSize(other.mappedSize)
    {
        other.pFile = nullptr;
        other.mappedSize = 0;
    }

    MemoryMappedFile& MemoryMappedFile::operator=(MemoryMappedFile&& other) noexcept
    {
        if (this != &other) {
            UnMap();
            pFile = other.pFile;
            mappedSize = other.mappedSize;
            other.pFile = nullptr;
            other.mappedSize = 0;
        }
        return *this;
    }

    MemoryMappedFile::~MemoryMappedFile()
    {
        UnMap();
    }

    void MemoryMappedFile::UnMap()
    {
        if (pFile) {
            UnmapViewOfFile(pFile);
            pFile = nullptr;
        }
    }

    void MemoryMappedFile::Write(const String& content)
    {
        if (!pFile) {
            throw std::runtime_error("Memory not mapped");
        }
        if (mappedSize < (content.size() + 1) * sizeof(wchar_t))
        {
            throw std::runtime_error("Mapped Size lower than content size");
        }
        wcscpy_s((wchar_t*)(pFile), mappedSize / sizeof(wchar_t), content.data());
    }

    void MemoryMappedFile::Write(const Buffer& buffer)
    {
        if (!pFile) {
            throw std::runtime_error("Memory not mapped");
        }
        if (buffer.len() > mappedSize) {
            throw std::runtime_error("Content exceeds mapped size");
        }
        wcscpy_s((wchar_t*)pFile, mappedSize / sizeof(wchar_t), buffer.data<wchar_t>());
    }

    String MemoryMappedFile::ReadAll() const
    {
        if (!pFile) {
            throw std::runtime_error("Memory not mapped");
        }
        return String((wchar_t*)pFile);
    }
}