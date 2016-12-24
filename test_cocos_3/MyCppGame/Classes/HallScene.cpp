//
//  HallScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/19.
//
//

#include "HallScene.h"
#include "InviteScene.h"
#include "ShopScene.h"
#include "ExchangeScene.h"
#include "PokerDeskScene.h"
#include "PopAlertDialog.h"
#include "SettingScene.h"

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

void Hall::onEnter(){
    Layer::onEnter();
    
    UserData user_data = Global::getInstance()->user_data;
    userNameLabel->setString(user_data.nikename);
    
    char userInfoString[300];
    sprintf(userInfoString, "ID:%s\nVIP:无\n钻石:%d\n金币:%d\n银币:%d\n战斗次数:%d\n胜率:%s",user_data.account, user_data.diamond, user_data.gold, user_data.silver, user_data.gameTimes, user_data.winningPercent);
    userinfoLabel->setString(userInfoString);
}
void Hall::onExit(){
    Layer::onExit();
    
}

bool Hall::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    int chip[2][5] = {{500,1000,3000,5000,10000},{10,20,50,100,200}};
    for (int i = 0; i < 5; i++) {
        RoomItem* item = new RoomItem();
        item->autorelease();
        item->chipMin = chip[0][i];
        item->perMin = chip[1][i];
        item->type = 0;
        item->status = i % 3;
        sprintf(item->content, "≥%d\n底注%d", item->chipMin, item->perMin);
        switch (i) {
            case 0:{
                sprintf(item->title, "新手房");
            }
                break;
                
            case 1:{
                sprintf(item->title, "初级房");
            }
                break;
                
            case 2:{
                sprintf(item->title, "普通房");
            }
                break;
                
            case 3:{
                sprintf(item->title, "中级房");
            }
                break;
                
            case 4:{
                sprintf(item->title, "高级房");
            }
                break;
                
            default:
                break;
        }
        
        tianItems.pushBack(item);
    }
    
    for (int i = 0; i < 6; i++) {
        RoomItem* item = new RoomItem();
        item->autorelease();
        item->chipMin = -1;
        item->perMin = -1;
        item->type = 1;
        if (i < 5) {
            item->chipMin = chip[0][i];
            item->perMin = chip[1][i];
            sprintf(item->title, "创建vip房间");
            sprintf(item->content, "≥%d\n底注%d", item->chipMin, item->perMin);
        }
        else{
            sprintf(item->title, "加入房间");
            sprintf(item->content, "凭密码加入");
        }
        
        diItems.pushBack(item);
    }
    
    for (int i = 0; i < 6; i++) {
        RoomItem* item = new RoomItem();
        item->autorelease();
        item->chipMin = -1;
        item->perMin = -1;
        item->type = 1;
        if (i < 5) {
            item->chipMin = chip[0][i];
            item->perMin = chip[1][i];
            sprintf(item->title, "创建钻石房间");
            sprintf(item->content, "≥%d\n底注%d", item->chipMin, item->perMin);
        }
        else{
            sprintf(item->title, "加入房间");
            sprintf(item->content, "凭密码加入");
        }
        
        xuanItems.pushBack(item);
    }
    
    for (int i = 0; i < 1; i++) {
        RoomItem* item = new RoomItem();
        item->autorelease();
        item->chipMin = -1;
        item->perMin = -1;
        item->type = 2;
        
        switch (i) {
            case 0:{
                sprintf(item->title, "练习房");
                sprintf(item->content, "使用银币单机练习");
            }
                break;
                
            default:
                break;
        }
        
        huangItems.pushBack(item);
    }
    
    for (int i = 0; i < 6; i++) {
        NoteItem* item = new NoteItem();
        item->autorelease();
        sprintf(item->image, "images/server_btn_%d.png", i);
        
        noteItems.pushBack(item);
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
    
    userNameLabel = Label::createWithTTF("没有昵称", "fonts/STKaiti.ttf", 14);
    userNameLabel->setTextColor(Color4B::WHITE);
    userNameLabel->setPosition(Vec2(userInfoSprite->getContentSize().width / 2, userInfoSprite->getContentSize().height * 0.6));
    userInfoSprite->addChild(userNameLabel);
    
    userinfoLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 8);
    userinfoLabel->setTextColor(Color4B(0xe0, 0xe0, 0xe0, 0xff));
    userinfoLabel->setPosition(userInfoSprite->getContentSize().width / 2, 0.25 * userInfoSprite->getContentSize().height);
    userinfoLabel->setDimensions(0.95 * userInfoSprite->getContentSize().width, 0.5 * userInfoSprite->getContentSize().height);
    userInfoSprite->addChild(userinfoLabel);
    
    auto refresh_UserInfoItem = MenuItemImage::create(
                                                  "images/btn_fresh.png",
                                                  "images/btn_fresh.png",
                                                  CC_CALLBACK_1(Hall::buttonCallback, this, 0));
    refresh_UserInfoItem->setScale(0.3 * userInfoSprite->getContentSize().width / refresh_UserInfoItem->getContentSize().width);
    refresh_UserInfoItem->setPosition(0.85 * userInfoSprite->getContentSize().width, refresh_UserInfoItem->getBoundingBox().size.height * 0.6);
    userInfoSprite->addChild(refresh_UserInfoItem);
    
    
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
    
    roominfoLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 12);
    roominfoLabel->setTextColor(Color4B::BLACK);
    roominfoLabel->setPosition(roomListSprite->getContentSize().width / 2, 0.08 * roomListSprite->getContentSize().height);
    roominfoLabel->setDimensions(0.96 * roomListSprite->getContentSize().width, 0.16 * roomListSprite->getContentSize().height);
    roomListSprite->addChild(roominfoLabel);
    
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    roomListSprite->addChild(menu);
    for (int i = 0; i < 4; i++) {
        auto room_Item = MenuItemImage::create(
                                               "images/btn_noselect.png",
                                               "images/btn_select.png",
                                               CC_CALLBACK_1(Hall::roomTypeCallback, this, i));
        
        room_Item->setScale(roomListBG->getBoundingBox().size.height * 0.12 / room_Item->getContentSize().height);
        room_Item->setPosition(roomListTableView->getBoundingBox().getMinX() + (0.6 + i * 1.1) * room_Item->getBoundingBox().size.width, roomListBG->getBoundingBox().getMaxY() - 0.4 * room_Item->getBoundingBox().size.height);
        
        roomMenuItems.pushBack(room_Item);
        menu->addChild(room_Item);
    }
    
    //默认房间类型0
    this->roomTypeSelectedAction(0);
    
    auto noteListSprite = Sprite::create();
    noteListSprite->setContentSize(Size(visibleSize.width, visibleSize.height - userInfoSize.height));
    noteListSprite->setPosition(origin.x + noteListSprite->getContentSize().width / 2, origin.y + noteListSprite->getContentSize().height / 2);
    this->addChild(noteListSprite, 1);
    
    auto noteListBG = Sprite::create("images/note_list_bg.png");
    noteListBG->setScale(noteListSprite->getContentSize().width / noteListBG->getContentSize().width, noteListSprite->getContentSize().height / noteListBG->getContentSize().height);
    noteListBG->setPosition(Vec2(noteListBG->getBoundingBox().size.width / 2, noteListBG->getBoundingBox().size.height / 2));
    noteListSprite->addChild(noteListBG);
    
    noteListCellHeight = noteListBG->getBoundingBox().size.height * 0.5;
    noteListTableView = TableView::create(this, Size(noteListBG->getBoundingBox().size.width * 0.96,  noteListCellHeight));
    noteListTableView->setPosition(noteListBG->getBoundingBox().size.width * 0.02, noteListBG->getBoundingBox().size.height * 0.25);
    noteListTableView->setDirection(TableView::Direction::HORIZONTAL);
    noteListTableView->setDelegate(this);
    noteListSprite->addChild(noteListTableView);
    
    noteListTableView->reloadData();
    
    return true;
}

