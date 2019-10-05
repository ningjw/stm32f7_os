#include <gui/containers/ContainerDialog.hpp>

ContainerDialog::ContainerDialog():
    onButtonClicked(this, &ContainerDialog::buttonClicked)
{
    
}

void ContainerDialog::initialize()
{
    ContainerDialogBase::initialize();
}

void ContainerDialog::setText(touchgfx::TypedTextId textId)
{
    textArea1.setTypedText(touchgfx::TypedText(textId));
    textArea1.setPosition(0, 50, 240, textArea1.getTextHeight());
    invalidate();
}

void ContainerDialog::setAnsweredCallback(touchgfx::GenericCallback<Answer>& callback)
{
    this->callback = &callback;
}

void ContainerDialog::buttonClicked(const touchgfx::AbstractButton& btn)
{
    if (callback && callback->isValid())
    {
        callback->execute(&btn == &buttonOk ? ContainerDialog::OK : ContainerDialog::CANCEL);
    }
}
