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
        return Size(inputListTableView->getContentSize().width, 30);
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
            
            float cellWidth = inputListTableView->getContentSize().width;
            
            Label* titleLabel = Label::createWithTTF("test", "fonts/STKaiti.ttf", 12);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(cellWidth / 2, 15);
            titleLabel->setDimensions(cellWidth, 30);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::TOP);
            cell->addChild(titleLabel, 0 , 1);
            
            cell->titleLabel = titleLabel;
        }
        
        char content[100];
        if (idx % 2 == 0) {
            sprintf(content, "2016-10-%d\t+25金币\tVIP奖励",(int)idx);
        }
        else{
            sprintf(content, "2016-10-%d\t+1000金币\t充值",(int)idx);
        }
        
        cell->titleLabel->setString(content);
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t LoginScene::numberOfCellsInTableView(TableView* table)
{
    if (table == inputListTableView) {
        return 10;
    }
    
    return 0;
}

void LoginScene::tableCellTouched(TableView* table, TableViewCell* cell){
    
}
