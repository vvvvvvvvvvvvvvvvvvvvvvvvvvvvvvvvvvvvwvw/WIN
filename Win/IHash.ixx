import Object;

export module IHash;

export namespace win::interfaces
{
	template<typename hash_type>
	class IHash
	{
	public:
		virtual hash_type Hash(hash_type value) = 0;
		virtual ~IHash() = default;
	};
}