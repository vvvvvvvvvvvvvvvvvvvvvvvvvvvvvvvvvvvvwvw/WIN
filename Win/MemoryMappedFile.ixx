export module MemoryMappedFile;

import Handle;
import String;
import Buffer;
import File;

export namespace win::io
{
    class MemoryMappedFile
    {
    public:
        MemoryMappedFile(const Handle& stream);
        MemoryMappedFile(const File& file);

        MemoryMappedFile(const MemoryMappedFile&) = delete;
        MemoryMappedFile& operator=(const MemoryMappedFile&) = delete;

        MemoryMappedFile(MemoryMappedFile&& other) noexcept;
        MemoryMappedFile& operator=(MemoryMappedFile&& other) noexcept;

        ~MemoryMappedFile();

        void Write(const String& content);
        void Write(const Buffer& buffer);

        template<typename T = void*>
        T* data();

        String ReadAll() const;

    private:

      
    private:
        void* pFile = nullptr;         
        unsigned long long mappedSize = 0;    

        void UnMap();                
    };

    template<typename T>
    T* MemoryMappedFile::data()
    {
        return (T*)pFile;
    }
}