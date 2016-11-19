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
    
    auto sprite = Sprite::create("images/new_bg.jpg");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    
    sprite->setScaleX(visibleSize.width / spx); //设置精灵宽度缩放比例
    sprite->setScaleY(visibleSize.height / spy);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    
    
    return true;
}

#pragma tableview
Size Hall::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    return Size::ZERO;
}

Size Hall::cellSizeForTable(TableView *table)
{
    return Size::ZERO;
};

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

#pragma scrollview delegate
//由于CCTableView是继承CCScrollView，所以要继承这两个方法
void scrollViewDidScroll(ScrollView* view) {
    
}
void scrollViewDidZoom(ScrollView* view) {
    
}

