#ifndef SCREEN1_VIEW_HPP
#define SCREEN1_VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <mvp/View.hpp>
#include <gui/common/ModalDialog.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <gui/containers/CustomContainer1.hpp>
#include <gui/model/QRCodeWidget.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void buttonUpClicked();
    virtual void buttonDownClicked();
    // Declaring callback handler for TextAreaWithOneWildcard
    void TextAreaClickHandler(const TextAreaWithOneWildcard& ta, const ClickEvent& e);
     
    virtual void handleClickEvent(const ClickEvent& evt);    
protected:
    QRCodeWidget      qrCode;
    QRCode                code;
    CustomContainer1  customContainer;
    ModalDialog       modalDialog;
    
    Callback<Screen1View, ModalDialog::Answer> onModalAnswered;
    void modalAnswered(ModalDialog::Answer answer);

    // Declaring callback type of ta and clickEvent
    Callback<Screen1View, const TextAreaWithOneWildcard&, const ClickEvent&> TextAreaClickedCallback;
    int TextValue;
};

#endif // SCREEN1_VIEW_HPP
