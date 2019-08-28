#include <gui/containers/CustomContainer1.hpp>

CustomContainer1::CustomContainer1()
{

}

void CustomContainer1::initialize()
{
    CustomContainer1Base::initialize();
}

void CustomContainer1::buttonOkClicked()
{
    this->setVisible(false);
    invalidate();
}

void CustomContainer1::buttonCancelClicked()
{
    this->setVisible(false);
    invalidate();
}
