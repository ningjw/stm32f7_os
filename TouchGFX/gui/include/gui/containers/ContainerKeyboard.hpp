#ifndef CONTAINERKEYBOARD_HPP
#define CONTAINERKEYBOARD_HPP

#include <gui_generated/containers/ContainerKeyboardBase.hpp>

class ContainerKeyboard : public ContainerKeyboardBase
{
public:
    typedef enum { ENTER, CANCEL } SpecialKey;
    
    ContainerKeyboard();
    virtual ~ContainerKeyboard() {}
    virtual void initialize();
        
    virtual void setTextArea(touchgfx::TextAreaWithOneWildcard &, touchgfx::Unicode::UnicodeChar *, uint8_t size);
    void setSpecialKeyCallback(touchgfx::GenericCallback<SpecialKey>& specialKeycallback);
    
private:
    bool isCapital;
    char key_values[TEXTKEYVALUES_SIZE];
    touchgfx::TextAreaWithOneWildcard *textArea;
    touchgfx::Unicode::UnicodeChar    *textBuff;
    uint8_t textBuffSize;
    
    touchgfx::GenericCallback<SpecialKey>*  specialKeycallback;
    
    /**
     * Callback for when special keys are pressed on the keyboard.
     */
    touchgfx::Callback<ContainerKeyboard, const touchgfx::AbstractButton&> specialKeyPressed;
     /**
     * Callback for when usual keys are pressed on the keyboard.
     */
    touchgfx::Callback<ContainerKeyboard, const touchgfx::AbstractButton&> usualKeyPressed;
    /**
     * Callback handler for usual key presses.
     */
    void usualKeyPressedhandler(const touchgfx::AbstractButton& btn);
    /**
     * Callback handler for special key presses.
     */
    void specialKeyPressedhandler(const touchgfx::AbstractButton& btn);
};

#endif // CONTAINERKEYBOARD_HPP
