#include "pch.h"

export module Hresult;

import Object;

export namespace win::debug
{
    class Hresult : public Object<Hresult>
    {
    private:
        HRESULT m_hresult = S_OK;
    public:
        Hresult() noexcept;
        Hresult(HRESULT hr) noexcept;
        Hresult(const Hresult& hr) noexcept;
        Hresult(Hresult&& hr) noexcept;

        Hresult& operator=(const Hresult& hr) noexcept;
        Hresult& operator=(Hresult&& hr) noexcept;

        bool IsSuccess() const noexcept;
        bool IsFailure() const noexcept;
        void ThrowIfFailed(const char* message) const;
        std::string Message() const;
    };
}
