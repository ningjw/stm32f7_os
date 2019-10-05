#include <gui/common/ModalDialog.hpp>
#include <BitmapDatabase.hpp>


ModalDialog::ModalDialog() :
    callback(0),
    onButtonClicked(this, &ModalDialog::buttonClicked)
{
    background.setBitmap(touchgfx::Bitmap(BITMAP_MODAL_BACKGROUND_ID));
    add(background);

    add(text);

    cancel.setBitmaps(touchgfx::Bitmap(BITMAP_CANCEL_ID), touchgfx::Bitmap(BITMAP_CANCEL_PRESSED_ID));
    cancel.setXY(18, 113);
    cancel.setAction(onButtonClicked);
    add(cancel);

    ok.setBitmaps(touchgfx::Bitmap(BITMAP_OK_ID), touchgfx::Bitmap(BITMAP_OK_PRESSED_ID));
    ok.setXY(154, 112);
    ok.setAction(onButtonClicked);
    add(ok);

    setWidth(background.getWidth());
    setHeight(background.getHeight());
}

void ModalDialog::setText(touchgfx::TypedTextId textId)
{
    text.setTypedText(touchgfx::TypedText(textId));
    text.setPosition(0, 50, 240, text.getTextHeight());
    invalidate();
}

void ModalDialog::setAnsweredCallback(touchgfx::GenericCallback<Answer>& callback)
{
    this->callback = &callback;
}

void ModalDialog::buttonClicked(const touchgfx::AbstractButton& btn)
{
    if (callback && callback->isValid())
    {
        callback->execute(&btn == &ok ? ModalDialog::OK : ModalDialog::CANCEL);
    }
}

