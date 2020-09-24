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

    #define KEY_CODE(CODE) ICE_KEY_CODE_ ## CODE

    enum key_codes
    {
        KEY_CODE(BACKSPACE),
        KEY_CODE(TAB),
        KEY_CODE(CLEAR),
        KEY_CODE(RETURN),
        KEY_CODE(PAUSE),
        KEY_CODE(SPACE),
        KEY_CODE(EXCLAIM),
        KEY_CODE(QUOTEDBL),
        KEY_CODE(HASH),
        KEY_CODE(DOLLAR),
        KEY_CODE(AMPERSAND),
        KEY_CODE(QUOTE),
        KEY_CODE(LEFTPAREN),
        KEY_CODE(RIGHTPAREN),
        KEY_CODE(ASTERISK),
        KEY_CODE(PLUS),
        KEY_CODE(COMMA),
        KEY_CODE(MINUS),
        KEY_CODE(PERIOD),
        KEY_CODE(SLASH),
        KEY_CODE(0),
        KEY_CODE(1),
        KEY_CODE(2),
        KEY_CODE(3),
        KEY_CODE(4),
        KEY_CODE(5),
        KEY_CODE(6),
        KEY_CODE(7),
        KEY_CODE(8),
        KEY_CODE(9),
        KEY_CODE(COLON),
        KEY_CODE(SEMICOLON),
        KEY_CODE(LESS),
        KEY_CODE(EQUALS),
        KEY_CODE(GREATER),
        KEY_CODE(QUESTION),
        KEY_CODE(AT),
        KEY_CODE(LEFTBRACKET),
        KEY_CODE(BACKSLASH),
        KEY_CODE(RIGHTBRACKET),
        KEY_CODE(CARET),
        KEY_CODE(UNDERSCORE),
        KEY_CODE(BACKQUOTE),
        KEY_CODE(a),
        KEY_CODE(b),
        KEY_CODE(c),
        KEY_CODE(d),
        KEY_CODE(e),
        KEY_CODE(f),
        KEY_CODE(g),
        KEY_CODE(h),
        KEY_CODE(i),
        KEY_CODE(j),
        KEY_CODE(k),
        KEY_CODE(l),
        KEY_CODE(m),
        KEY_CODE(n),
        KEY_CODE(o),
        KEY_CODE(p),
        KEY_CODE(q),
        KEY_CODE(r),
        KEY_CODE(s),
        KEY_CODE(t),
        KEY_CODE(u),
        KEY_CODE(v),
        KEY_CODE(w),
        KEY_CODE(x),
        KEY_CODE(y),
        KEY_CODE(z),
        KEY_CODE(DELETE),
        KEY_CODE(KP0),
        KEY_CODE(KP1),
        KEY_CODE(KP2),
        KEY_CODE(KP3),
        KEY_CODE(KP4),
        KEY_CODE(KP5),
        KEY_CODE(KP6),
        KEY_CODE(KP7),
        KEY_CODE(KP8),
        KEY_CODE(KP9),
        KEY_CODE(KP_PERIOD),
        KEY_CODE(KP_DIVIDE),
        KEY_CODE(KP_MULTIPLY),
        KEY_CODE(KP_MINUS),
        KEY_CODE(KP_PLUS),
        KEY_CODE(KP_ENTER),
        KEY_CODE(KP_EQUALS),
        KEY_CODE(UP),
        KEY_CODE(DOWN),
        KEY_CODE(RIGHT),
        KEY_CODE(LEFT),
        KEY_CODE(INSERT),
        KEY_CODE(HOME),
        KEY_CODE(END),
        KEY_CODE(PAGEUP),
        KEY_CODE(PAGEDOWN),
        KEY_CODE(F1),
        KEY_CODE(F2),
        KEY_CODE(F3),
        KEY_CODE(F4),
        KEY_CODE(F5),
        KEY_CODE(F6),
        KEY_CODE(F7),
        KEY_CODE(F8),
        KEY_CODE(F9),
        KEY_CODE(F10),
        KEY_CODE(F11),
        KEY_CODE(F12),
        KEY_CODE(F13),
        KEY_CODE(F14),
        KEY_CODE(F15),
        KEY_CODE(NUMLOCK),
        KEY_CODE(CAPSLOCK),
        KEY_CODE(SCROLLOCK),
        KEY_CODE(RSHIFT),
        KEY_CODE(LSHIFT),
        KEY_CODE(RCTRL),
        KEY_CODE(LCTRL),
        KEY_CODE(RALT),
        KEY_CODE(LALT),
        KEY_CODE(RMETA),
        KEY_CODE(LMETA),
        KEY_CODE(LSUPER),
        KEY_CODE(RSUPER),
        KEY_CODE(MODE),
        KEY_CODE(HELP),
        KEY_CODE(PRINT),
        KEY_CODE(SYSREQ),
        KEY_CODE(BREAK),
        KEY_CODE(MENU),
        KEY_CODE(POWER),
        KEY_CODE(EURO)
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