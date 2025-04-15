#include "pch.h"

export module Directory;

import Object;
import String;
import File;

export namespace win::io
{
	class Directory : public Object<Directory>
	{
	private:
		std::list<File> m_files;
		String m_name;
	private:

		void LoadFiles();

	public:

		Directory();
		Directory(String name);
		Directory(String name, bool auto_mode);
		Directory(const Directory& other);
		Directory(Directory&& other) noexcept;

		std::list<File>& GetFiles();
		void Delete();
		void Create();
		void Add(File& file);
		bool Exists() const;

		static bool Exists(String name);
		static void Delete(String name);

		~Directory();

	};
}