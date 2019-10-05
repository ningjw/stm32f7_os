#ifndef CONTAINERDIALOG_HPP
#define CONTAINERDIALOG_HPP

#include <gui_generated/containers/ContainerDialogBase.hpp>

class ContainerDialog : public ContainerDialogBase
{
public:
    typedef enum { OK, CANCEL } Answer;
    ContainerDialog();
    virtual ~ContainerDialog() {}

    virtual void initialize();
        
    void setText(touchgfx::TypedTextId textId);
    void setAnsweredCallback(touchgfx::GenericCallback<Answer>& callback);
protected:
    
    touchgfx::GenericCallback<Answer>* callback;
    
    touchgfx::Callback<ContainerDialog, const touchgfx::AbstractButton&> onButtonClicked;
    void buttonClicked(const touchgfx::AbstractButton& btn);
};

#endif // CONTAINERDIALOG_HPP
