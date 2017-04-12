//
//  OnlinePokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "OnlinePokerDeskScene.h"
#include "PopAlertDialog.h"

#define dialogTag      9527

#define inputMoneyBoxTag   9529

#define pokerMoveTime 0.5
const float jetton_height_scale = 0.08;

OnlinePokerDesk::OnlinePokerDesk():m_deskState(0),m_IndexSend(0),m_IndexStart(0),m_isStarted(false),m_isSendSingle(true),m_isSendSet(true),stabberPlayer(NULL),dealerPlayer(NULL),m_pMessage(NULL){
    pcPlayer = new Player();
    pcPlayer->retain();
    pcPlayer->infoConfig("电脑", "images/p2.png", 3000);
    
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(OnlinePokerDesk::onNotification_Socket), kNotification_Socket, NULL);
}

OnlinePokerDesk::~OnlinePokerDesk(){
    NotificationCenter::getInstance()->removeAllObservers(this);
    Global::getInstance()->table_data = {0};
    Global::getInstance()->clearPlayerList();
    
    CC_SAFE_RELEASE(pcPlayer);
//    CC_SAFE_RELEASE(stabberPlayer);
//    CC_SAFE_RELEASE(dealerPlayer);
}

Scene* OnlinePokerDesk::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = OnlinePokerDesk::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool OnlinePokerDesk::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    judgementPosition = Vec2(origin.x + 0.3 * visibleSize.width, origin.y + 0.95 * visibleSize.height);
    
    auto sprite = Sprite::create("images/pokerDesk_bg.jpg");
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    sprite->setScaleX(visibleSize.width / spx); //设置精灵宽度缩放比例
    sprite->setScaleY(visibleSize.height / spy);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto btn_BackItem = MenuItemImage::create(
                                              "images/window_close@2x.png",
                                              "images/window_close@2x.png",
                                              CC_CALLBACK_1(OnlinePokerDesk::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    btn_PrepareItem = MenuItemImage::create(
                                              "images/btn_prepare.png",
                                              "images/btn_prepare.png",
                                              CC_CALLBACK_1(OnlinePokerDesk::buttonCallback, this, 1));
    
    btn_PrepareItem->setScale(0.09 * visibleSize.height / btn_PrepareItem->getContentSize().height);
    btn_PrepareItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 0.25 * visibleSize.height));
    
    btn_PrepareItem->setVisible(false);
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, btn_PrepareItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto lotus = Sprite::create("images/lotus.png");
    lotus->setScale(150.0 / 640.0 * visibleSize.height / lotus->getTextureRect().getMaxY());
    lotus->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - lotus->getBoundingBox().size.height / 2);
    this->addChild(lotus);
    
    upright_sprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.15 * visibleSize.width, 0.20 * visibleSize.width));
    upright_sprite->setPosition(origin.x + visibleSize.width - upright_sprite->getContentSize().width * 0.6, origin.y + visibleSize.height - upright_sprite->getContentSize().height * 0.6);
    this->addChild(upright_sprite);
    
    dealerHead = Sprite::create("images/default_head.png");
    dealerHead->setScale(0.4 * upright_sprite->getContentSize().width / dealerHead->getContentSize().width);
    dealerHead->setPosition(0.5 * upright_sprite->getContentSize().width, 1.0 * upright_sprite->getContentSize().height - 0.5 * dealerHead->getBoundingBox().size.height);
    upright_sprite->addChild(dealerHead);
    dealerHead->setVisible(false);
    
    btn_BeBankerItem = MenuItemImage::create(
                                             "images/top_btn_banker.png",
                                             "images/top_btn_banker_highlight.png",
                                             CC_CALLBACK_1(OnlinePokerDesk::buttonCallback, this, 3));
    btn_BeBankerItem->setScale(0.8 * upright_sprite->getContentSize().width / btn_BeBankerItem->getContentSize().width);
    btn_BeBankerItem->setPosition(0.5 * upright_sprite->getContentSize().width, 0.75 * upright_sprite->getContentSize().height);
    btn_BeBankerItem->setVisible(false);
    
    countLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 8);
    countLabel->setColor(Color3B::BLACK);
//    countLabel->setHorizontalAlignment(TextHAlignment::LEFT);
//    countLabel->setVerticalAlignment(TextVAlignment::CENTER);
    countLabel->setPosition(0.5 * upright_sprite->getContentSize().width, 0.3 * upright_sprite->getContentSize().height);
    upright_sprite->addChild(countLabel);
    
    auto btn_playerList = MenuItemFont::create("点击查看玩家列表", CC_CALLBACK_1(OnlinePokerDesk::buttonCallback, this, 4));
    btn_playerList->setFontSizeObj(8);
    btn_playerList->setColor(Color3B::BLACK);
    btn_playerList->setPosition(0.5 * upright_sprite->getContentSize().width, 0.6 * btn_playerList->getContentSize().height);
    
    auto menu_ur = Menu::create(btn_BeBankerItem, btn_playerList, NULL);
    menu_ur->setPosition(Vec2::ZERO);
    upright_sprite->addChild(menu_ur, 0);
    
    bottom_sprite = QLImageSprite::create("images/desk_bottom_bg.png", Size(visibleSize.width, 0.12 * visibleSize.height));
    bottom_sprite->setPosition(origin.x + visibleSize.width / 2, origin.y + bottom_sprite->getContentSize().height / 2);
    this->addChild(bottom_sprite);
    
    gamePlayerInfoLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 8);
    gamePlayerInfoLabel->setTextColor(Color4B::WHITE);
    gamePlayerInfoLabel->setPosition(0.85 * bottom_sprite->getContentSize().width, 0.5 * bottom_sprite->getContentSize().height);
    bottom_sprite->addChild(gamePlayerInfoLabel);
    
    btn_addJetton = Button::create("images/btn_add.png","images/btn_add.png");
