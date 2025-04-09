export module Time;

import TypeDef;
import Object;

export namespace win::system
{
    struct SystemTime {
        unsigned short Year;
        unsigned short Month;
        unsigned short DayOfWeek;
        unsigned short Day;
        unsigned short Hour;
        unsigned short Minute;
        unsigned short Second;
        unsigned short Milliseconds;
    };

    class Time : public Object<Time>
    {
    public:
        static SystemTime GetSystemTime();
        static SystemTime GetLocalTime();
    };

    class PerformanceCounter : public Object<Time>
    {
    public:
        static l_int GetValue();
        static l_int GetFrequency();
    };
}