#ifndef MODAL_DIALOG_HPP
#define MODAL_DIALOG_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Button.hpp>

class ModalDialog : public touchgfx::Container
{
public:
    typedef enum { OK, CANCEL } Answer;
    
    ModalDialog();

    void setText(touchgfx::TypedTextId textId);
    void setAnsweredCallback(touchgfx::GenericCallback<Answer>& callback);
private:
    touchgfx::Image    background;
    touchgfx::TextArea text;
    touchgfx::Button   ok;
    touchgfx::Button   cancel;

    touchgfx::GenericCallback<Answer>* callback;

    touchgfx::Callback<ModalDialog, const touchgfx::AbstractButton&> onButtonClicked;
    void buttonClicked(const touchgfx::AbstractButton& btn);
};

#endif