//    btn_addJetton->setScale9Enabled(true);//打开scale9 可以拉伸图片
//    btn_addJetton->setContentSize(Size(0.8 * bottom_sprite->getContentSize().height, 0.8 * bottom_sprite->getContentSize().height));
    btn_addJetton->setScale(0.8 * bottom_sprite->getContentSize().height / btn_addJetton->getContentSize().height);
    btn_addJetton->setPosition(Vec2(bottom_sprite->getContentSize().width - 0.6 * btn_addJetton->getBoundingBox().size.width, 0.5 * bottom_sprite->getContentSize().height));
    btn_addJetton->addTouchEventListener(CC_CALLBACK_2(OnlinePokerDesk::touchEvent, this));
    btn_addJetton->setTag(10);
    btn_addJetton->setVisible(false);
    bottom_sprite->addChild(btn_addJetton);
    
    int betJettonArray[6] = {10,50,100,500,1000,5000};
    betLimiter = BetLimiter::create(betJettonArray, 6, Size(0.8 * bottom_sprite->getContentSize().width, 1.0 * bottom_sprite->getContentSize().height), BetType_Addition);
    betLimiter->setPosition(0.5 * bottom_sprite->getContentSize().width - 0.5 * betLimiter->getContentSize().width, 0.5 * bottom_sprite->getContentSize().height - 0.5 * betLimiter->getContentSize().height);
    betLimiter->setVisible(false);
    bottom_sprite->addChild(betLimiter);
    
    showTimer = GameTimer::createTimer();
    showTimer->showTag = 0;
    showTimer->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.74 * visibleSize.height);
    showTimer->setCallBackFunc(this,callfuncN_selector(OnlinePokerDesk::showTimerDoneCallback));
    this->addChild(showTimer);
    
    char imageName[4][100] = {"","images/chair_bet_1_bg.png","images/chair_bet_2_bg.png","images/chair_bet_3_bg.png"};
    float scaleArray[4][2] = {{0.5,0.65},{0.24,0.4},{0.5,0.4},{0.76,0.4}};
    for (int i = 0; i < 4; i++) {
        PokerChair* chair = this->createChair(imageName[i], scaleArray[i][0], scaleArray[i][1], i);
        m_arrChairs.pushBack(chair);
        this->addChild(chair, 0);
    }
    
    playerList_sprite = QLImageSprite::create("images/window_upright_bg.png", Size(upright_sprite->getContentSize().width, 0.9 * (upright_sprite->getBoundingBox().getMinY() - bottom_sprite->getBoundingBox().getMaxY())));
    playerList_sprite->setPosition(upright_sprite->getPositionX(), 0.5 * (upright_sprite->getBoundingBox().getMinY() + bottom_sprite->getBoundingBox().getMaxY()));
    playerList_sprite->setVisible(false);
    this->addChild(playerList_sprite);
    
    playerListTableView = TableView::create(this, Size(playerList_sprite->getContentSize().width,  playerList_sprite->getContentSize().height));
    playerListTableView->setPosition(0 , 0);
    playerListTableView->setDirection(TableView::Direction::VERTICAL);
    playerListTableView->setDelegate(this);
    playerList_sprite->addChild(playerListTableView);
    
    srand((unsigned)time(NULL));//初始化随机种子
    
    return true;
}

void OnlinePokerDesk::updateDeskState(DeskState state){
    if (m_deskState == state) {
        return;
    }
    
    switch (m_deskState) {
        case DeskState_Settle:{
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at((i + m_IndexStart) % m_arrChairs.size());
                chair->clearChair();
            }
            if (judgementPokerIndex < m_arrPokers.size()) {
                PokerSprite* poker = m_arrPokers.at(judgementPokerIndex);
                poker->setVisible(false);
            }
        }
            break;
            
        case DeskState_SendPoker:{
            if (!m_isSendSet) {
                //发牌动画未完成
                m_isSendSet = true;
                if (m_IndexSend % 9 != 0) {
                    sendPokerWithoutAnimation();
                }
            }
        }
            break;
            
        case DeskState_ChooseStabber:{
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chairBuffer = m_arrChairs.at(i);
                chairBuffer->showBeStabber(false);
            }
        }
            break;
            
        default:
            break;
    }
    
    switch (state) {
        case DeskState_Waiting:{
            m_isStarted = false;
        }
            break;
            
        case DeskState_Start:{
            //牌局开始
            m_isStarted = true;
            sprintf(showTimer->prefixString,"开始！");
            showTimer->showPrefix();
        }
            break;
            
        default:
            break;
    }
    
    m_deskState = state;
    if (m_deskState != DeskState_Waiting) {
        btn_PrepareItem->setVisible(false);
    }
    
    if (m_deskState != DeskState_ChooseDealer) {
        btn_BeBankerItem->setVisible(false);
    }
    
    
}

