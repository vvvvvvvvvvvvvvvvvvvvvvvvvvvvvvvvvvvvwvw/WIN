#include "pch.h"
export module Console;

import Object;
import String;
import Handle;
import Hresult;
import Buffer;

export namespace win::debug
{
    class Console : public Object<Console>
    {
    private:
        static Handle m_output;
        static Handle m_input;
    private:

    
    public:
        enum class Color
        {
            Black = 0,
            Blue = 9,
            Green = 10,
            Cyan = 11,
            Red = 12,
            Purple = 13,
            Yellow = 14,
            White = 15,
            Gray = 16
        };
        enum class Background
        {
            Red = BACKGROUND_RED | BACKGROUND_INTENSITY,
            Green = BACKGROUND_GREEN | BACKGROUND_INTENSITY,
            Yellow = BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY,
        };

        Console() = delete;

        static String ReadLine();

        static void Write(Color color, const String& message);
        static void Write(const String& message);

        static void Write(const Buffer& buffer);

        static void WriteLine(Color color, const String& message);
        static void WriteLine(const String& message);

        static void Warning(const String& message);
        static void Error(const String& message);
        static void Info(const String& message);
        static void Success(const String& message);
        static void Clear();
        static void SetColor(Color color);
        static void Result(const Hresult& hr, const String& message);
    };
}
