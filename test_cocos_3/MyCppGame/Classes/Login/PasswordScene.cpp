//
//  PasswordScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "PasswordScene.h"
#include "RegistInputView.h"
#include "QLImageSprite.h"

USING_NS_CC;
using namespace ui;

Scene* PasswordScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PasswordScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool PasswordScene::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("images/new_bg.jpg");
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    sprite->setScaleX(visibleSize.width / spx);
    sprite->setScaleY(visibleSize.height / spy);
    this->addChild(sprite, 0);
    
    auto btn_BackItem = MenuItemImage::create(
                                              "images/window_close@2x.png",
                                              "images/window_close@2x.png",
                                              CC_CALLBACK_1(PasswordScene::buttonCallback, this, 0));
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto inputListSprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.5 * visibleSize.width, 0.95 * visibleSize.height));
    inputListSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(inputListSprite);
    
    char showContent[3][100] = {"11位手机号码","新密码(6-16位)","验证码"};
    float inputHeight = MAX(32, inputListSprite->getContentSize().height / 8);
    float inputY = 0.0;
    for (int i = 0; i < 3; i++) {
        auto inputBox = ui::EditBox::create(Size(0.9 * inputListSprite->getContentSize().width, 0.8 * inputHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
        inputBox->setPosition(Vec2(0.5 * inputListSprite->getContentSize().width, inputListSprite->getContentSize().height - (i * 1.0 + 0.8) * inputHeight));
        inputListSprite->addChild(inputBox);
        inputY = inputBox->getBoundingBox().getMinY();
        
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
        
        inputBox->setPlaceHolder(showContent[i]);
        switch (i) {
            case 0:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                inputBox->setMaxLength(11);
                usernameBox = inputBox;
            }
                break;
                
            case 1:{
                inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
                inputBox->setMaxLength(16);
                passwordBox = inputBox;
            }
                break;
                
            case 2:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                inputBox->setMaxLength(6);
                vcodeBox = inputBox;
                
                inputBox->setContentSize(Size(0.45 * inputBox->getContentSize().width, inputBox->getContentSize().height));
                inputBox->setPositionX(0.05 * inputListSprite->getContentSize().width + 0.5 * inputBox->getContentSize().width);
                
                auto btn_getVCode = Button::create("images/btn_green.png","images/btn_green.png");
                btn_getVCode->setScale9Enabled(true);//打开scale9 可以拉伸图片
                btn_getVCode->setTitleText("获取验证码");
                btn_getVCode->setTitleFontSize(12);
                btn_getVCode->setContentSize(inputBox->getContentSize());
                btn_getVCode->setPosition(Vec2(0.95 * inputListSprite->getContentSize().width - 0.5 * btn_getVCode->getContentSize().width, inputBox->getPositionY()));
                btn_getVCode->addTouchEventListener(CC_CALLBACK_2(PasswordScene::touchEvent, this));
                btn_getVCode->setTag(1);
                inputListSprite->addChild(btn_getVCode);
            }
                break;
                
            default:
                break;
        }
    }
    
    return true;
}

void PasswordScene::onEnter(){
    Layer::onEnter();
    
}

void PasswordScene::onExit(){
    Layer::onExit();
    
}

void PasswordScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
            break;
            
        default:{
        }
            break;
    }
}

void PasswordScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 1:{
                    if (strlen(usernameBox->getText()) != 11) {
                        NoteTip::show("手机号码输入有误");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        
                        
                    }
                }
                    break;
                    
                default:
                    break;
            }
            break;
            
        case Widget::TouchEventType::CANCELED:{
            
        }
            break;
            
        default:
            break;
    }
}