void OnlinePokerDesk::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            bool can_leave = false;
            
            switch (m_deskState) {
                case DeskState_Default:
                case DeskState_Prepared:
                case DeskState_Waiting:{
                    can_leave = !m_isStarted;
                }
                    break;
                    
                default:{
                    bool remainZero = false;
                    for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                        PlayerData player_data = Global::getInstance()->playerList[i];
                        if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
                            remainZero = (player_data.remainCap == 0);
                            break;
                        }
                    }
                    if (!Global::getInstance()->isDealer && remainZero) {
                        can_leave = true;
                    }
                }
                    break;
            }
            
            if (can_leave) {
                m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
                Global::getInstance()->sendLeaveRoom();
            }
            else {
                NoteTip::show("当前阶段不能退出");
            }
        }
            break;
            
        case 1:{
            //准备
            if (m_deskState == DeskState_Waiting) {
                bool canPrepare = true;
                if (Global::getInstance()->playerListCount > 0) {
                    for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                        PlayerData player_data = Global::getInstance()->playerList[i];
                        if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
                            canPrepare = (player_data.remainCap >= 3 * Global::getInstance()->table_data.minPerStack);
                            break;
                        }
                    }
                }
                
                if (canPrepare) {
                    m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
                    Global::getInstance()->sendPlayerReady();
                }
                else {
                    NoteTip::show("本金不足，不能准备");
                }
            }
            
        }
            break;
            
        case 3:{
            //抢庄
            if (m_deskState == DeskState_ChooseDealer) {
                m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
                Global::getInstance()->sendApplyOwner();
            }
        }
            break;
            
        case 4:{
            //玩家列表
            playerList_sprite->setVisible(!playerList_sprite->isVisible());
        }
            break;
            
        default:
            break;
    }
}

void OnlinePokerDesk::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 10:{
                    if (Global::getInstance()->isDealer) {
                        NoteTip::show("庄家不能补充本金");
                    }
                    else {
                        this->showSettingChip();
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

void OnlinePokerDesk::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        PopAlertDialog* popup = (PopAlertDialog *)this->getChildByTag(dialogTag);
        if (popup) {
            cocos2d::ui::EditBox* editBox = (cocos2d::ui::EditBox* )popup->getChildByTag(inputMoneyBoxTag);
            if (editBox) {
                int jettonToAdd = atoi(editBox->getText());
                if (jettonToAdd <= 0) {
                    NoteTip::show("请输入正确的数目");
                }
                else if (Global::getInstance()->isDealer) {
                    NoteTip::show("庄家不能补充本金");
                }
                else {
                    Global::getInstance()->sendSupplyBit(jettonToAdd);
                }
            }
        }
    }
}

void OnlinePokerDesk::onEnter(){
    Layer::onEnter();
    
    this->showGamePlayerInfo();
    this->showDealerInfo();
    
    if (this->roomType == RoomType_Gold) {
        this->showMessageManager(true);
        Global::getInstance()->sendEnterRoom(roomTypeId, jettonToEnter);
    }
    else if (this->roomType == RoomType_VIP) {
        this->showMessageManager(true);
        Global::getInstance()->sendEnterRoomByPassword(this->roomPassword, jettonToEnter, 1);
    }
    else if (this->roomType == RoomType_Diamond) {
        this->showMessageManager(true);
        Global::getInstance()->sendEnterRoomByPassword(this->roomPassword, jettonToEnter, 2);
    }
    else if (this->roomType == RoomType_Recovery) {
//        Global::getInstance()->sendEnterRoom(roomTypeId, jettonToEnter);
    }
}

void OnlinePokerDesk::onExit(){
    Layer::onExit();
    unscheduleUpdate();
    
}

void OnlinePokerDesk::showMessageManager(bool isShow){
    if (isShow) {
        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
    }
    else {
        if (m_pMessage != NULL) {
            m_pMessage->hidden();
            m_pMessage = NULL;
        }
    }
}

void OnlinePokerDesk::showSettingChip(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PopAlertDialog* popup = PopAlertDialog::create("images/bg_dialog_empty.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
    popup->setTitle("补充本金");
    popup->setTag(dialogTag);
    popup->setCallBackFunc(this,callfuncN_selector(OnlinePokerDesk::popButtonCallback));
    
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "","",1);
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "","",0);
    
    this->addChild(popup, 60);
    
    auto inputBox = ui::EditBox::create(Size(0.4 * popup->m_dialogContentSize.width, MIN(0.15 * popup->m_dialogContentSize.height, 32)), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
    inputBox->setPosition(Vec2(popup->getContentSize().width / 2, 0.55 * popup->getContentSize().height));
    inputBox->setTag(inputMoneyBoxTag);
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
//    inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::SENSITIVE);
//    inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    
    inputBox->setPlaceHolder("补充本金的数目");
    inputBox->setMaxLength(length_room_capital);
    
}

void OnlinePokerDesk::goBackAction(){
    Director::getInstance()->popScene();
}

void OnlinePokerDesk::waitForPrepareAction(){
    if (!showTimer->getIsValid()) {
        m_IndexSend = 0;
        
        btn_PrepareItem->setVisible(true);
        
        sprintf(showTimer->prefixString,"请准备…");
        showTimer->start(Global::getInstance()->countDownInSecond);
    }
}

void OnlinePokerDesk::preparedAction(){
    updateDeskState(DeskState_Prepared);
    
    btn_PrepareItem->setVisible(false);
    
    sprintf(showTimer->prefixString,"等待开始…");
    showTimer->showPrefix();
}

void OnlinePokerDesk::waitForBetAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString, Global::getInstance()->isDealer ? "等待闲家下注..." : "设置筹码，选择过、天、坎下注");
        showTimer->start(Global::getInstance()->countDownInSecond);
        
        Global::getInstance()->playEffect_place(false);
    }
}

