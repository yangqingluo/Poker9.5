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
    sprite->setScale(visibleSize.width / spx, visibleSize.height / spy);
    this->addChild(sprite, 0);
    
    
    auto userInfoSprite = Sprite::create();
    userInfoSprite->setContentSize(Size((193.0 / 504.0) * 0.7 * visibleSize.height, 0.7 * visibleSize.height));
    userInfoSprite->setPosition(Vec2(origin.x + edge + 0.5 * userInfoSprite->getContentSize().width, visibleSize.height + origin.y - 0.5 * userInfoSprite->getContentSize().height));
    this->addChild(userInfoSprite, 1);
    
    auto userInfoSize = userInfoSprite->getContentSize();
    auto userInfoBG = Sprite::create("images/userinfo_bg.png");
    userInfoBG->setScale(userInfoSize.height / userInfoBG->getContentSize().height);
    userInfoBG->setPosition(Vec2(0.5 * userInfoSize.width, 0.5 * userInfoSize.height));
    userInfoSprite->addChild(userInfoBG);
    
    
    auto label = Label::createWithTTF("阿罗", "fonts/STKaiti.ttf", 12);
    label->setTextColor(Color4B::WHITE);
    
    label->setPosition(Vec2(userInfoSprite->getContentSize().width / 2, userInfoSprite->getContentSize().height * 0.5));
    userInfoSprite->addChild(label);
    
    auto roomListSprite = Sprite::create();
    roomListSprite->setContentSize(Size(visibleSize.width - userInfoSize.width - 3 * edge, userInfoSize.height));
    roomListSprite->setPosition(Vec2(userInfoSprite->getBoundingBox().getMaxX() + edge + roomListSprite->getContentSize().width / 2, userInfoSprite->getBoundingBox().getMinY() + roomListSprite->getContentSize().height / 2));
    this->addChild(roomListSprite, 1);
    
    
    auto roomListBG = Sprite::create("images/room_list_bg.png");
    roomListBG->setScale(roomListSprite->getContentSize().width / roomListBG->getContentSize().width, (465.0 / 504.0) * roomListSprite->getContentSize().height / roomListBG->getContentSize().height);
    roomListBG->setPosition(Vec2(roomListBG->getBoundingBox().size.width / 2, roomListBG->getBoundingBox().size.height / 2));
    roomListSprite->addChild(roomListBG);
    
    
    roomListCellHeight = roomListBG->getBoundingBox().size.height * 0.65;
    roomListTableView = TableView::create(this, Size(roomListBG->getBoundingBox().size.width * 0.96,  roomListCellHeight));
//    roomListTableView->autorelease();
    roomListTableView->setPosition(roomListBG->getBoundingBox().size.width * 0.02, roomListBG->getBoundingBox().size.height * 0.22);
    roomListTableView->setDirection(TableView::Direction::HORIZONTAL);
    //    roomListTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    roomListTableView->setDelegate(this);
    roomListSprite->addChild(roomListTableView);
    
    roomListTableView->reloadData();
    
    
    auto room_TianItem = MenuItemImage::create(
                                               "images/btn_noselect.png",
                                               "images/btn_select.png",
                                               CC_CALLBACK_1(Hall::roomTypeCallback, this, 0));
    
    room_TianItem->setPosition(roomListTableView->getBoundingBox().getMinX() + 0.5 * room_TianItem->getContentSize().width, roomListBG->getBoundingBox().getMaxY());
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(room_TianItem, NULL);
    menu->setPosition(Vec2::ZERO);
    roomListSprite->addChild(menu, 1);
    
    return true;
}

void Hall::roomTypeCallback(cocos2d::Ref* pSender, int index){
    
}

#pragma tableview
Size Hall::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    return Size(roomListCellScale * roomListCellHeight, roomListCellHeight);
}

//定制每个cell的内容
TableViewCell* Hall::tableCellAtIndex(TableView* table, ssize_t idx)
{
    char Icon[20];   //∏˘æ›idx—°÷–œ‘ æµƒÕº∆¨
    char number[10]; //œ‘ ælabel±Í«©µƒ ˝◊÷
    sprintf(Icon, "sp%ld.png", idx % 3 + 1);
    sprintf(number, "%02d", (int)idx);
    
    RoomListCell* cell = (RoomListCell *)table->dequeueCell();
    
    if(!cell)
    {
        cell = new RoomListCell();
        cell->autorelease();
        
        cell->bg_sprite = Sprite::create("images/room_bg2.png");
        //        cell->bg_sprite->setAnchorPoint(Point::ZERO);
        cell->bg_sprite->setPosition(0.5 * roomListCellScale * roomListCellHeight, 0.5 * roomListCellHeight);
        //        cell->bg_sprite->setContentSize(Size(0.9 * roomListCellScale * roomListCellHeight, 0.9 * roomListCellHeight));
        cell->addChild(cell->bg_sprite, 0, 1);
        
        //ÃÌº”“ª∏ˆlabel±Í«©
        Label* label = Label::createWithTTF(number, "fonts/arial.ttf", 8);
        label->setPosition(Point::ZERO);
        cell->addChild(label, 0, 2);
    }
    
    //    cell->bg_sprite->setScale(0.9 * roomListCellHeight / cell->bg_sprite->getContentSize().height);
    //    cell->bg_sprite->setContentSize(Size(0.9 * roomListCellScale * roomListCellHeight, 0.9 * roomListCellHeight));
    cell->bg_sprite->setScale(0.9 * roomListCellHeight / cell->bg_sprite->getContentSize().height);
    
    Label* label = (Label *)cell->getChildByTag(2);
    label->setString(number);
    
    return cell;
}

//确定这个tableview的cell行数
ssize_t Hall::numberOfCellsInTableView(TableView* table)
{
    return 20;
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
