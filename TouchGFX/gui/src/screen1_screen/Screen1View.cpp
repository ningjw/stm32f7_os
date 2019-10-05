#include <gui/screen1_screen/Screen1View.hpp>
#include "ff.h"
Screen1View::Screen1View():
      onModalAnswered(this, &Screen1View::modalAnswered),
          onDialogAnswered(this, &Screen1View::dialogAnswered),
          onSpecialKeyPressed(this, &Screen1View::keyboardFinished),
          // In constructor for callback, bind to this view object and bind which function to handle the event.
          TextAreaClickedCallback(this, &Screen1View::TextAreaClickHandler)
{

}

void Screen1View::setupScreen()
{
//    uint16_t width, height;
//    FILE* f = fopen("image.jpg", "rb");
//    BitmapId bmpId;
//    

    
    Screen1ViewBase::setupScreen();
    // Add the callback to textArea1
    textShowValue.setClickAction(TextAreaClickedCallback);
    textAreaEdit.setClickAction(TextAreaClickedCallback);
    // Add the modalDialog to Screen1
//    modalDialog.setXY(HAL::DISPLAY_WIDTH / 2 - modalDialog.getWidth() / 2, HAL::DISPLAY_HEIGHT / 2 - modalDialog.getHeight() / 2);
//    modalDialog.setText(T_DIALOGMSG);
//    modalDialog.setAnsweredCallback(onModalAnswered);
//    add(modalDialog);
    
    // Add the customContainer to Screen1
//    containerDialog.setXY(HAL::DISPLAY_WIDTH / 2 - containerDialog.getWidth() / 2, HAL::DISPLAY_HEIGHT / 2 - containerDialog.getHeight() / 2);
//    containerDialog.setText(T_DIALOGMSG);
//    containerDialog.setAnsweredCallback(onDialogAnswered);
//    add(containerDialog);
    
    containerKeyboard.setXY(HAL::DISPLAY_WIDTH / 2 - containerKeyboard.getWidth() / 2, HAL::DISPLAY_HEIGHT - containerKeyboard.getHeight());
    containerKeyboard.setVisible(false);
    add(containerKeyboard);
    
//    BMPFileLoader::getBMP24Dimensions(f, width, height);
    //Create (16bit) dynamic bitmap
//    bmpId = Bitmap::dynamicBitmapCreate(width, height, Bitmap::RGB565);
    //Load BMP file
//    BMPFileLoader::readBMP24File(Bitmap(bmpId), f);
//    //make Image show the loaded bitmap
//    image.setBitmap(Bitmap(bmpId));
//    //Position image and add to View
//    image.setXY(20, 20);
//    add(image);
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::buttonUpClicked()
{
    ++TextValue;
    Unicode::snprintf(textShowValueBuffer, TEXTSHOWVALUE_SIZE, "%d", TextValue);
    textShowValue.invalidate();
}

void Screen1View::buttonDownClicked()
{
    --TextValue;
    Unicode::snprintf(textShowValueBuffer, TEXTSHOWVALUE_SIZE, "%d", TextValue);
    textShowValue.invalidate();
}

void Screen1View::TextAreaClickHandler(const TextAreaWithOneWildcard& ta, const ClickEvent& evt)
{
    if (&ta == &textShowValue)
    {
        TextValue = 50;
        Unicode::snprintf(textShowValueBuffer, TEXTSHOWVALUE_SIZE, "%d", TextValue);
        textShowValue.invalidate();
    }
    else if(&ta == &textAreaEdit)
    {
        containerKeyboard.setTextArea(textAreaEdit, textAreaEditBuffer, TEXTAREAEDIT_SIZE);
        containerKeyboard.setVisible(true);
        containerKeyboard.setSpecialKeyCallback(onSpecialKeyPressed);
        containerKeyboard.invalidate();
    }
}

void Screen1View::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED && !modalDialog.isVisible())
    {
        modalDialog.setVisible(true);
        modalDialog.invalidate();
    }
    View::handleClickEvent(evt);
}

void Screen1View::modalAnswered(ModalDialog::Answer answer)
{
    containerDialog.setVisible(false);
    containerDialog.invalidate();
}

void Screen1View::dialogAnswered(ContainerDialog::Answer answer)
{
    modalDialog.setVisible(false);
    modalDialog.invalidate();
}

/***************************************************************************************
  * @brief   按键盘中的Enter与Cancel键后的回调函数
  * @input   
  * @return  
***************************************************************************************/
void Screen1View::keyboardFinished(ContainerKeyboard::SpecialKey keyType)
{
    
}