void OnlinePokerDesk::settleAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString,"结算");
        showTimer->start(Global::getInstance()->countDownInSecond);
        
        if (stabberPlayer == NULL) {
            PokerChair* chair0 = m_arrChairs.at(0);
            int accountPlayer = 0;
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at(i % m_arrChairs.size());
                chair->calculatePokerType();
                if (i > 0) {
                    chair->calculateSettlement(chair0);
                    chair->m_settlement.accounts = Global::getInstance()->table_data.round.settleList[i].count;
                    chair->m_settlement.multiple = Global::getInstance()->table_data.round.settleList[i].times;
                    chair->m_settlement.winned = (chair->m_settlement.accounts > 0);
                    chair->showSettlement();
                    
                    accountPlayer += chair->m_settlement.accounts;
                }
                
                
                
                chair->showPokerType();
                
                for (int i = 0; i < chair->pokerArray.size(); i++) {
                    PokerSprite* poker = chair->pokerArray.at(i);
                    poker->showPokerAnimated(true, true, 0);
                }
            }
            
            for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                PlayerData player_data = Global::getInstance()->playerList[i];
                if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
                    player_data.remainCap += accountPlayer;
                }
            }

        }
        else {
            int accountDealer = 0;
            int accountPlayer = 0;
            PokerChair* chair0 = m_arrChairs.at(0);
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at(i % m_arrChairs.size());
                chair->calculatePokerType();
                if (i > 0) {
                    chair->calculateSettlementForStabber(chair0, dealerPlayer->getJettonCount());
                    chair->showSettlementForStabber();
                    
                    if (chair->m_Stabber->isVisible()) {
//                        if (stabberPlayer == gamePlayer) {
//                            accountDealer -= chair->m_settlement.accounts;
//                            accountPlayer += chair->m_settlement.accounts;
//                        }
                    }
                }
                
                chair->showPokerType();
                
                for (int i = 0; i < chair->pokerArray.size(); i++) {
                    PokerSprite* poker = chair->pokerArray.at(i);
                    poker->showPokerAnimated(true, true, 0);
                }
            }
            
            dealerPlayer->setJettonCount(dealerPlayer->getJettonCount() + accountDealer);
//            gamePlayer->setJettonCount(gamePlayer->getJettonCount() + accountPlayer);
        }
        
        showDealerInfo();
        showGamePlayerInfo();
    }
}

void OnlinePokerDesk::waitForChooseDealerAction(){
    if (!showTimer->getIsValid()) {
        btn_BeBankerItem->setVisible(true);
        
        sprintf(showTimer->prefixString,"抢庄");
        showTimer->start(Global::getInstance()->countDownInSecond);
    }
}

void OnlinePokerDesk::waitForChooseStabberAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString,"等待其他玩家抢刺");
        if (!Global::getInstance()->isDealer) {
            int playerIndex = -1;
            int dealerIndex = -1;
            
            for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                PlayerData player_data = Global::getInstance()->playerList[i];
                if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
                    playerIndex = i;
                }
                if (0 == strcmp(Global::getInstance()->table_data.bureau.bureauOwnerId, player_data.user.ID)) {
                    dealerIndex = i;
                }
            }
            
            if (playerIndex >= 0 && dealerIndex >=0) {
                PlayerData player = Global::getInstance()->playerList[playerIndex];
                PlayerData dealer = Global::getInstance()->playerList[dealerIndex];
                if (player.remainCap > dealer.remainCap) {
                    for (int i = 0; i < m_arrChairs.size(); i++) {
                        PokerChair* chair = m_arrChairs.at((i + m_IndexStart) % m_arrChairs.size());
                        chair->showBeStabber(true);
                    }
                    sprintf(showTimer->prefixString,"抢刺");
                }
            }
        }
        
        showTimer->start(Global::getInstance()->countDownInSecond);
    }
}

void OnlinePokerDesk::sendPokerAction(){
    m_isSendSet = false;
    m_isSendSingle = true;
    updateDeskState(DeskState_SendPoker);
    
    sprintf(showTimer->prefixString,"发牌");
    showTimer->start(Global::getInstance()->countDownInSecond);
}

void OnlinePokerDesk::showGamePlayerInfo(){
    this->playerListTableView->reloadData();
    
    char mString[200] = {0};
    
    if (strlen(Global::getInstance()->table_data.roomType) > 0) {
        sprintf(mString, "%s\n", Global::getInstance()->table_data.roomType);
    }
    else {
        sprintf(mString, "%s\n", "房间信息未知");
    }
    
    bool hasUser = false;
    for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
        PlayerData player_data = Global::getInstance()->playerList[i];
        if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
            sprintf(mString + strlen(mString),"玩家：%s\n本金：%d",player_data.user.nikename, player_data.remainCap);
            gamePlayerInfoLabel->setString(mString);
            hasUser = true;
        }
    }
    
    if (!hasUser) {
        sprintf(mString + strlen(mString),"玩家：%s\n本金：%d", Global::getInstance()->user_data.nikename, jettonToEnter);
    }
    
    if (Global::getInstance()->table_data.round.roundIndex > 0) {
        char m_indexString[10] = {0};
        sprintf(m_indexString, " 第%d把", Global::getInstance()->table_data.round.roundIndex);
        strcat(mString, m_indexString);
    }
    
    gamePlayerInfoLabel->setString(mString);
}
void OnlinePokerDesk::showDealerInfo(){
    char mString[100];
    
    bool hasDealer = false;
    if (strlen(Global::getInstance()->table_data.bureau.bureauOwnerId) > 0) {
        for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
            PlayerData player_data = Global::getInstance()->playerList[i];
            if (0 == strcmp(Global::getInstance()->table_data.bureau.bureauOwnerId, player_data.user.ID)) {
                hasDealer = true;
                sprintf(mString,"庄家：%s\n本金：%d\n玩家总数：%d\n",player_data.user.nikename, player_data.remainCap, Global::getInstance()->playerListCount);
            }
        }
        
    }
    
    if (!hasDealer) {
        sprintf(mString,"庄家：无\n玩家总数：%d\n", Global::getInstance()->playerListCount);
    }
    
    countLabel->setString(mString);
    playerListTableView->reloadData();
}

