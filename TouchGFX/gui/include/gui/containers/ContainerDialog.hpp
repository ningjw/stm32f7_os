#ifndef CONTAINERDIALOG_HPP
#define CONTAINERDIALOG_HPP

#include <gui_generated/containers/ContainerDialogBase.hpp>

class ContainerDialog : public ContainerDialogBase
{
public:
    ContainerDialog();
    virtual ~ContainerDialog() {}

    virtual void initialize();
protected:
};

#endif // CONTAINERDIALOG_HPP
