#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View():
    // In constructor for callback, bind to this view object and bind which function to handle the event.
  TextAreaClickedCallback(this, &Screen1View::TextAreaClickHandler)
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    // Add the callback to HiddenBox
    textArea1.setClickAction(TextAreaClickedCallback);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::buttonUpClicked()
{
    ++TextValue;
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", TextValue);
    textArea1.invalidate();
}

void Screen1View::buttonDownClicked()
{
    --TextValue;
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", TextValue);
    textArea1.invalidate();
}

void Screen1View::TextAreaClickHandler(const TextAreaWithOneWildcard& ta, const ClickEvent& evt)
{
    if (&ta == &textArea1)
    {
        TextValue = 50;
        Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%d", TextValue);
        textArea1.invalidate();
    }
}
