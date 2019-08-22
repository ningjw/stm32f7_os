#ifndef SCREEN1_VIEW_HPP
#define SCREEN1_VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void buttonUpClicked();
    virtual void buttonDownClicked();
    // Declaring callback handler for HiddenBox
    void TextAreaClickHandler(const TextAreaWithOneWildcard& ta, const ClickEvent& e);
protected:
    // Declaring callback type of box and clickEvent
    Callback<Screen1View, const TextAreaWithOneWildcard&, const ClickEvent&> TextAreaClickedCallback;
    int TextValue;
};

#endif // SCREEN1_VIEW_HPP