void OnlinePokerDesk::resetShowDealerInfo(){
    showDealerInfo();
//            betLimiter->minValue = Global::getInstance()->table_data.minPerStack;
    betLimiter->reset();
    betLimiter->setVisible(!Global::getInstance()->isDealer);
}

void OnlinePokerDesk::showTimerDoneCallback(Node* pNode){
    switch (m_deskState) {
            
        default:{
            updateDeskState(DeskState_Default);
        }
            break;
    }
}


void OnlinePokerDesk::update(float delta){
    switch (m_deskState) {
        case DeskState_Waiting:{
            waitForPrepareAction();
        }
            break;
            
        case DeskState_Prepared:{
            
        }
            break;
            
        case DeskState_ChooseDealer:{
            waitForChooseDealerAction();
        }
            break;
            
        case DeskState_ChooseStabber:{
            waitForChooseStabberAction();
        }
            break;
            
        case DeskState_SendPoker:{
            if (m_isSendSet) {
                
            }
            else {
                sendPoker();
            }
        }
            break;
            
        case DeskState_Bet:{
            waitForBetAction();
        }
            break;
            
        case DeskState_Settle:{
            settleAction();
        }
            break;
            
        default:
            break;
    }
}

#pragma chair
void OnlinePokerDesk::touchedChairCallback(Node* pSender, void* pTarget){
    if (Global::getInstance()->isDealer) {
        return;
    }
    
    PokerChair* chair = (PokerChair* )pSender;
    Node* node = (Node* )pTarget;
    switch (node->getTag()) {
        case 10:{
            if (m_deskState == DeskState_Bet) {
                
                for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                    PlayerData player_data = Global::getInstance()->playerList[i];
                    if (0 == strcmp(Global::getInstance()->user_data.account, player_data.user.account)) {
                        
                        int totalBet = 0;
                        for (int j = 1; j < m_arrChairs.size(); j++) {
                            PokerChair* chairBuffer = m_arrChairs.at(j);
                            if (chairBuffer == chair) {
                                totalBet += betLimiter->getSelectedJettonValue();
                            }
                            else {
                                totalBet += chairBuffer->betPlayer;
                            }
                        }
                        
                        if (betLimiter->getSelectedJettonValue() < Global::getInstance()->table_data.minPerStack) {
                            char m_PerStack[100] = {0};
                            sprintf(m_PerStack, "本房间下注额不能小于%d",Global::getInstance()->table_data.minPerStack);
                            NoteTip::show(m_PerStack);
                        }
                        else if (totalBet * 3 > player_data.remainCap) {
                            NoteTip::show("下注不能超过本金1/3");
//                            betLimiter->reset();
                        }
                        else {
                            JettonSprite* sp = this->createjetton(betLimiter->getSelectedJettonValue());
                            chair->updateJetton(sp);
                            Global::getInstance()->playEffect_add_gold(false);
                            
                            //                m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
                            
                            
                            Global::getInstance()->sendBetStake(betLimiter->getSelectedJettonValue(), (int)m_arrChairs.getIndex(chair) + 1);
                        }
                        
                        break;
                    }
                }
                
            }
        }
            break;
            
        case 11:{
            if (m_deskState == DeskState_ChooseStabber) {
                //发送抢刺
                for (int i = 0; i < m_arrChairs.size(); i++) {
                    PokerChair* chairBuffer = m_arrChairs.at(i);
                    chairBuffer->showBeStabber(false);
                }

                Global::getInstance()->sendApplyStabber((int)m_arrChairs.getIndex(chair) + 1);
                
            }
        }
            break;
            
        default:
            break;
    }
    
}

PokerChair* OnlinePokerDesk::createChair(const char* backgroudImage, float xScale, float yScale, int index){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float pokerScale = 0.125;
    
    float radius = MIN(0.24 * visibleSize.width, 0.36 * visibleSize.height);
    auto chair = PokerChair::create(index == 0 ? NULL : backgroudImage, Size(radius, radius - pokerScale * visibleSize.height));
    chair->setContentSize(Size(radius, (index == 0 ? pokerScale * visibleSize.height : radius)));
    chair->setPosition(origin.x + xScale * visibleSize.width - chair->getContentSize().width / 2, origin.y + yScale * visibleSize.height  - chair->getContentSize().height / 2);
    chair->setPoint(Vec2(chair->getPosition().x + 0.5 * chair->getContentSize().width, chair->getPosition().y + 0.5 * pokerScale * visibleSize.height));
    if (index == 0) {
        chair->setIsBanker(true);
    }
    else{
        chair->setCanTouch(true);
        chair->setTouchCallBackFunc(this, callfuncND_selector(OnlinePokerDesk::touchedChairCallback));
    }
    
    return chair;
}

#pragma jetton
JettonSprite* OnlinePokerDesk::createjetton(int value){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    JettonSprite* sp = JettonSprite::create(value, Size(jetton_height_scale * visibleSize.height, jetton_height_scale * visibleSize.height));
    sp->isPlayer = true;
    return sp;
}

void OnlinePokerDesk::addJetton(PokerChair* chair,JettonSprite* jetton){
    
}

