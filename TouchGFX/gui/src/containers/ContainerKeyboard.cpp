#include <gui/containers/ContainerKeyboard.hpp>
#include <string.h>
ContainerKeyboard::ContainerKeyboard():
    usualKeyPressed(this, &ContainerKeyboard::usualKeyPressedhandler),
        specialKeyPressed(this, &ContainerKeyboard::specialKeyPressedhandler)
{
    key_0.setAction(usualKeyPressed);
    key_1.setAction(usualKeyPressed);
    key_2.setAction(usualKeyPressed);
    key_3.setAction(usualKeyPressed);
    key_4.setAction(usualKeyPressed);
    key_5.setAction(usualKeyPressed);
    key_6.setAction(usualKeyPressed);
    key_7.setAction(usualKeyPressed);
    key_8.setAction(usualKeyPressed);
    key_9.setAction(usualKeyPressed);
    key_a.setAction(usualKeyPressed);
    key_b.setAction(usualKeyPressed);
    key_c.setAction(usualKeyPressed);
    key_d.setAction(usualKeyPressed);
    key_e.setAction(usualKeyPressed);
    key_f.setAction(usualKeyPressed);
    key_g.setAction(usualKeyPressed);
    key_h.setAction(usualKeyPressed);
    key_i.setAction(usualKeyPressed);
    key_j.setAction(usualKeyPressed);
    key_k.setAction(usualKeyPressed);
    key_l.setAction(usualKeyPressed);
    key_m.setAction(usualKeyPressed);
    key_n.setAction(usualKeyPressed);
    key_o.setAction(usualKeyPressed);
    key_p.setAction(usualKeyPressed);
    key_q.setAction(usualKeyPressed);
    key_r.setAction(usualKeyPressed);
    key_s.setAction(usualKeyPressed);
    key_t.setAction(usualKeyPressed);
    key_u.setAction(usualKeyPressed);
    key_v.setAction(usualKeyPressed);
    key_w.setAction(usualKeyPressed);
    key_x.setAction(usualKeyPressed);
    key_y.setAction(usualKeyPressed);
    key_z.setAction(usualKeyPressed);
    
    key_caps.setAction(usualKeyPressed);
    key_back.setAction(usualKeyPressed);
    
    key_enter.setAction(specialKeyPressed);
}

void ContainerKeyboard::initialize()
{
    ContainerKeyboardBase::initialize();
}


/***************************************************************************************
  * @brief   当按下常用的字母、数字按键时，在上方文本区域显示按键值
  * @input   
  * @return  
***************************************************************************************/
void ContainerKeyboard::usualKeyPressedhandler(const touchgfx::AbstractButton& btn)
{
    char key_value[1] = {0};
    
    if(&btn == &key_caps){//大写锁定按键
        isCapital = key_caps.getState();
        return;
    }else if(&btn == &key_back){//Backspace按键
        if(strlen(key_values) != 0){
            textKeyValuesBuffer[strlen(key_values) - 1] = 0;
            key_values[strlen(key_values) - 1] = 0;
            textKeyValues.invalidate();
        }
        return;
    }else if(&btn == &key_enter){//Enter 按键

    }else if(strlen(key_values) >= TEXTKEYVALUES_SIZE){//判断长度
        return;
    }else if(&btn == &key_0){
        key_value[0] = *key_0.getLabelText().getText();
    }else if(&btn == &key_1){
        key_value[0] = *key_1.getLabelText().getText();
    }else if(&btn == &key_2){
        key_value[0] = *key_2.getLabelText().getText();
    }else if(&btn == &key_3){
        key_value[0] = *key_3.getLabelText().getText();
    }else if(&btn == &key_4){
        key_value[0] = *key_4.getLabelText().getText();
    }else if(&btn == &key_5){
        key_value[0] = *key_5.getLabelText().getText();
    }else if(&btn == &key_6){
        key_value[0] = *key_6.getLabelText().getText();
    }else if(&btn == &key_7){
        key_value[0] = *key_7.getLabelText().getText();
    }else if(&btn == &key_8){
        key_value[0] = *key_8.getLabelText().getText();
    }else if(&btn == &key_9){
        key_value[0] = *key_9.getLabelText().getText();
    }else if(&btn == &key_a){
        key_value[0] = *key_a.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_b){
        key_value[0] = *key_b.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_c){
        key_value[0] = *key_c.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_d){
        key_value[0] = *key_d.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_e){
        key_value[0] = *key_e.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_f){
        key_value[0] = *key_f.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_g){
        key_value[0] = *key_g.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_h){
        key_value[0] = *key_h.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_i){
        key_value[0] = *key_i.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_j){
        key_value[0] = *key_j.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_k){
        key_value[0] = *key_k.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_l){
        key_value[0] = *key_l.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_m){
        key_value[0] = *key_m.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_n){
        key_value[0] = *key_n.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_o){
        key_value[0] = *key_o.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_p){
        key_value[0] = *key_p.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_q){
        key_value[0] = *key_q.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_r){
        key_value[0] = *key_r.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_s){
        key_value[0] = *key_s.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_t){
        key_value[0] = *key_t.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_u){
        key_value[0] = *key_u.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_v){
        key_value[0] = *key_v.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_w){
        key_value[0] = *key_w.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_x){
        key_value[0] = *key_x.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_y){
        key_value[0] = *key_y.getLabelText().getText() + (isCapital ? 0: 0x20);
    }else if(&btn == &key_z){
        key_value[0] = *key_z.getLabelText().getText() + (isCapital ? 0: 0x20);
    }
    
    if(strlen(key_values) < textBuffSize-1){
        strcat(key_values, key_value);
    }
    
    for(int i = 0; key_values[i] != 0 && i<textBuffSize-1; i++){
        textKeyValuesBuffer[i] = key_values[i];
    }
    textKeyValues.invalidate();
}

/***************************************************************************************
  * @brief   当按确认键或者取消键退出键盘界面时调用该函数，并执行回调函数
  * @input   
  * @return  
***************************************************************************************/
void ContainerKeyboard::specialKeyPressedhandler(const touchgfx::AbstractButton& btn)
{
    if(&btn == &key_enter){//按下确认按键，设置文本
        ContainerKeyboard::setVisible(false);
        ContainerKeyboard::invalidate();
        for(int i = 0; (i<(TEXTKEYVALUES_SIZE-1)) && (i<(textBuffSize-1)); i++){
            textBuff[i] = key_values[i];
        }
        textArea->invalidate();
    }
    
    //调用回调函数
    if (specialKeycallback && specialKeycallback->isValid())
    {
        specialKeycallback->execute( ContainerKeyboard::ENTER);
    }
}

/***************************************************************************************
  * @brief   设置回调函数
  * @input   
  * @return  
***************************************************************************************/
void ContainerKeyboard::setSpecialKeyCallback(touchgfx::GenericCallback<SpecialKey>& callback)
{
    this->specialKeycallback = &callback;
}

/***************************************************************************************
  * @brief   获取初始文本显示在键盘上面
  * @input   
  * @return  
***************************************************************************************/
void ContainerKeyboard::setTextArea(touchgfx::TextAreaWithOneWildcard &ta, touchgfx::Unicode::UnicodeChar *tb,uint8_t size)
{
    textArea = &ta;
    textBuff = tb;
    textBuffSize = size;
    Unicode::snprintf(textKeyValuesBuffer, TEXTKEYVALUES_SIZE, "%s", textArea->getWildcard());
    for(int i = 0; textKeyValuesBuffer[i] != 0 && i<textBuffSize-1; i++){
        key_values[i] = textKeyValuesBuffer[i];
    }
    textKeyValues.invalidate();
}


