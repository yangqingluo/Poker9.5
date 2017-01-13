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
#include "HelpScene.h"
#include "OnlinePokerDeskScene.h"

#define dialogTag      9527
#define sliderTag      9528
#define passwordBoxTag   9529

static int chipTypeCount = 5;

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
    
    
    int vip_level = Global::getInstance()->calculateVIPLevel(user_data.introCount);
    if (vip_level > 0) {
        sprintf(userInfoString, "ID:%s\nVIP:vip%d\n钻石:%d\n金币:%d\n银币:%d\n战斗次数:%d\n胜率:%s",user_data.account, vip_level , user_data.diamond, user_data.gold, user_data.silver, user_data.gameTimes, user_data.winningPercent);
    }
    else {
        sprintf(userInfoString, "ID:%s\nVIP:无\n钻石:%d\n金币:%d\n银币:%d\n战斗次数:%d\n胜率:%s",user_data.account, user_data.diamond, user_data.gold, user_data.silver, user_data.gameTimes, user_data.winningPercent);
    }
    
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
    char goldRoomID[5][33] = {"ca22bf326f78469ab3f387f9625b43c4","11d01846ca6e4449ad1809426ff33b6f","9310fedae22b45a59d4ef9768a7bbeec","bdfe95b277e84ce5bfe2ce8123558bc8","f869fe26232b464c9159a87b0ee236a0"};
    char vipRoomID[5][33] = {"de8b5d247b02491e88b011f33df3ea65","8b36978ce93a4dd485ffd61e5405499c","7299037e3e9e4a44b843e2f2110dd00d","75788e6f3f34450180a10c59d10a28fd","8494f5ea601d4427b17ce9e2a0ab1112"};
    char diamondRoomID[5][33] = {"93e9ff7482dc4be4a6100ef61a103adb","281c8761602d41a8b91ed3ac3fabcbc5","cd027cf993434e22b0f908d3f1f51192","e867c097effb431f92d934fc66c997d5","da531815c8c54317b3db887d84c9952a"};
    
    int chip[2][5] = {{500, 1000,3000,5000,10000},{10, 20,50,100,200}};
    
    
    char goldTitle[5][20] = {"新手房", "初级房","普通房","中级房","高级房"};
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
        item->type = RoomType_VIP;
        memcpy(item->typeID, vipRoomID[i], roomIDLength);
        if (i < chipTypeCount) {
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
        item->chipMin = chip[0][0];
        item->perMin = chip[1][0];
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
    
    userinfoLabel = Label::create("", "", 7);
    userinfoLabel->setTextColor(Color4B(0xe0, 0xe0, 0xe0, 0xff));
    userinfoLabel->setPosition(0.55 * userInfoSprite->getContentSize().width, 0.25 * userInfoSprite->getContentSize().height);
    userinfoLabel->setDimensions(0.95 * userInfoSprite->getContentSize().width, 0.5 * userInfoSprite->getContentSize().height);
    userInfoSprite->addChild(userinfoLabel);
    
    
    auto btn_refresh_UserInfo = Button::create("images/btn_fresh.png");
    btn_refresh_UserInfo->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_refresh_UserInfo->setContentSize(Size(0.3 * userInfoSprite->getContentSize().width, 0.3 * userInfoSprite->getContentSize().width));
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
    
    auto noteListSprite = QLImageSprite::create("images/note_list_bg.png", Size(1.0 * visibleSize.width, userInfoSprite->getBoundingBox().getMinY() - origin.y));
    noteListSprite->setPosition(origin.x + noteListSprite->getContentSize().width / 2, origin.y + noteListSprite->getContentSize().height / 2);
    this->addChild(noteListSprite, 2);
    
    noteListCellHeight = noteListSprite->getBoundingBox().size.height * 0.5;
    noteListTableView = TableView::create(this, Size(noteListSprite->getBoundingBox().size.width * 0.96,  noteListCellHeight));
    noteListTableView->setPosition(noteListSprite->getBoundingBox().size.width * 0.02, 0.0 * noteListSprite->getBoundingBox().size.height);
    noteListTableView->setDirection(TableView::Direction::HORIZONTAL);
    noteListTableView->setDelegate(this);
    noteListSprite->addChild(noteListTableView);
    
    noteListTableView->reloadData();
    
    auto inputBox = ui::EditBox::create(Size(0.8 * noteListSprite->getContentSize().width, MIN(0.3 * noteListSprite->getContentSize().height, 32)), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
    inputBox->setPosition(Vec2(noteListTableView->getPositionX() + 0.5 * inputBox->getContentSize().width, 0.65 * noteListSprite->getContentSize().height));
    noteListSprite->addChild(inputBox);
    
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
    
    inputBox->setPlaceHolder("请输入喇叭的内容");
    inputBox->setDelegate(this);
    msgBox = inputBox;
    
    auto btn_send = Button::create("images/chat_btn_send.png");
    btn_send->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_send->setTitleText("发送");
    btn_send->setTitleFontSize(12);
    btn_send->setTitleColor(Color3B::BLACK);
    btn_send->setContentSize(Size(MIN(2 * inputBox->getContentSize().height, 0.15 * noteListSprite->getContentSize().width), inputBox->getContentSize().height));
    btn_send->setPosition(Vec2(inputBox->getBoundingBox().getMaxX() + 0.6 * btn_send->getContentSize().width, inputBox->getPositionY()));
    btn_send->setTag(0);
    btn_send->addTouchEventListener(CC_CALLBACK_2(Hall::touchEvent, this));
    noteListSprite->addChild(btn_send);
    
    msgLabel = Label::createWithTTF("","fonts/STKaiti.ttf",16);
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
                    if (strlen(msgBox->getText()) > 0) {
                        auto visibleSize = Director::getInstance()->getVisibleSize();
                        Vec2 origin = Director::getInstance()->getVisibleOrigin();
                        
                        msgLabel->stopAllActions();
                        msgLabel->setString(msgBox->getText());
                        msgLabel->setPosition(Vec2(visibleSize.width + origin.x + msgLabel->getContentSize().width / 2, visibleSize.height + origin.y - 0.5 * msgLabel->getContentSize().height));
                        
                        msgBox->setText("");
                        
                        float width = visibleSize.width + msgLabel->getContentSize().width;
                        MoveBy* to = MoveBy::create(0.01 * width, Vec2(-width, 0));
                        CallFunc* func1 = CallFunc::create([=]{
                            msgLabel->setPositionX(visibleSize.width + origin.x + msgLabel->getContentSize().width / 2);
                        });
                        msgLabel->runAction(RepeatForever::create(Sequence::create(to, func1, NULL)));
                    }
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

void Hall::sliderChangerCallBack(Ref* pSender, Control::EventType type){
    ControlSlider* slider = (ControlSlider* )pSender;
    switch (slider->getTag()) {
        case sliderTag:{
            jettonToEnter = (int)slider->getValue();
            PopAlertDialog* popup = (PopAlertDialog *)this->getChildByTag(dialogTag);
            
            char content[200];
            switch (roomTypeSelected) {
                case 0:{
                    sprintf(content, "请设置带入的金币数目:%d", jettonToEnter);
                }
                    break;
                    
                case 1:{
                    sprintf(content, "请设置带入的金币数目:%d", jettonToEnter);
                }
                    break;
                    
                case 2:{
                    sprintf(content, "请设置带入的钻石数目:%d", jettonToEnter);
                }
                    break;
                    
                case 3:{
                    sprintf(content, "请设置带入的银币数目:%d", jettonToEnter);
                }
                    break;
                    
                default:
                    break;
            }
            
            
            popup->setContentTextShowed(content);
        }
            break;
            
        default:
            break;
    }
}
#pragma alert
void Hall::showSettingChip(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    PopAlertDialog* popup = NULL;
    
    if ((roomTypeSelected == 1 && roomIndexSelected < diItems.size() - 1) || (roomTypeSelected == 2 && roomIndexSelected < xuanItems.size() - 1)) {
        popup = PopAlertDialog::create("images/bg_dialog_empty.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
        popup->setTitle("创建房间");
    }
    else {
        popup = PopAlertDialog::create("images/set_chip_bg.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
        popup->setTitle("");
    }
    
    popup->setTag(dialogTag);
    popup->setCallBackFunc(this,callfuncN_selector(Hall::popButtonCallback));
    
    auto contentPosition = Vec2(0.1 * popup->m_dialogContentSize.width, (120.0 / 190.0) * popup->m_dialogContentSize.height);
    popup->setContentText("", 12, contentPosition.x, contentPosition.y);
    
    auto myslider = ControlSlider::create("images/slider_jd.png", "images/slider_bg.png", "images/slider_hk.png");
    myslider->setPosition(popup->getContentSize().width / 2, 0.50 * popup->getContentSize().height);
    myslider->setMinimumValue(0);
    myslider->setTag(sliderTag);
    
    bool canEnter = true;
    bool passwordEnter = false;
    bool showSlider = true;
    RoomItem* room = NULL;
    switch (roomTypeSelected) {
        case 0:{
            room = tianItems.at(roomIndexSelected);
            canEnter = Global::getInstance()->user_data.gold >= room->chipMin;
            if (!canEnter) {
                popup->setContentTextShowed("您的金币不足");
            }
            else {
                myslider->setMaximumValue(Global::getInstance()->user_data.gold);
            }
            
        }
            break;
            
        case 1:{
            room = diItems.at(roomIndexSelected);
            if (roomIndexSelected + 1 == diItems.size()) {
                passwordEnter = true;
                myslider->setMaximumValue(Global::getInstance()->user_data.gold);
            }
            else {
                canEnter = Global::getInstance()->user_data.gold >= room->chipMin;
                if (!canEnter) {
                    popup->setContentTextShowed("您的金币不足");
                }
                else {
                    char m_creat_string[200] = {0};
                    sprintf(m_creat_string, "创建%d金币的房间", room->chipMin);
                    popup->setContentTextShowed(m_creat_string);
                    
                    showSlider = false;
                }
            }
        }
            break;
            
        case 2:{
            room = xuanItems.at(roomIndexSelected);
            if (roomIndexSelected + 1 == xuanItems.size()) {
                passwordEnter = true;
                myslider->setMaximumValue(Global::getInstance()->user_data.diamond);
            }
            else {
                canEnter = Global::getInstance()->user_data.diamond >= room->chipMin;
                if (!canEnter) {
                    popup->setContentTextShowed("您的钻石不足");
                }
                else {
                    char m_creat_string[200] = {0};
                    sprintf(m_creat_string, "创建%d钻石的房间", room->chipMin);
                    popup->setContentTextShowed(m_creat_string);
                    
                    showSlider = false;
                }
            }
        }
            break;
            
        case 3:{
            room = huangItems.at(roomIndexSelected);
            canEnter = Global::getInstance()->user_data.silver >= room->chipMin;
            if (!canEnter) {
                popup->setContentTextShowed("您的银币不足");
            }
            else {
                myslider->setMaximumValue(Global::getInstance()->user_data.silver);
                
            }
        }
            break;
            
        default:
            break;
    }
    
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "",1);
    if (canEnter) {
        popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "",0);
    }
    
    this->addChild(popup, 2);
    
    if (canEnter) {
        if (showSlider) {
            popup->addChild(myslider);
            myslider->addTargetWithActionForControlEvents(this, cccontrol_selector(Hall::sliderChangerCallBack), Control::EventType::VALUE_CHANGED);
        }
        
        if (room != NULL) {
            myslider->setMinimumAllowedValue(room->chipMin);
            myslider->setValue(room->chipMin);
        }
    }
    
    if (passwordEnter) {
        auto inputBox = ui::EditBox::create(Size(0.4 * popup->m_dialogContentSize.width, MIN(0.15 * popup->m_dialogContentSize.height, 32)), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
        inputBox->setPosition(Vec2(popup->getContentSize().width / 2, 0.40 * popup->getContentSize().height));
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
        inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
        inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
        inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
        
        inputBox->setPlaceHolder("房间密码");
        inputBox->setMaxLength(length_room_password);
    }
}

void Hall::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        PopAlertDialog* popup = (PopAlertDialog *)this->getChildByTag(dialogTag);
        RoomItem* room = NULL;
        switch (roomTypeSelected) {
            case 0:{
                room = tianItems.at(roomIndexSelected);
                
                auto scene = OnlinePokerDesk::createScene();
                OnlinePokerDesk* layer = (OnlinePokerDesk* )(scene->getChildren().at(1));
                layer->roomType = room->type;
                strcpy(layer->roomTypeId, room->typeID);
                layer->jettonToEnter = jettonToEnter;
                
                Director::getInstance()->pushScene(scene);

            }
                break;
                
            case 1:{
                if (roomIndexSelected + 1 == diItems.size()) {
                    if (popup) {
                        cocos2d::ui::EditBox* box = (cocos2d::ui::EditBox* )popup->getChildByTag(passwordBoxTag);
                        if (box) {
                            if (strlen(box->getText()) != length_room_password) {
                                NoteTip::show("密码输入有误");
                            }
                            else {
                                auto scene = OnlinePokerDesk::createScene();
                                OnlinePokerDesk* layer = (OnlinePokerDesk* )(scene->getChildren().at(1));
                                layer->roomType = RoomType_VIP;
                                layer->jettonToEnter = jettonToEnter;
                                strcpy(layer->roomPassword, box->getText());
                                
                                Director::getInstance()->pushScene(scene);
                            }
                        }
                    }
                    
                    
                }
                else {
                    int vip_level = Global::getInstance()->calculateVIPLevel(Global::getInstance()->user_data.introCount);
                    if (vip_level > 0) {
                        room = diItems.at(roomIndexSelected);
                        
                        if (room->chipMin > Global::getInstance()->user_data.gold) {
                            NoteTip::show("金币不足，不能创建");
                        }
                        else {
                            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                            onHttpRequest_CreateRoom(room->typeID);
                        }
                    }
                    else {
                        NoteTip::show("抱歉，您还不是VIP会员");
                    }
                }
            }
                break;
                
            case 2:{
                if (roomIndexSelected + 1 == xuanItems.size()) {
                    if (popup) {
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
                
            case 3:{
                room = huangItems.at(roomIndexSelected);
                
                auto scene = PokerDesk::createScene();
                PokerDesk* layer = (PokerDesk* )(scene->getChildren().at(1));
                layer->roomType = room->type;
                layer->perMin = room->perMin;
                layer->chipMin = room->chipMin;
                layer->gamePlayer->infoConfig(Global::getInstance()->user_data.nikename, "images/default_head.png", jettonToEnter);
                
//                TransitionScene* ts = TransitionMoveInR::create(0.2, scene);
                Director::getInstance()->pushScene(scene);
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
                auto scene = HelpScene::createScene();
                Director::getInstance()->pushScene(scene);
            }
                break;
                
            case 4:{
                auto scene = SettingScene::createScene();
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
    request->setUrl("http://115.28.109.174:8181/game/room/createRoom");
    
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
    request->setUrl("http://115.28.109.174:8181/game/user/userinfo");
    
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
                        char msg[200] = {0};
                        sprintf(msg, "请牢记您的密码：%s", document["content"].GetString());
                        MessageBox(msg, "创建房间成功");
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
//    PostRef* post = (PostRef* )pSender;
//    switch (post->cmd) {
//        case cmd_enterRoom:{
//            if (m_pMessage != NULL) {
//                m_pMessage->hidden();
//            }
//            
//            auto scene = OnlinePokerDesk::createScene();
//            PokerDesk* layer = (PokerDesk* )(scene->getChildren().at(1));
//            layer->gamePlayer->infoConfig("阿罗", "images/default_head.png", 3000);
//            
//            TransitionScene* ts = TransitionMoveInR::create(0.2, scene);
//            Director::getInstance()->pushScene(ts);
//        }
//            break;
//            
//        default:
//            break;
//    }
}

void Hall::onNotification_RefreshUserinfo(Ref* pSender){
    this->showUserInfo();
}