void Hall::roomTypeCallback(cocos2d::Ref* pSender, int index){
    this->roomTypeSelectedAction(index);
}

void Hall::roomTypeSelectedAction(int type){
    switch (type) {
        case 0:{
            roominfoLabel->setString("使用金币结算");
        }
            break;
            
        case 1:{
            roominfoLabel->setString("使用金币结算，vip玩家可创建房间，非vip玩家凭密码加入");
        }
            break;
            
        case 2:{
            roominfoLabel->setString("使用钻石结算，有足够钻石的玩家可创建或者加入房间");
        }
            break;
            
        case 3:{
            roominfoLabel->setString("使用银币结算，单机练习");
        }
            break;
            
        default:
            break;
    }
    roomTypeSelected = type;
    for (int i = 0; i < roomMenuItems.size(); i++) {
        MenuItem* item = roomMenuItems.at(i);
        if (i == roomTypeSelected) {
            item->selected();
        }
        else{
            item->unselected();
        }
    }
    roomListTableView->reloadData();
}

void Hall::buttonCallback(cocos2d::Ref* pSender, int index){
    
}
#pragma alert
void Hall::showSettingChip(bool needPassword){
    PopAlertDialog* popup = PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
    popup->setTitle("");
    if (needPassword) {
        popup->setContentText("请设置带入的游戏币数目并验证密码",12,50,100);
    }
    else{
        popup->setContentText("请设置带入的游戏币数目",12,50,130);
    }
    
    popup->setCallBackFunc(this,callfuncN_selector(Hall::popButtonCallback));
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "",0);
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "",1);
    
    this->addChild(popup, 2);
    
    ControlSlider* myslider = ControlSlider::create("images/slider_bg.png","images/slider_jd.png","images/slider_hk.png");
    myslider->setPosition(popup->getContentSize().width / 2, popup->getContentSize().height * 0.45);
    myslider->setMaximumValue(100);
    myslider->setMinimumValue(0);
    popup->addChild(myslider);
}

