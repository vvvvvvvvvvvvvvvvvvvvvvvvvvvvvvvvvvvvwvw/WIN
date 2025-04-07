#include <typeinfo>

export module Object;

export namespace win
{
    template<typename DT>
    class Object
    {
    public:
       
        const char* ToString();

        size_t HashCode();

        operator DT* ();

    };

    template<typename DT>
    const char* Object<DT>::ToString()
    {
        return typeid(DT).name();
    }

    template<typename DT>
    size_t Object<DT>::HashCode()
    {
        return typeid(DT).hash_code();
    }

    template<typename DT>
	Object<DT>::operator DT* ()
	{
		return static_cast<DT*>(this);
	}

}
