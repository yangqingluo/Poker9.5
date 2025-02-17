//
//  HallScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/19.
//
//

#include "HallScene.h"
#include "ShopScene.h"
#include "ExchangeScene.h"
#include "PokerDeskScene.h"
#include "PopAlertDialog.h"
#include "SettingScene.h"
#include "HelpScene.h"
#include "OnlinePokerDeskScene.h"
#include "ChatScene.h"

#define dialogTag         9527
#define sliderTag         9528
#define passwordBoxTag    9529
#define capitalBoxTag     9530

static int chipTypeCount = 4;

Hall::Hall():m_pMessage(NULL){
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Hall::onNotification_Socket), kNotification_Socket, NULL);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Hall::onNotification_RefreshUserinfo), kNotification_RefreshUserInfo, NULL);
}
Hall::~Hall(){
    NotificationCenter::getInstance()->removeAllObservers(this);
}

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
    
    this->showUserInfo();
    
}

void Hall::showUserInfo(){
    UserData user_data = Global::getInstance()->user_data;
    userNameLabel->setString(user_data.nikename);
    
    char userInfoString[300];
    
    sprintf(userInfoString, "ID:%s\n钻石:%d\n金币:%d\n银币:%d\n战斗次数:%d\n胜率:%s",user_data.account, user_data.diamond, user_data.gold, user_data.silver, user_data.gameTimes, user_data.winningPercent);
    
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
    
    Global::getInstance()->playBackgroundMusic(true);
    
    int roomIDLength = 32;
    char goldRoomID[4][33] = {"11d01846ca6e4449ad1809426ff33b6f","9310fedae22b45a59d4ef9768a7bbeec","bdfe95b277e84ce5bfe2ce8123558bc8","f869fe26232b464c9159a87b0ee236a0"};
    char diamondRoomID[4][33] = {"281c8761602d41a8b91ed3ac3fabcbc5","cd027cf993434e22b0f908d3f1f51192","e867c097effb431f92d934fc66c997d5","da531815c8c54317b3db887d84c9952a"};
    
    int chip[2][4] = {{1000,3000,5000,10000},{20,50,100,200}};
    
    
    char goldTitle[4][20] = {"初级房","普通房","中级房","高级房"};
    for (int i = 0; i < chipTypeCount; i++) {
        RoomItem* item = new RoomItem();
        item->autorelease();
        item->chipMin = chip[0][i];
        item->perMin = chip[1][i];
        item->type = RoomType_Gold;
        item->status = i % 3;
        sprintf(item->content, "≥%d\n底注%d", item->chipMin, item->perMin);
        memcpy(item->title, goldTitle[i], strlen(goldTitle[i]));
        memcpy(item->typeID, goldRoomID[i], roomIDLength);
        
        tianItems.pushBack(item);
    }
    
    for (int i = 0; i < chipTypeCount + 1; i++) {
        RoomItem* item = new RoomItem();
        item->autorelease();
        if (i < chipTypeCount) {
            item->chipMin = chip[0][i];
            item->perMin = chip[1][i];
        }
        else {
            item->chipMin = chip[0][0];
            item->perMin = chip[1][0];
        }
        item->type = RoomType_Diamond;
        memcpy(item->typeID, diamondRoomID[i], roomIDLength);
        if (i < chipTypeCount) {
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
        item->chipMin = 500;
        item->perMin = 10;
        item->type = RoomType_Silver;
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
    userInfoSprite->setPosition(Vec2(origin.x + edge + 0.5 * userInfoSprite->getContentSize().width, 0.98 * visibleSize.height + origin.y - 0.5 * userInfoSprite->getContentSize().height));
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
    
    userinfoLabel = Label::create();
    userinfoLabel->setSystemFontSize(7.0);
    userinfoLabel->setTextColor(Color4B(0xe0, 0xe0, 0xe0, 0xff));
    userinfoLabel->setPosition(0.55 * userInfoSprite->getContentSize().width, 0.25 * userInfoSprite->getContentSize().height);
    userinfoLabel->setDimensions(0.95 * userInfoSprite->getContentSize().width, 0.5 * userInfoSprite->getContentSize().height);
    userInfoSprite->addChild(userinfoLabel);
    
    
    auto btn_refresh_UserInfo = Button::create("images/btn_fresh.png");
//    btn_refresh_UserInfo->setScale9Enabled(true);//打开scale9 可以拉伸图片
//    btn_refresh_UserInfo->setContentSize(Size(0.3 * userInfoSprite->getContentSize().width, 0.3 * userInfoSprite->getContentSize().width));
    btn_refresh_UserInfo->cocos2d::Node::setScale(0.3 * userInfoSprite->getContentSize().width / btn_refresh_UserInfo->getContentSize().width);
    btn_refresh_UserInfo->setPosition(Vec2(0.85 * userInfoSprite->getContentSize().width, btn_refresh_UserInfo->getBoundingBox().size.height * 0.6));
    btn_refresh_UserInfo->setTag(1);
    btn_refresh_UserInfo->addTouchEventListener(CC_CALLBACK_2(Hall::touchEvent, this));
    userInfoSprite->addChild(btn_refresh_UserInfo);
    
    
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
    
    
    char roomTypeTitle[4][20] = {"金币房","钻石房","练习房"};
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    roomListSprite->addChild(menu);
    for (int i = 0; i < 3; i++) {
        auto room_Item = MenuItemImage::create(
                                               "images/btn_noselect.png",
                                               "images/btn_select.png",
                                               CC_CALLBACK_1(Hall::roomTypeCallback, this, i));
        
        room_Item->setScale(roomListBG->getBoundingBox().size.height * 0.12 / room_Item->getContentSize().height);
        room_Item->setPosition(roomListTableView->getBoundingBox().getMinX() + (0.6 + i * 1.1) * room_Item->getBoundingBox().size.width, roomListBG->getBoundingBox().getMaxY() - 0.4 * room_Item->getBoundingBox().size.height);
        
        roomMenuItems.pushBack(room_Item);
        menu->addChild(room_Item);
        
        Label* ltf = Label::create();
        ltf->setSystemFontSize(10.0);
        ltf->setColor(Color3B::GRAY);
        ltf->setPosition(room_Item->getPosition());
//        ltf->setDimensions(room_Item->getContentSize().width, room_Item->getContentSize().height);
//        ltf->setHorizontalAlignment(TextHAlignment::CENTER);
//        ltf->setVerticalAlignment(TextVAlignment::CENTER);
        ltf->setString(roomTypeTitle[i]);
        roomListSprite->addChild(ltf);
    }
    
    //默认房间类型0
    this->roomTypeSelectedAction(0);
    
    auto noteListSprite = QLImageSprite::create("images/note_list_bg.png", Size(1.0 * visibleSize.width, userInfoSprite->getBoundingBox().getMinY() - origin.y));
    noteListSprite->setPosition(origin.x + noteListSprite->getContentSize().width / 2, origin.y + noteListSprite->getContentSize().height / 2);
    this->addChild(noteListSprite, 2);
    
    noteListCellHeight = MIN(noteListSprite->getBoundingBox().size.height * 0.5, 36);
    
    auto noteNoticeBG = Sprite::create("images/notice_no_gamble.png");
    noteNoticeBG->setScale(0.3 * noteListSprite->getContentSize().width / noteNoticeBG->getContentSize().width);
    noteNoticeBG->setPosition(noteListSprite->getContentSize().width - 0.5 * noteNoticeBG->getBoundingBox().size.width - 1.5 * edge, 0.5 * noteListSprite->getBoundingBox().size.height);
    noteListSprite->addChild(noteNoticeBG);
    
    noteListTableView = TableView::create(this, Size(noteListSprite->getBoundingBox().size.width * 0.96,  noteListCellHeight));
    noteListTableView->setPosition(noteListSprite->getBoundingBox().size.width * 0.02, 0.5 * noteListSprite->getBoundingBox().size.height - 0.5 * noteListCellHeight);
    noteListTableView->setDirection(TableView::Direction::HORIZONTAL);
    noteListTableView->setDelegate(this);
    noteListSprite->addChild(noteListTableView);
    
    noteListTableView->reloadData();
        
    msgLabel = Label::create();
    msgLabel->setSystemFontSize(14.0);
    msgLabel->setColor(Color3B::WHITE);
    this->addChild(msgLabel, 20);
    
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
            roominfoLabel->setString("使用钻石结算，有足够钻石的玩家可创建或者加入房间");
        }
            break;
            
        case 2:{
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

void Hall::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 0:{
                    
                }
                    break;
                    
                case 1:{
                    m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                    onHttpRequest_SearchUser(Global::getInstance()->user_data.account);
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

#pragma alert
void Hall::showSettingChip(){
    bool passwordEnter = false;//是否需要密码输入
    bool capitalEnter = true;//是否需要本金输入
    
    RoomItem* room = NULL;
    char m_show_string[200] = {0};
    
    switch (roomTypeSelected) {
        case 0:{
            room = tianItems.at(roomIndexSelected);
            if (Global::getInstance()->user_data.gold < room->chipMin) {
                NoteTip::show("您的金币不足");
                return;
            }
            sprintf(m_show_string, "加入%d金币的房间", room->chipMin);
        }
            break;
            
        case 1:{
            room = xuanItems.at(roomIndexSelected);
            if (roomIndexSelected + 1 == xuanItems.size()) {
                passwordEnter = true;
                sprintf(m_show_string, "加入钻石房间");
            }
            else {
                if (Global::getInstance()->user_data.diamond < room->chipMin) {
                    NoteTip::show("您的钻石不足");
                    return;
                }
                capitalEnter = false;
                sprintf(m_show_string, "创建%d钻石的房间", room->chipMin);
            }
        }
            break;
            
        case 2:{
            room = huangItems.at(roomIndexSelected);
            if (Global::getInstance()->user_data.silver < room->chipMin) {
                NoteTip::show("您的银币不足");
                return;
            }
            sprintf(m_show_string, "加入练习房(银币>=%d)", room->chipMin);
        }
            break;
            
        default:
            break;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PopAlertDialog* popup = PopAlertDialog::create("images/bg_dialog_empty.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
    popup->setTitle(m_show_string);
    
    popup->setTag(dialogTag);
    popup->setCallBackFunc(this,callfuncN_selector(Hall::popButtonCallback));
    
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "","",1);
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "","",0);
    
    this->addChild(popup, 2);
    
    float boxHeight = MIN(0.15 * popup->m_dialogContentSize.height, 32);
    if (capitalEnter) {
        auto inputBox = ui::EditBox::create(Size(0.4 * popup->m_dialogContentSize.width, boxHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
        inputBox->setPosition(Vec2(popup->getContentSize().width / 2, 0.55 * popup->getContentSize().height));
        inputBox->setTag(capitalBoxTag);
        popup->addChild(inputBox);
        
        //属性设置
        //    inputBox->setFontName("fonts/STKaiti.ttf");
        inputBox->setFontSize(12);
        inputBox->setFontColor(Color4B::BLACK);
        //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
        inputBox->setPlaceholderFontSize(12);
        inputBox->setPlaceholderFontColor(Color4B::GRAY);
        
        //模式类型设置
        inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        
        inputBox->setPlaceHolder("带入房间的本金");
        inputBox->setMaxLength(length_room_capital);
    }
    
    if (passwordEnter) {
        auto inputBox = ui::EditBox::create(Size(0.4 * popup->m_dialogContentSize.width, boxHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
        inputBox->setPosition(Vec2(popup->getContentSize().width / 2, 0.45 * popup->getContentSize().height));
        inputBox->setTag(passwordBoxTag);
        popup->addChild(inputBox);
        
        //属性设置
        //    inputBox->setFontName("fonts/STKaiti.ttf");
        inputBox->setFontSize(12);
        inputBox->setFontColor(Color4B::BLACK);
        //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
        inputBox->setPlaceholderFontSize(12);
        inputBox->setPlaceholderFontColor(Color4B::GRAY);
        
        //模式类型设置
        inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
        
        inputBox->setPlaceHolder("房间密码");
        inputBox->setMaxLength(length_room_password);
    }
}

void Hall::showMessageBox(const char* subTitle, const char* title){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PopAlertDialog* popup = PopAlertDialog::create("images/bg_dialog_empty.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
    popup->setTitle(title);
    popup->setContentText(subTitle, 15, 50, 0.45 * popup->getContentSize().height);
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "","",0);
    
    this->addChild(popup, INT_MAX);
}

void Hall::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        PopAlertDialog* popup = (PopAlertDialog *)this->getChildByTag(dialogTag);
        if (popup == NULL) {
            return;
        }
        cocos2d::ui::EditBox* capitalBox = (cocos2d::ui::EditBox* )popup->getChildByTag(capitalBoxTag);
        if (capitalBox == NULL) {
            
        }
        else {
            jettonToEnter = atoi(capitalBox->getText());
            if (jettonToEnter <= 0) {
                NoteTip::show("本金输入有误");
                return;
            }
        }
        
        RoomItem* room = NULL;
        switch (roomTypeSelected) {
            case 0:{
                room = tianItems.at(roomIndexSelected);
                
                if (jettonToEnter > Global::getInstance()->user_data.gold) {
                    NoteTip::show("金币不足");
                }
                else if (room->chipMin > jettonToEnter) {
                    NoteTip::show("带入金币不足，不能加入");
                }
                else {
                    auto scene = OnlinePokerDesk::createScene();
                    OnlinePokerDesk* layer = (OnlinePokerDesk* )(scene->getChildren().at(1));
                    layer->roomType = room->type;
                    strcpy(layer->roomTypeId, room->typeID);
                    layer->jettonToEnter = jettonToEnter;
                    
                    Director::getInstance()->pushScene(scene);
                }
            }
                break;
                
            case 1:{
                if (roomIndexSelected + 1 == xuanItems.size()) {
                    cocos2d::ui::EditBox* box = (cocos2d::ui::EditBox* )popup->getChildByTag(passwordBoxTag);
                    if (box) {
                        if (strlen(box->getText()) != length_room_password) {
                            NoteTip::show("密码输入有误");
                        }
                        else {
                            auto scene = OnlinePokerDesk::createScene();
                            OnlinePokerDesk* layer = (OnlinePokerDesk* )(scene->getChildren().at(1));
                            layer->roomType = RoomType_Diamond;
                            layer->jettonToEnter = jettonToEnter;
                            strcpy(layer->roomPassword, box->getText());
                            
                            Director::getInstance()->pushScene(scene);
                        }
                    }
                }
                else {
                    room = xuanItems.at(roomIndexSelected);
                    if (room->chipMin > Global::getInstance()->user_data.diamond) {
                        NoteTip::show("钻石不足，不能创建");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_CreateRoom(room->typeID);
                    }
                }
            }
                break;
                
            case 2:{
                room = huangItems.at(roomIndexSelected);
                
                if (jettonToEnter > Global::getInstance()->user_data.silver) {
                    NoteTip::show("银币不足");
                }
                else if (room->chipMin > jettonToEnter) {
                    NoteTip::show("带入银币不足，不能加入");
                }
                else {
                    auto scene = PokerDesk::createScene();
                    PokerDesk* layer = (PokerDesk* )(scene->getChildren().at(1));
                    layer->roomType = room->type;
                    layer->perMin = room->perMin;
                    layer->chipMin = room->chipMin;
                    layer->gamePlayer->infoConfig(Global::getInstance()->user_data.nikename, "images/default_head.png", jettonToEnter);
                    
                    Director::getInstance()->pushScene(scene);
                }
            }
                break;
            default:
                break;
        }
        
    }
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
                room = xuanItems.at(idx);
            }
                break;
                
            case 2:{
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
            
            cell->bg_sprite = QLImageSprite::create("images/server_btn_0.png", Size(0.9 * noteListCellHeight, 0.9 * noteListCellHeight));
            cell->bg_sprite->setPosition(0.5 * noteListCellHeight, 0.5 * noteListCellHeight);
            cell->bg_sprite->setContentSize(Size(0.9 * noteListCellHeight, 0.9 * noteListCellHeight));
            cell->addChild(cell->bg_sprite);
            
        }
        
        NoteItem* item = noteItems.at((int)idx);
        if (item != NULL) {
            cell->bg_sprite->setBackgroundImage(item->image);
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
                return xuanItems.size();
            }
                break;
                
            case 2:{
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
        roomIndexSelected = (int)cell->getIdx();
        this->showSettingChip();
    }
    else if (table == noteListTableView){
        switch (cell->getIdx()) {
            case 0:{
                auto scene = ShopScene::createScene();
                
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 1:{
//                auto scene = ExchangeScene::createScene();
//                
//                Director::getInstance()->pushScene(scene);
                NoteTip::show("精彩功能，敬请期待");
            }
                break;
                
            case 2:{
                auto scene = HelpScene::createScene();
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 3:{
                auto scene = SettingScene::createScene();
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 4:{
                auto scene = ChatScene::createScene();
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 5:{
                Global::getInstance()->logout();
            }
                break;
                
            default:
                break;
        }
    }
}

#pragma edixBox
//开始编辑
void Hall::editBoxEditingDidBegin(ui::EditBox* editBox){
}

//结束编辑
void Hall::editBoxEditingDidEnd(ui::EditBox* editBox){
}

//编辑框内容改变
void Hall::editBoxTextChanged(ui::EditBox* editBox, const std::string& text){
}

//触发return返回
void Hall::editBoxReturn(ui::EditBox* editBox){
    
}

#pragma http
//创建房间
void Hall::onHttpRequest_CreateRoom(const char* roomTypeId){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(Global::getInstance()->getURLWithSuffix("room/createRoom"));
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "roomTypeId=%s&userId=%s", roomTypeId, Global::getInstance()->user_data.ID);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(Hall::onHttpResponse, this));
    request->setTag("createRoom");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

// 搜索用户信息
void Hall::onHttpRequest_SearchUser(const char* account){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(Global::getInstance()->getURLWithSuffix("user/userinfo"));
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "account=%s", account);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(Hall::onHttpResponse, this));
    request->setTag("searchuser");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

// HTTP响应请求函数
void Hall::onHttpResponse(HttpClient* sender, HttpResponse* response){
    if (m_pMessage != NULL) {
        m_pMessage->hidden();
        m_pMessage = NULL;
    }
    
    // 没有收到响应
    if (!response){
        NoteTip::show("请检查网络");
        return;
    }
    
    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLOG("response code: %s", statusString);
    
    if (statusCode > 200) {
        NoteTip::show("网络错误");
        return;
    }
    // 链接失败
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        NoteTip::show("请检查网络");
        return;
    }
    
    std::vector<char>* responseData = response -> getResponseData();
    std::string responseStr = std::string(responseData -> begin(), responseData -> end());
    log("%s\n", responseStr.c_str());
    rapidjson::Document document;
    document.Parse<0>(responseStr.c_str());
    CCASSERT(!document.HasParseError(), "Parsing to document failed");
    
    if(document.IsObject()){
        if(document.HasMember("code")){
            const rapidjson::Value& val_code = document["code"];
            int code = val_code.GetInt();
            if (code == 1) {
                if (0 != strlen(response->getHttpRequest()->getTag())){
                    std::string tag = response->getHttpRequest()->getTag();
                    if (tag == "searchuser") {
                        if (document.HasMember("content")) {
                            const rapidjson::Value& val_content = document["content"];
                            
                            Global::getInstance()->user_data = {0};
                            Global::getInstance()->parseUserData(val_content, &Global::getInstance()->user_data);
                            this->showUserInfo();
                        }
                        
                    }
                    else if (tag == "createRoom"){
                        RoomItem* room = NULL;
                        char m_roomString[50] = {0};
                        if (roomTypeSelected == 1) {
                            room = xuanItems.at(roomIndexSelected);
                            sprintf(m_roomString, "钻石房间：≥%d\t底注%d", room->chipMin, room->perMin);
                        }
                        
                        char msg[200] = {0};
                        if (strlen(m_roomString) > 0) {
                            sprintf(msg, "%s\n请牢记您的密码：%s", m_roomString, document["content"].GetString());
                        }
                        else {
                            sprintf(msg, "请牢记您的密码：%s", document["content"].GetString());
                        }
                        
                        showMessageBox(msg, "创建房间成功");
                    }
                }
            }
            else {
                const rapidjson::Value& val_content = document["content"];
                const char* content = val_content.GetString();
                NoteTip::show(content);
            }
        }
    }
}

#pragma notification
void Hall::onNotification_Socket(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    switch (post->cmd) {
        case cmd_sendMessageToAll:{
            if (strlen(post->description) > 0) {
                NoteTip::show(post->description);
            }
        }
            break;
            
        case cmd_receiveAllMessage:{
            size_t count = Global::getInstance()->messageItems.size();
            if (count > 0) {
                MessageRef* messageItem = Global::getInstance()->messageItems.at(count - 1);
                
                auto visibleSize = Director::getInstance()->getVisibleSize();
                Vec2 origin = Director::getInstance()->getVisibleOrigin();
                
                msgLabel->stopAllActions();
                
                
                char m_string[200] = {0};
                sprintf(m_string, "%s说：%s", messageItem->fromUserNikeName, messageItem->message);
                msgLabel->setString(m_string);
                msgLabel->setPosition(Vec2(visibleSize.width + origin.x + msgLabel->getContentSize().width / 2, visibleSize.height + origin.y - 0.5 * msgLabel->getContentSize().height));
                
                float width = visibleSize.width + msgLabel->getContentSize().width;
                MoveBy* to = MoveBy::create(0.01 * width, Vec2(-width, 0));
                CallFunc* func1 = CallFunc::create([=]{
                    msgLabel->setPositionX(visibleSize.width + origin.x + msgLabel->getContentSize().width / 2);
                });
                msgLabel->runAction(RepeatForever::create(Sequence::create(to, func1, NULL)));
            }
            

        }
            break;
            
        case cmd_notifyRecoverBureau:{
            auto scene = OnlinePokerDesk::createScene();
            OnlinePokerDesk* layer = (OnlinePokerDesk* )(scene->getChildren().at(1));
            layer->roomType = RoomType_Recovery;
            strcpy(layer->roomTypeId, Global::getInstance()->table_data.roomTypeId);
            layer->jettonToEnter = 0;
            
            Director::getInstance()->pushScene(scene);
        }
            break;
            
        default:
            break;
    }
}

void Hall::onNotification_RefreshUserinfo(Ref* pSender){
    this->showUserInfo();
}