void Hall::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        auto scene = PokerDesk::createScene();
        PokerDesk* layer = (PokerDesk* )(scene->getChildren().at(1));
        layer->gamePlayer = new Player();
        layer->gamePlayer->infoConfig("阿罗", "images/default_head.png", 3000);
        
        TransitionScene* ts = TransitionMoveInR::create(0.2, scene);
        Director::getInstance()->pushScene(ts);
    }
    
    pNode->removeFromParent();
}

#pragma tableview
Size Hall::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == roomListTableView) {
        return Size(roomListCellScale * roomListCellHeight, roomListCellHeight);
    }
    else if (table == noteListTableView){
        return Size(noteListCellHeight, noteListCellHeight);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* Hall::tableCellAtIndex(TableView* table, ssize_t idx)
{
    if (table == roomListTableView) {
        RoomListCell* cell = (RoomListCell *)table->dequeueCell();
        
        if(!cell)
        {
            cell = new RoomListCell();
            cell->autorelease();
            
            cell->bg_sprite = Sprite::create();
            cell->bg_sprite->setPosition(0.5 * roomListCellScale * roomListCellHeight, 0.5 * roomListCellHeight);
            cell->bg_sprite->setContentSize(Size(0.9 * roomListCellScale * roomListCellHeight, 0.9 * roomListCellHeight));
            cell->addChild(cell->bg_sprite);
            
            auto cellBG = Sprite::create("images/room_bg2.png");
            cellBG->setScale(cell->bg_sprite->getContentSize().height / cellBG->getContentSize().height);
            cellBG->setPosition(cell->bg_sprite->getContentSize().width / 2, cell->bg_sprite->getContentSize().height / 2);
            cell->bg_sprite->addChild(cellBG);
            
            cell->contentLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 12);
            cell->contentLabel->setPosition(cell->bg_sprite->getContentSize().width / 2, (114.0 / 2 / 268.0) * cell->bg_sprite->getContentSize().height);
            cell->contentLabel->setDimensions(cell->bg_sprite->getContentSize().width, (114.0 / 268.0) * cell->bg_sprite->getContentSize().height);
            cell->contentLabel->setHorizontalAlignment(TextHAlignment::CENTER);
            cell->contentLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->bg_sprite->addChild(cell->contentLabel);
            
            cell->titleLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 16);
            cell->titleLabel->setPosition(cell->bg_sprite->getContentSize().width / 2, ((100.0 / 2 + 114.0) / 268.0) * cell->bg_sprite->getContentSize().height);
            cell->titleLabel->setDimensions(cell->bg_sprite->getContentSize().width, (100.0 / 268.0) * cell->bg_sprite->getContentSize().height);
            cell->titleLabel->setHorizontalAlignment(TextHAlignment::CENTER);
            cell->titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->bg_sprite->addChild(cell->titleLabel);
            
            cell->stateImage = Sprite::create("images/roomstate0.png");
            cell->stateImage->setScale((54.0 / 2.0 / 268.0) * cell->bg_sprite->getContentSize().height / cell->stateImage->getContentSize().height);
            cell->stateImage->setPosition(cell->bg_sprite->getContentSize().width * 0.8, ((54.0 / 2.0 + 214.0) / 268.0) * cell->bg_sprite->getContentSize().height);
            cell->bg_sprite->addChild(cell->stateImage);
        }
        
        RoomItem* room;
        
        switch (roomTypeSelected) {
            case 0:{
                room = tianItems.at(idx);
            }
                break;
                
            case 1:{
                room = diItems.at(idx);
            }
                break;
                
            case 2:{
                room = xuanItems.at(idx);
            }
                break;
                
            case 3:{
                room = huangItems.at(idx);
            }
                break;
                
            default:{
                
            }
                break;
        }
        
        if (room != NULL) {
            cell->contentLabel->setString(room->content);
            cell->titleLabel->setString(room->title);
            
            char Icon[30];
            sprintf(Icon, "images/roomstate%d.png", room->status);
            cell->stateImage->setTexture(Icon);
        }
        
        return cell;
    }
    else if (table == noteListTableView){
        NoteListCell* cell = (NoteListCell *)table->dequeueCell();
        
        if(!cell)
        {
            cell = new NoteListCell();
            cell->autorelease();
            
            cell->bg_sprite = Sprite::create();
            cell->bg_sprite->setPosition(0.5 * noteListCellHeight, 0.5 * noteListCellHeight);
            cell->bg_sprite->setContentSize(Size(0.9 * noteListCellHeight, 0.9 * noteListCellHeight));
            cell->addChild(cell->bg_sprite);
            
            auto cellBG = Sprite::create("images/server_btn_0.png");
            cellBG->setScale(cell->bg_sprite->getContentSize().height / cellBG->getContentSize().height);
            cellBG->setPosition(cell->bg_sprite->getContentSize().width / 2, cell->bg_sprite->getContentSize().height / 2);
            cell->bg_sprite->addChild(cellBG, 0 , 1);
        }
        
        NoteItem* item = noteItems.at((int)idx);
        if (item != NULL) {
            Sprite* sprite = (Sprite *)cell->bg_sprite->getChildByTag(1);
            sprite->setTexture(item->image);
        }
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t Hall::numberOfCellsInTableView(TableView* table)
{
    if (table == roomListTableView) {
        switch (roomTypeSelected) {
            case 0:{
                return tianItems.size();
            }
                break;
                
            case 1:{
                return diItems.size();
            }
                break;
                
            case 2:{
                return xuanItems.size();
            }
                break;
                
            case 3:{
                return huangItems.size();
            }
                break;
                
            default:{
                return 0;
            }
                break;
        }
    }
    else if (table == noteListTableView){
        return noteItems.size();
    }
    
    return 0;
}

void Hall::tableCellTouched(TableView* table, TableViewCell* cell){
    if (table == roomListTableView) {
        if (roomTypeSelected == 1 || roomTypeSelected == 2){
            this->showSettingChip(cell->getIdx() == 5);
        }
        else{
            this->showSettingChip(false);
        }
    }
    else if (table == noteListTableView){
        switch (cell->getIdx()) {
            case 0:{
                auto scene = ShopScene::createScene();
                
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 1:{
                auto scene = ExchangeScene::createScene();
                
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 2:{
                auto scene = InviteScene::createScene();
                
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 3:{
                
            }
                break;
                
            case 4:{
                auto scene = SettingScene::createScene();
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 5:{
                Director::getInstance()->popToRootScene();
            }
                break;
                
            default:
                break;
        }
    }
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
