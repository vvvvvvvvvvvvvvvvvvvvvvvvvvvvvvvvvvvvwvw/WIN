#include <Windows.h>

export module File;

import Handle;
import Object;
import String;
import TypeDef;

export namespace win::io
{
    class File : public Object<File>
    {
    private:
        Handle m_handle;
        String m_name;
    public:
        File();
        File(const String& name);
        explicit File(const String& name, bool auto_mode = false);
        explicit File(Handle stream);
        File(const File& file);
        File(File&& file) noexcept;
        File& operator=(const File& file);
        File& operator=(File&& file) noexcept;
        ~File();

        Handle& GetStream();
        Handle GetStream() const;
        String ReadAll();
        void Write(String content);
        void Close();
        void Open();
        void Rename(const String& newname);
        void Delete();
        void Move(const String& to);
        void Copy(const String& to);
        bool Exists() const;
        String Name() const;
        l_int Size() const;
        static l_int Size(Handle stream);
    };
}