#pragma poker
PokerSprite* OnlinePokerDesk::createPoker(PokerColor color,PokerPoint point){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PokerSprite* pk = PokerSprite::create(color, point, Size(0.09375 * visibleSize.height, 0.125 * visibleSize.height));
    
    return pk;
}
bool OnlinePokerDesk::createPokers(){
    if (m_arrPokers.size() == 0) {
        //创建52个牌
        for (int i = PokerColor_Spade; i <= PokerColor_Diamond; ++i){
            for (int j = PokerPoint_Ace; j <= PokerPoint_King; ++j){
                PokerSprite* pk = createPoker((PokerColor)i, (PokerPoint)j);
                m_arrPokers.pushBack(pk);
                
                this->addChild(pk);
                pk->setCallBackFunc(this, callfuncN_selector(OnlinePokerDesk::turnedSinglePokerCallback));
            }
        }
        //创建小鬼
        PokerSprite* joker_junior = createPoker(PokerColor_JokerJunior, PokerPoint_Joker);
        m_arrPokers.pushBack(joker_junior);
        
        //创建大鬼
        PokerSprite* joker_senior = createPoker(PokerColor_JokerSenior, PokerPoint_Joker);
        m_arrPokers.pushBack(joker_senior);
        
        this->addChild(joker_junior);
        this->addChild(joker_senior);
        joker_junior->setCallBackFunc(this, callfuncN_selector(OnlinePokerDesk::turnedSinglePokerCallback));
        joker_senior->setCallBackFunc(this, callfuncN_selector(OnlinePokerDesk::turnedSinglePokerCallback));
    }
    
    return true;
}

void OnlinePokerDesk::updatePokerData(){
    if (m_arrPokers.size() >= 9 + m_IndexSend) {
        judgementPokerIndex = m_IndexSend;
        for (int i = 0; i < 9; i++) {
            PokerSprite *pk = m_arrPokers.at(m_IndexSend + i);
            if (i == 0) {
                this->updatePokerWithData(pk, Global::getInstance()->table_data.round.pokerJudgement);
                if (pk->getPoker_point() == PokerPoint_Joker) {
                    m_IndexStart = 0;
                }
                else{
                    m_IndexStart = pk->getPoker_point() - 1;
                }
            }
            else {
                int chair_index = ((i - 1) % m_arrChairs.size() + m_IndexStart) % m_arrChairs.size();
                
                PokerPair pair = Global::getInstance()->table_data.round.pokerSendedList[chair_index];
                PokerData card = pair.poker[(i - 1) / 4];
                
                this->updatePokerWithData(pk, card);
            }
        }
    }
}

void OnlinePokerDesk::updatePokerWithData(PokerSprite* poker, PokerData data){
    int color = data.color;
    int num = data.num;
    
    if (color == 0 && (num == 14 || num == 15)) {
        color = (num == 14) ? PokerColor_JokerJunior : PokerColor_JokerSenior;
        num = PokerPoint_Joker;
    }
    poker->setPoker_color((PokerColor)color);
    poker->setPoker_point((PokerPoint)num);
}

void OnlinePokerDesk::adjustPoker(int index){
    index = index % 6;
    
    m_IndexSend = index * 9;
    m_isSendSet = false;
    m_isSendSingle = true;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 position = Vec2(origin.x + 0.3 * visibleSize.width, origin.y + 0.8 * visibleSize.height);
    for (size_t i = m_arrPokers.size(); i > 0; --i) {
        PokerSprite* pk = m_arrPokers.at(i - 1);
        pk->setVisible((i - 1) >= index * 9);
        pk->showPokerAnimated(false, false, 0);
        pk->setPosition(position.x, position.y - (i - 1) * 0.005 * pk->getContentSize().height);
        this->reorderChild(pk, (int)m_arrPokers.size() - (int)i + 1);
    }
}

void OnlinePokerDesk::sendPoker(){
    if (m_IndexSend >= m_arrPokers.size()) {
        return;
    }
    
    int index = m_IndexSend % 9;
    if (index == 0 && m_isSendSingle) {
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        pk->showPokerAnimated(true, true, pokerMoveTime);
        
        m_isSendSingle = false;
        ++m_IndexSend;
    }
    else if (index > 0 && index <= 8 && m_isSendSingle) {
        int chair_index = ((index - 1) % m_arrChairs.size() + m_IndexStart) % m_arrChairs.size();
        PokerChair* chair = m_arrChairs.at(chair_index);
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        
        movePoker(chair, pk);
        
        m_isSendSingle = false;
        ++m_IndexSend;
    }
}

//发牌(没有动画)
void OnlinePokerDesk::sendPokerWithoutAnimation(){
    if (m_IndexSend >= m_arrPokers.size()) {
        return;
    }
    
    int countToSend = 9 - m_IndexSend % 9;
    for (int i= 0; i < countToSend; ++i) {
        int index = m_IndexSend % 9;
        if (index == 0) {
            PokerSprite *pk = m_arrPokers.at(m_IndexSend);
            pk->showPokerAnimated(true, false, pokerMoveTime);
            pk->setPosition(judgementPosition);
            ++m_IndexSend;
        }
        else if (index > 0 && index <= 8) {
            int chair_index = ((index - 1) % m_arrChairs.size() + m_IndexStart) % m_arrChairs.size();
            PokerChair* chair = m_arrChairs.at(chair_index);
            PokerSprite *pk = m_arrPokers.at(m_IndexSend);
            
            movePokerWithoutAnimation(chair, pk);
            ++m_IndexSend;
        }
    }
}

