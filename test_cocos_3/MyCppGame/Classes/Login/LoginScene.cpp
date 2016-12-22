//
//  LoginScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "LoginScene.h"
#include "RegistInputView.h"
#include "QLImageSprite.h"

USING_NS_CC;
using namespace ui;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LoginScene::init()
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
                                              CC_CALLBACK_1(LoginScene::buttonCallback, this, 0));
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto inputListSprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.5 * visibleSize.width, 0.9 * visibleSize.height));
    inputListSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(inputListSprite);
    
    char showContent[2][100] = {"11位手机号码","6-16位密码"};
    float inputHeight = MIN(30, inputListSprite->getContentSize().height / 8);
    float inputY = 0.0;
    for (int i = 0; i < 2; i++) {
        auto inputBox = EditBox::create(Size(0.9 * inputListSprite->getContentSize().width, 0.8 * inputHeight), Scale9Sprite::create("images/orange_edit.png"));
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
                inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
                inputBox->setMaxLength(16);
            }
                break;
                
            default:
                break;
        }
    }
    
    auto btn_login = MenuItemFont::create("登录", CC_CALLBACK_1(LoginScene::buttonCallback, this, 1));
    btn_login->setFontNameObj("fonts/STKaiti.ttf");
    btn_login->setFontSizeObj(16);
    btn_login->setColor(Color3B::BLACK);
    btn_login->setPosition(0.5 * inputListSprite->getContentSize().width, inputY - btn_login->getContentSize().height);
    
    auto menu_input = Menu::create(btn_login, NULL);
    menu_input->setPosition(Vec2::ZERO);
    inputListSprite->addChild(menu_input);
    
    return true;
}

void LoginScene::onEnter(){
    Layer::onEnter();
    
}

void LoginScene::onExit(){
    Layer::onExit();
    
}

void LoginScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
        case 1:{
            if (strlen(usernameBox->getText()) != 11) {
                
            }
            else if (strlen(passwordBox->getText()) < 6) {
                
            }
            else {
                
            }
        }
            break;
            
            break;
            
        default:{
        }
            break;
    }
}

#pragma edixBox
//开始编辑
void LoginScene::editBoxEditingDidBegin(EditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

//结束编辑
void LoginScene::editBoxEditingDidEnd(EditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

//编辑框内容改变
void LoginScene::editBoxTextChanged(EditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

//触发return返回
void LoginScene::editBoxReturn(EditBox* editBox)
{
    CCLog("editBox %p was returned !",editBox);
}

//#pragma tableview
//Size LoginScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
//{
//    if (table == inputListTableView) {
//        return InputCell::tableCellSizeForIndex(table, idx);
//    }
//    
//    return Size::ZERO;
//}
//
////定制每个cell的内容
//TableViewCell* LoginScene::tableCellAtIndex(TableView* table, ssize_t idx)
//{
//    if (table == inputListTableView) {
//        InputCell* cell = (InputCell* )table->dequeueCell();
//        
//        if(!cell)
//        {
//            cell = InputCell::create();
//            cell->setContentSize(tableCellSizeForIndex(table, idx));
//        }
//        
//        LoginShowItem* item = showItems.at(idx);
//        cell->inputBox->setPlaceHolder(item->content);
//        if (idx == 2 || idx == 3) {
//            cell->inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
//        }
//        else {
//            cell->inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
//        }
//        
//        return cell;
//    }
//    
//    return NULL;
//}
//
////确定这个tableview的cell行数
//ssize_t LoginScene::numberOfCellsInTableView(TableView* table)
//{
//    if (table == inputListTableView) {
//        return showItems.size();
//    }
//    
//    return 0;
//}
//
//void LoginScene::tableCellTouched(TableView* table, TableViewCell* cell){
//    
//}
//
//#pragma scrollview
//void LoginScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view){
//    
//}
//void LoginScene::scrollViewDidZoom(cocos2d::extension::ScrollView* view){
//    
//}
