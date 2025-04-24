module;
#include "pch.h"

export module Buffer;

import Object;
import Error;
import String;


export namespace win {

    struct Buffer : public Object<Buffer> {
    private:
        wchar_t* m_data{ nullptr };
        size_t    m_len{ 0 };       
        size_t    m_capacity{ 0 }; 
    public:
        Buffer() = default;
        template<typename T>
        Buffer(const T& data);
        Buffer(const std::wstring& data);
        Buffer(const char* data);
        Buffer(size_t size);
        Buffer(const Buffer& other);
        Buffer& operator=(const Buffer& other);
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        ~Buffer();

     
        void resize(size_t newSize);
        void free();

        [[nodiscard]] size_t len() const noexcept { return m_len; }
        template<typename T = Char>
        [[nodiscard]] T* data();
        template<typename T = Char>
        [[nodiscard]] const T* data() const;
    };
    template<typename T>
    Buffer::Buffer(const T& data) {
        static_assert(std::is_trivially_copyable_v<T>, "Buffer<T>: T must be trivially copyable");
       
        size_t wc_count = (sizeof(T) + sizeof(wchar_t) - 1) / sizeof(wchar_t);
        m_capacity = wc_count + 1;             
        m_data = new wchar_t[m_capacity]();   
        std::memcpy(m_data, &data, sizeof(T));
        m_len = wc_count;
        m_data[m_len] = L'\0';
    }
    template<typename T>
    T* Buffer::data() {
        if (!m_data) throw debug::Error("Buffer is null");
        return reinterpret_cast<T*>(m_data);
    }
    template<typename T>
    const T* Buffer::data() const {
        if (!m_data) throw debug::Error("Buffer is null");
        return reinterpret_cast<const T*>(m_data);
    }

}
