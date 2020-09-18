#ifndef EVENT_H
#define EVENT_H
#define EVENT_PROPERTY(TYPE,NAME)                                    \
public:                                                              \
    void set_ ## NAME ## (const TYPE ## & NAME ##)                   \
    {                                                                \
        _ ## NAME = NAME ## ;                                        \
    }                                                                \
    TYPE get_ ## NAME ## () const                                    \
    {                                                                \
        return _ ## NAME;                                            \
    }                                                                \
private:                                                             \
    TYPE _ ## NAME

namespace ice
{
    enum key_event_types
    {
        ICE_KEY_PRESSED,
        ICE_KEY_RELEASED
    };

    class key_event
    {
        EVENT_PROPERTY(int,event_type);
        EVENT_PROPERTY(int,key_code);
        EVENT_PROPERTY(bool,repeat);
    };

    enum mouse_event_types
    {
        ICE_MOUSE_MOTION,
        ICE_MOUSE_BUTTON,
        ICE_MOUSE_WHEEL
    };

    enum mouse_buttons
    {
        ICE_MOUSE_LMASK,
        ICE_MOUSE_MMASK,
        ICE_MOUSE_RMASK,
        ICE_MOUSE_X1MASK,
        ICE_MOUSE_x2MASK
    };

    class mouse_event
    {

    };

    class quit_event
    {
        EVENT_PROPERTY(int,timestamp);
    };

    class window_event
    {

    };
}


#endif