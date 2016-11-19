//
//  HallScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/19.
//
//

#include "HallScene.h"

Scene* Hall::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Hall::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool Hall::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float edge = 10;
    
    auto sprite = Sprite::create("images/new_bg.jpg");
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    sprite->setScaleX(visibleSize.width / spx);
    sprite->setScaleY(visibleSize.height / spy);
    
    this->addChild(sprite, 0);
    
    auto userInfoSprite = Sprite::create("images/userinfo_bg.png");
    userInfoSprite->setScale(0.7 * visibleSize.height / userInfoSprite->getContentSize().height);
    auto userInfoSize = userInfoSprite->getBoundingBox().size;
    userInfoSprite->setPosition(Vec2(origin.x + 0.5 * userInfoSize.width + edge, visibleSize.height + origin.y - userInfoSize.height / 2));
    
    this->addChild(userInfoSprite, 1);
    
    auto label = Label::createWithTTF("阿罗", "fonts/STKaiti.ttf", 10);
    label->setTextColor(Color4B::WHITE);
    
    label->setPosition(Vec2(userInfoSprite->getContentSize().width / 2, userInfoSprite->getContentSize().height * 0.5));
    userInfoSprite->addChild(label);
    
    
    auto roomListSprite = Sprite::create("images/room_list_bg.png");
    roomListSprite->setScale((visibleSize.width - userInfoSize.width - 3 * edge) / roomListSprite->getContentSize().width, (465.0 / 504.0) * userInfoSize.height / roomListSprite->getContentSize().height);
    roomListSprite->setPosition(Vec2(userInfoSprite->getBoundingBox().getMaxX() + edge + roomListSprite->getBoundingBox().size.width / 2, userInfoSprite->getBoundingBox().getMinY() + roomListSprite->getBoundingBox().size.height / 2));
    
    this->addChild(roomListSprite, 1);
    
    return true;
}

#pragma tableview
Size Hall::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size::ZERO;
}


//定制每个cell的内容
TableViewCell* Hall::tableCellAtIndex(TableView *table, ssize_t idx)
{
    return TableViewCell::create();
}

//确定这个tableview的cell行数
ssize_t Hall::numberOfCellsInTableView(TableView *table)
{
    return 0;
}

void Hall::tableCellTouched(TableView* table, TableViewCell* cell){
    
}
//void Hall::tableCellHighlight(TableView* table, TableViewCell* cell){
//    
//}
//void Hall::tableCellUnhighlight(TableView* table, TableViewCell* cell){
//    
//}
//void Hall::tableCellWillRecycle(TableView* table, TableViewCell* cell){
//    
//}
//#pragma scrollview
////由于CCTableView是继承CCScrollView，所以要继承这两个方法
//void scrollViewDidScroll(ScrollView* view) {
//    
//}
//void scrollViewDidZoom(ScrollView* view) {
//    
//}