void OnlinePokerDesk::movePoker(PokerChair* chair,PokerSprite* poker){
    poker->chairIndex = m_arrChairs.getIndex(chair);
    chair->pokerArray.pushBack(poker);
    MoveTo* move = MoveTo::create(pokerMoveTime, chair->getPoint());
    RotateBy* rotate = RotateBy::create(pokerMoveTime, 360);
    CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(OnlinePokerDesk::sendedSinglePoker, this, chair));
    Sequence* sequence = Sequence::create(Spawn::create(rotate,move, NULL),func,NULL);
    poker->runAction(sequence);
}

//发牌移动(没有动画)
void OnlinePokerDesk::movePokerWithoutAnimation(PokerChair* chair,PokerSprite* poker){
    poker->chairIndex = m_arrChairs.getIndex(chair);
    chair->pokerArray.pushBack(poker);
    
    poker->setPosition(chair->getPoint());
    this->sendedSinglePoker(poker, chair);
}

void OnlinePokerDesk::sendedSinglePoker(Node* pSender, void* pData){
    Global::getInstance()->playEffect_sendcard(false);
    
    PokerChair* chair = (PokerChair* )pData;
    chair->updatePokerPosition();
    if (chair->pokerArray.size() > 0) {
        PokerSprite* pk0 = chair->pokerArray.at(0);
        this->reorderChild(pk0, 0);
    }
    m_isSendSingle = true;
    if (m_IndexSend % 9 == 0) {
        m_isSendSet = true;
    }
}

void OnlinePokerDesk::turnedSinglePokerCallback(Node* pSender){
    PokerSprite* poker = (PokerSprite* )pSender;
    
    if (m_arrPokers.getIndex(poker) % 9 == 0) {
        for (int i = 0; i < m_arrChairs.size(); i++) {
            PokerChair* chair = m_arrChairs.at(i);
            chair->setHighlighted(i == (m_IndexStart % m_arrChairs.size()));
        }
        
        if (m_deskState == DeskState_SendPoker) {
            MoveTo* move = MoveTo::create(pokerMoveTime, judgementPosition);
            Sequence* sequence = Sequence::create(move,NULL);
            poker->runAction(sequence);
        }
        else {
            poker->setPosition(judgementPosition);
        }
        
        m_isSendSingle = true;
    }
    
    
    else if (m_deskState == DeskState_Settle){
//        if (poker->chairIndex < m_arrChairs.size()) {
//            PokerChair* chair = m_arrChairs.at(poker->chairIndex);
//            if (chair->pokerArray.getIndex(poker) == chair->pokerArray.size() - 1) {
//                chair->showPokerType();
//                if (poker->chairIndex > 0) {
//                    chair->showSettlement();
//                }
//            }
//        }
    }
}

