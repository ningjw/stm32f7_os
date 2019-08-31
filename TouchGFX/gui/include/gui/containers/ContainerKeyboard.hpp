#ifndef CONTAINERKEYBOARD_HPP
#define CONTAINERKEYBOARD_HPP

#include <gui_generated/containers/ContainerKeyboardBase.hpp>

class ContainerKeyboard : public ContainerKeyboardBase
{
public:
    ContainerKeyboard();
    virtual ~ContainerKeyboard() {}

    virtual void initialize();
protected:
};

#endif // CONTAINERKEYBOARD_HPP
