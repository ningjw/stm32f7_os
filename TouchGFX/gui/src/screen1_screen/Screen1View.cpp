#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
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

