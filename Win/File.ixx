#include <Windows.h>

import Handle;
import Object;
import String;

export module File;

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
        void Close();
        void Open();
        void Rename(const String& newname);
        void Delete();
        void Move(const String& to);
        void Copy(const String& to);
        bool Exists() const;
        String Name() const;
    };
}
