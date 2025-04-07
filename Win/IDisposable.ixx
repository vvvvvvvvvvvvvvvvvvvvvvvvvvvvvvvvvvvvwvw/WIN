export module IDisposable;

export namespace win::io::interfaces
{
	class IDisposable
	{
	public:
		virtual void Dispose() = 0;
		virtual ~IDisposable() = default;
	};
}