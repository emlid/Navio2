#ifndef LED
#define LED

enum class Colors {
    Black,
    Red,
    Green,
    Blue,
    Cyan,
    Magenta,
    Yellow,
    White};


class Led {

public:
    virtual bool initialize() = 0;
    virtual void setColor(Colors c) = 0;

};

#endif // LED

