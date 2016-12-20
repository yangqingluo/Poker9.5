//
//  LoginScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "LoginScene.h"
#include "InputCell.h"

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
    
    char showTitle[4][100] = {"手机号","验证码","密码","确认密码"};
    char showContent[4][100] = {"输入手机号","输入验证码","输入密码","再次输入密码"};
    for (int i = 0; i < 4; i++) {
        LoginShowItem* item = new LoginShowItem();
        item->autorelease();
        memcpy(item->title, showTitle[i], strlen(showTitle[i]));
        memcpy(item->content, showContent[i], strlen(showContent[i]));
        
        showItems.pushBack(item);
    }
    
    auto inputListSprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.5 * visibleSize.width, 0.9 * visibleSize.height));
    inputListSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(inputListSprite);
    
    inputListTableView = TableView::create(this, Size(inputListSprite->getContentSize().width,  inputListSprite->getContentSize().height));
    inputListTableView->setPosition(0 , 0);
    inputListTableView->setDirection(TableView::Direction::VERTICAL);
    inputListTableView->setDelegate(this);
    inputListSprite->addChild(inputListTableView);
    
    inputListTableView->reloadData();
    
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
            
            break;
            
        default:{
        }
            break;
    }
}

#pragma tableview
Size LoginScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == inputListTableView) {
        return InputCell::tableCellSizeForIndex(table, idx);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* LoginScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
    if (table == inputListTableView) {
        InputCell* cell = (InputCell* )table->dequeueCell();
        
        if(!cell)
        {
            cell = InputCell::create();
            cell->setContentSize(tableCellSizeForIndex(table, idx));
        }
        
        LoginShowItem* item = showItems.at(idx);
        cell->textField->setPlaceHolder(item->content);
        
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t LoginScene::numberOfCellsInTableView(TableView* table)
{
    if (table == inputListTableView) {
        return showItems.size();
    }
    
    return 0;
}

void LoginScene::tableCellTouched(TableView* table, TableViewCell* cell){
    
}
