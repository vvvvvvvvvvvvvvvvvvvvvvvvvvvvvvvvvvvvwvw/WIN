#include "pch.h"

module Hresult;

namespace win
{
    Hresult::Hresult() noexcept
        : m_hresult(S_OK) {
    }

    Hresult::Hresult(HRESULT hr) noexcept
        : m_hresult(hr) {
    }

    Hresult::Hresult(const Hresult& hr) noexcept
        : m_hresult(hr.m_hresult) {
    }

    Hresult::Hresult(Hresult&& hr) noexcept
        : m_hresult(hr.m_hresult)
    {
        hr.m_hresult = S_OK;
    }

    Hresult& Hresult::operator=(const Hresult& hr) noexcept
    {
        if (this != &hr)
        {
            m_hresult = hr.m_hresult;
        }
        return *this;
    }

    Hresult& Hresult::operator=(Hresult&& hr) noexcept
    {
        if (this != &hr)
        {
            m_hresult = hr.m_hresult;
            hr.m_hresult = S_OK;
        }
        return *this;
    }

    bool Hresult::IsSuccess() const noexcept
    {
        return SUCCEEDED(m_hresult);
    }

    bool Hresult::IsFailure() const noexcept
    {
        return FAILED(m_hresult);
    }

    void Hresult::ThrowIfFailed(const char* message) const
    {
        if (FAILED(m_hresult))
        {
            throw std::system_error(m_hresult, std::system_category(), message);
        }
    }

    std::string Hresult::Message() const
    {
        return std::system_category().message(m_hresult);
    }
}