#pragma tableview
Size OnlinePokerDesk::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == playerListTableView) {
        return Size(playerList_sprite->getContentSize().width, 30);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* OnlinePokerDesk::tableCellAtIndex(TableView* table, ssize_t idx)
{
    if (table == playerListTableView) {
        TableViewCell* cell = table->dequeueCell();
        
        if(!cell)
        {
            cell = new TableViewCell();
            cell->autorelease();
            
            float listCellWidth = playerList_sprite->getContentSize().width;
            float height = 30;
            
            auto head = Sprite::create("images/default_head.png");
            head->setScale(MIN(0.3 * listCellWidth, height) / head->getContentSize().width);
            head->setPosition(0.5 * head->getBoundingBox().size.width, 0.5 * height);
            cell->addChild(head, 0 , 2);
            
            Label* titleLabel = Label::createWithTTF("昵称:", "fonts/STKaiti.ttf", 8);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.65 * listCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.7 * listCellWidth, height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            titleLabel->setTag(1);
            cell->addChild(titleLabel);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
//        Sprite* head = (Sprite* )cell->getChildByTag(2);
        
        char content[200];
        PlayerData player = Global::getInstance()->playerList[idx];
        sprintf(content, "%s\n%d", player.user.nikename, player.remainCap);
        
        label->setString(content);
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t OnlinePokerDesk::numberOfCellsInTableView(TableView* table)
{
    if (table == playerListTableView) {
        return Global::getInstance()->playerListCount;
    }
    
    return 0;
}

void OnlinePokerDesk::tableCellTouched(TableView* table, TableViewCell* cell){
    
}

#pragma schedule
void OnlinePokerDesk::stepIn(DeskState state){
    showTimer->stop();
    updateDeskState(state);
}

#pragma notification
void OnlinePokerDesk::onNotification_Socket(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    if (post->cmd >= 10000) {
        //大于10000则是恢复牌局 重置显示庄家信息
        m_isStarted = true;
        resetShowDealerInfo();
        NoteTip::show(this, "恢复牌局成功");
        
        //显示当前玩家下注情况
        for (int j = 1; j < m_arrChairs.size(); j++) {
            int stake = Global::getInstance()->table_data.round.betStakes[j];
            if (stake > 0) {
                PokerChair* chairBuffer = m_arrChairs.at(j);
                JettonSprite* sp = this->createjetton(stake);
                chairBuffer->updateJetton(sp);
            }
        }
        
        this->showMessageManager(false);
        this->showGamePlayerInfo();
        
        createPokers();
        this->adjustPoker(Global::getInstance()->table_data.round.roundIndex - 1);
        
        btn_addJetton->setVisible(true);
        scheduleUpdate();
    }
    
    switch (post->cmd) {
        case cmd_removePlayer:{
            this->showMessageManager(false);
            
            Director::getInstance()->popScene();
        }
            break;
        case cmd_beginCountDownBeforeBureau:{
            this->showMessageManager(false);
            
            this->showGamePlayerInfo();
            
            this->stepIn(DeskState_Waiting);
            
        }
            break;
        case cmd_synPlayerList:{
            this->showGamePlayerInfo();
            this->showDealerInfo();
        }
            break;
            
        case cmd_bureauOpen:{
            //开始牌局
            this->stepIn(DeskState_Start);
            adjustPoker(0);
        }
            break;
            
            //抢庄
        case cmd_countDownApplyBureauOwner:
            //恢复抢庄
        case cmd_applyBureauOwnerRecover:{
            this->stepIn(DeskState_ChooseDealer);
        }
            break;
        case cmd_selectedBureauOwner:{
            //选中庄家通知
            btn_BeBankerItem->setVisible(false);
            
            if (Global::getInstance()->isDealer) {
                NoteTip::show("恭喜，抢庄成功！");
            }
            resetShowDealerInfo();
        }
            break;
            
            //抢刺
        case cmd_countDownApplyStabber:
            //恢复抢刺
        case cmd_applyStabberRecover:{
            this->showGamePlayerInfo();
            this->stepIn(DeskState_ChooseStabber);
        }
            break;
            
        case cmd_notifyStabber:{
            //选中刺通知
            for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                PlayerData player_data = Global::getInstance()->playerList[i];
                if (0 == strcmp(Global::getInstance()->table_data.round.stabberId, player_data.user.ID)) {
                    for (int j = 0; j < m_arrChairs.size(); j++) {
                        PokerChair* chairBuffer = m_arrChairs.at(j);
                        chairBuffer->showBeStabber(false);
                        if (j + 1 == Global::getInstance()->table_data.round.stabberIndex) {
                            chairBuffer->showStabber("images/default_head.png", player_data.user.nikename, player_data.remainCap);
                        }
                    }
                    
                    break;
                }
            }
            
        }
            break;
            
            //开始下注倒计时
        case cmd_countDownBetStake:
            //恢复下注
        case cmd_betStakeRecover:{
            this->showGamePlayerInfo();
            this->stepIn(DeskState_Bet);
        }
            break;
            
            
        case cmd_leaveRoom:{
            this->showMessageManager(false);
            
            if (post->sub_cmd == 2) {
                if (strlen(post->description) > 0) {
                    NoteTip::show(post->description);
                }
                else {
                    NoteTip::show("当前阶段不能退出");
                }
            }
            else {
                Director::getInstance()->popScene();
            }
        }
            break;
            
        case cmd_enterRoom:
        case cmd_enterRoomByPassword:{
            this->showMessageManager(false);
            this->showGamePlayerInfo();
            
            createPokers();
            adjustPoker(0);
            
            switch (Global::getInstance()->table_data.code) {
                case state_enterRoom_success_wait:
                case state_enterRoom_success_prepare:
                case state_enterRoom_success_countdown:{
                    sprintf(showTimer->prefixString,"%s", Global::getInstance()->table_data.description);
                    showTimer->showPrefix();
                    btn_addJetton->setVisible(true);
                    
                    scheduleUpdate();
                    
                    if (Global::getInstance()->table_data.code == state_enterRoom_success_countdown) {
                        this->showMessageManager(false);
                        
                        this->showGamePlayerInfo();
                        
                        this->stepIn(DeskState_Waiting);
                    }
                }
                    break;

                case state_enterRoom_fail_no_money:
                case state_enterRoom_fail_full:
                case state_enterRoom_fail_password:
                case state_enterRoom_fail_type:{
                    NoteTip::show(Global::getInstance()->table_data.description);
                }
                    break;

                default:
                    break;
            }
            
            if (strlen(post->description) > 0) {
                NoteTip::show(post->description);
            }
        }
            break;
            
        case cmd_playerReady:{
            this->showMessageManager(false);
            this->preparedAction();
        }
            break;
            
        case cmd_applyOwner:{
            this->showMessageManager(false);
            
            btn_BeBankerItem->setVisible(false);
            NoteTip::show(post->description);
        }
            break;
            
        case cmd_betStake:{
            this->showMessageManager(false);
            
        }
            break;
            
        case cmd_supplyBit:{
            //补充本金
            this->showMessageManager(false);
            
            if (post->sub_cmd == 1000) {
                NoteTip::show("补充本金成功");
                this->showGamePlayerInfo();
                this->showDealerInfo();
            }
            else {
                if (strlen(post->description) > 0) {
                    NoteTip::show(post->description);
                }
                else {
                    NoteTip::show("补充本金失败");
                }
            }
        }
            break;
            
            //发牌
        case cmd_countDownSendCard:
            //恢复发牌
        case cmd_sendCardRecover:{
            this->showMessageManager(false);
//            if (post->cmd == cmd_sendCardRecover) {
                this->adjustPoker(Global::getInstance()->table_data.round.roundIndex - 1);
//            }
            this->updatePokerData();
            this->sendPokerAction();
        }
            break;
            
        case cmd_sysBetStake:{
            for (int j = 1; j < m_arrChairs.size(); j++) {
                PokerChair* chairBuffer = m_arrChairs.at(j);
                chairBuffer->updateTotal(Global::getInstance()->table_data.round.betList[j]);
            }
        }
            break;
        
            //结算
        case cmd_settle:
            //结算恢复
        case cmd_settleRecover:{
            this->showMessageManager(false);
            if (post->cmd == cmd_settleRecover) {
                //添加发牌显示
                this->adjustPoker(Global::getInstance()->table_data.round.roundIndex - 1);
                this->updatePokerData();
                this->sendPokerWithoutAnimation();
            }
            
            
            this->stepIn(DeskState_Settle);
        }
            break;
            
        case cmd_bureauOwnerOff:{
            //下庄通知
            showDealerInfo();
            
            NoteTip::show("庄家下庄");
        }
            break;
            
        default:
            break;
    }
}
