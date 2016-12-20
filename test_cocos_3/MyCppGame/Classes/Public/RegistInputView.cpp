#include "RegistInputView.h"

RegistInputView::RegistInputView(){
    
}

RegistInputView::~RegistInputView(){
    
}

bool RegistInputView::init(){
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->setContentSize(Size(0.5 * visibleSize.width, 0.9 * visibleSize.height));
    
    char showContent[8][100] = {"输入手机号","输入验证码","输入密码","再次输入密码","生日","邮箱","邀请码","验证码"};
    float inputHeight = MIN(30, this->getContentSize().height / 8);
    for (int i = 0; i < 8; i++) {
//        LoginShowItem* item = new LoginShowItem();
//        item->autorelease();
//        memcpy(item->title, showTitle[i], strlen(showTitle[i]));
//        memcpy(item->content, showContent[i], strlen(showContent[i]));
        
        auto inputBox = EditBox::create(Size(0.9 * this->getContentSize().width, 0.8 * inputHeight), Scale9Sprite::create("images/orange_edit.png"));
        inputBox->setPosition(Vec2(0.5 * this->getContentSize().width, this->getContentSize().height - (i * 1.0 + 0.8) * inputHeight));
        this->addChild(inputBox);
        
        //属性设置
        //    inputBox->setFontName("fonts/STKaiti.ttf");
        inputBox->setFontSize(12);
        inputBox->setFontColor(Color4B::BLACK);
        //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
        inputBox->setPlaceholderFontSize(12);
        inputBox->setPlaceholderFontColor(Color4B::GRAY);
        
        //模式类型设置
        inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
        inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
        inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
        
        //委托代理对象this
        inputBox->setDelegate(this);
        
        inputBox->setPlaceHolder(showContent[i]);
        switch (i) {
            case 0:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                inputBox->setMaxLength(11);
            }
                break;
                
            case 1:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                inputBox->setMaxLength(6);
            }
                break;
                
            case 2:
            case 3:{
                inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
                inputBox->setMaxLength(16);
            }
                break;
                
            case 4:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::DECIMAL);
                inputBox->setMaxLength(10);
            }
                break;
                
            case 5:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
                inputBox->setMaxLength(32);
            }
                break;
                
            default:
                break;
        }
    }
    
    
    
    
    
    return true;
}

void RegistInputView::onEnter(){
    LayerColor::onEnter();
    
    
}
void RegistInputView::onExit(){
    LayerColor::onExit();
    
}

#pragma edixBox
//开始编辑
void RegistInputView::editBoxEditingDidBegin(EditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

//结束编辑
void RegistInputView::editBoxEditingDidEnd(EditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

//编辑框内容改变
void RegistInputView::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

//触发return返回
void RegistInputView::editBoxReturn(EditBox* editBox)
{
    CCLog("editBox %p was returned !",editBox);
}

