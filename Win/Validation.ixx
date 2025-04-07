import Handle;

export module Validation;

export namespace win::dbg
{
	void ThrowIfInvalid(Handle handle)
	{
		if (!handle.IsValid()) throw handle_exception("handle has invalid value");
	}
}