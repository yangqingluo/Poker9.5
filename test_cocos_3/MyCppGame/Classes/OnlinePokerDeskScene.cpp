//
//  OnlinePokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "OnlinePokerDeskScene.h"
#include "PopAlertDialog.h"

const float jetton_height_scale = 0.08;

OnlinePokerDesk::OnlinePokerDesk():chipMin(0),perMin(0),m_deskState(0),m_IndexSend(0),m_IndexStart(0),m_isSendSingle(true),m_isSendSet(true),stabberPlayer(NULL),dealerPlayer(NULL),m_pMessage(NULL){
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
    gamePlayerInfoLabel->setColor(Color3B::WHITE);
    gamePlayerInfoLabel->setPosition(0.9 * bottom_sprite->getContentSize().width, 0.5 * bottom_sprite->getContentSize().height);
    bottom_sprite->addChild(gamePlayerInfoLabel);
    
    int betJettonArray[9] = {10,20,50,100,200,500,1000,2000,5000};
    betLimiter = BetLimiter::create(betJettonArray, 9, Size(bottom_sprite->getContentSize().width, 0.8 * bottom_sprite->getContentSize().height));
    betLimiter->setPosition(2 * bottom_sprite->getContentSize().height, 0.5 * bottom_sprite->getContentSize().height - 0.5 * betLimiter->getContentSize().height);
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
            
        default:
            break;
    }
    
    switch (state) {
        case DeskState_Start:{
            //牌局开始
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
            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
            
            Global::getInstance()->sendLeaveRoom();
            
        }
            break;
            
        case 1:{
            //准备
            if (m_deskState == DeskState_Waiting) {
                bool canPrepare = true;
//                if (Global::getInstance()->playerListCount > 0) {
//                    for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
//                        PlayerData player_data = Global::getInstance()->playerList[i];
//                        if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
//                            canPrepare = (player_data.remainCap >= this->chipMin);
//                            break;
//                        }
//                    }
//                }
                
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

void OnlinePokerDesk::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        updateDeskState(DeskState_Waiting);
        scheduleUpdate();
    }
    else if(pNode->getTag() == 1) {
        this->goBackAction();
    }
    pNode->removeFromParent();
}

void OnlinePokerDesk::onEnter(){
    Layer::onEnter();
    
    this->showGamePlayerInfo();
    this->showDealerInfo();
    
    this->showMessageManager(true);
    if (this->roomType == RoomType_Gold) {
        Global::getInstance()->sendEnterRoom(roomTypeId, jettonToEnter);
    }
    else if (this->roomType == RoomType_VIP || this->roomType == RoomType_Diamond) {
        Global::getInstance()->sendEnterRoomByPassword(this->roomPassword, jettonToEnter);
    }
    else {
        this->showMessageManager(false);
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
    PopAlertDialog* popup = PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
    popup->setTitle("");
    popup->setContentText("请设置本金数目",12,50,130);
    popup->setCallBackFunc(this,callfuncN_selector(OnlinePokerDesk::popButtonCallback));
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "",0);
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "",1);
    
    this->addChild(popup);
    
    ControlSlider* myslider = ControlSlider::create("images/slider_bg.png","images/slider_jd.png","images/slider_hk.png");
    myslider->setPosition(popup->getContentSize().width / 2, popup->getContentSize().height * 0.45);
    myslider->setMaximumValue(100);
    myslider->setMinimumValue(0);
    popup->addChild(myslider);
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
//    createPokers();
//    reindexPoker();
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
                    chair->m_settlement.accounts = Global::getInstance()->settleList[i];
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

//void OnlinePokerDesk::chooseDealerAction(){
//    //单机模拟强制选择电脑为庄家
//    if (dealerPlayer == NULL) {
//        dealerPlayer = pcPlayer;
//        dealerHead->setTexture(dealerPlayer->headImage);
//        btn_BeBankerItem->setVisible(false);
//        dealerHead->setVisible(true);
//    }
//    
//    showDealerInfo();
//}

//void OnlinePokerDesk::dealerDidChoosedAction(){
//    if (dealerPlayer->getJettonCount() * 3 < dealerPlayer->getJettonInitial()) {
//        //庄家本金小于初始本金1/3，抢刺
//        updateDeskState(DeskState_ChooseStabber);
//    }
//    else {
//        //下注
//        updateDeskState(DeskState_Bet);
//    }
//}

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
                if (0 == strcmp(Global::getInstance()->table_data.bureauOwnerId, player_data.user.ID)) {
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

void OnlinePokerDesk::chooseStabberAction(int index){
//    stabberPlayer = gamePlayer;
//    for (int i = 0; i < m_arrChairs.size(); i++) {
//        PokerChair* chair = m_arrChairs.at(i);
//        chair->showBeStabber(false);
//        if (i == index) {
//            chair->showStabber(gamePlayer->headImage, gamePlayer->nickName, gamePlayer->getJettonCount());
//        }
//    }
}

void OnlinePokerDesk::sendPokerAction(){
    m_isSendSet = false;
    updateDeskState(DeskState_SendPoker);
    
    sprintf(showTimer->prefixString,"发牌");
    showTimer->start(Global::getInstance()->countDownInSecond);
}

void OnlinePokerDesk::showGamePlayerInfo(){
    this->playerListTableView->reloadData();
    
    char mString[100];
    for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
        PlayerData player_data = Global::getInstance()->playerList[i];
        if (0 == strcmp(Global::getInstance()->user_data.ID, player_data.user.ID)) {
            sprintf(mString,"%s\n筹码：%d",player_data.user.nikename, player_data.remainCap);
            gamePlayerInfoLabel->setString(mString);
            
            return;
        }
    }
    
    sprintf(mString,"%s\n筹码：%d", Global::getInstance()->user_data.nikename, jettonToEnter);
    gamePlayerInfoLabel->setString(mString);
}
void OnlinePokerDesk::showDealerInfo(){
    char mString[100];
    
    bool hasDealer = false;
    if (strlen(Global::getInstance()->table_data.bureauOwnerId) > 0) {
        for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
            PlayerData player_data = Global::getInstance()->playerList[i];
            if (0 == strcmp(Global::getInstance()->table_data.bureauOwnerId, player_data.user.ID)) {
                hasDealer = true;
                sprintf(mString,"庄家：%s\n筹码：%d\n玩家总数：%d\n",player_data.user.nikename, player_data.remainCap, Global::getInstance()->playerListCount);
            }
        }
        
    }
    
    if (!hasDealer) {
        sprintf(mString,"庄家：无\n玩家总数：%d\n", Global::getInstance()->playerListCount);
    }
    
    countLabel->setString(mString);
    playerListTableView->reloadData();
}

void OnlinePokerDesk::showTimerDoneCallback(Node* pNode){
    switch (m_deskState) {
//        case DeskState_SendPoker:{
//            if (m_isSendSet) {
//                //发完一把牌
//                updateDeskState(DeskState_Settle);
//            }
//            else {
//                //取消动画，强制发完牌
//                
//            }
//        }
//            break;
            
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
                        
                        if (totalBet * 3 > player_data.remainCap) {
                            NoteTip::show("下注不能超过本金1/3");
                        }
                        else {
                            JettonSprite* sp = this->createjetton(betLimiter->getSelectedJettonValue());
                            chair->updateJetton(sp);
                            Global::getInstance()->playEffect_add_gold(false);
                            
                            //                m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
                            totalBet += betLimiter->getSelectedJettonValue();
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
//                chooseStabberAction((int)m_arrChairs.getIndex(chair));
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
//    m_arrPokers.clear();
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

bool OnlinePokerDesk::reindexPoker(){
    for(int i = 0; i < m_arrPokers.size(); ++i){
        PokerSprite* pk1 = m_arrPokers.getRandomObject();
        PokerSprite* pk2 = m_arrPokers.getRandomObject();
        m_arrPokers.swap(pk1, pk2);
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 position = Vec2(origin.x + 0.3 * visibleSize.width, origin.y + 0.8 * visibleSize.height);
    for (size_t i = m_arrPokers.size(); i > 0; --i) {
        PokerSprite* pk = m_arrPokers.at(i - 1);
        pk->setPosition(position.x, position.y - (i - 1) * 0.005 * pk->getContentSize().height);
        pk->setVisible(true);
        if (pk->getIsFront()) {
            pk->showPokerAnimated(false, false, 0);
        }
        this->reorderChild(pk, (int)(m_arrPokers.size() - i));
    }
    
    return true;
}

void OnlinePokerDesk::sendPoker(){
    if (m_IndexSend >= m_arrPokers.size()) {
        return;
    }
    
    int index = m_IndexSend % 9;
    if (index == 0 && m_isSendSingle) {
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        
        int color = Global::getInstance()->pokerJudgement.color;
        int num = Global::getInstance()->pokerJudgement.num;
        
        if (color == 0 && (num == 14 || num == 15)) {
            color = (num == 14) ? PokerColor_JokerJunior : PokerColor_JokerSenior;
            num = PokerPoint_Joker;
        }
        pk->setPoker_color((PokerColor)color);
        pk->setPoker_point((PokerPoint)num);
        
        pk->showPokerAnimated(true, true, 0.5);
        
        judgementPokerIndex = m_IndexSend;
        ++m_IndexSend;
        m_isSendSingle = false;
        
        if (pk->getPoker_point() == PokerPoint_Joker) {
            m_IndexStart = 0;
        }
        else{
            m_IndexStart = pk->getPoker_point() - 1;
        }
    }
    else if (index > 0 && index <= 8 && m_isSendSingle) {
        int chair_index = ((index - 1) % m_arrChairs.size() + m_IndexStart) % m_arrChairs.size();
        
        PokerChair* chair = m_arrChairs.at(chair_index);
        
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);

        PokerPair pair = Global::getInstance()->pokerSendedList[chair_index];
        PokerData card = pair.poker[(index - 1) / 4];
        
        int color = card.color;
        int num = card.num;
        
        if (color == 0 && (num == 14 || num == 15)) {
            color = (num == 14) ? PokerColor_JokerJunior : PokerColor_JokerSenior;
            num = PokerPoint_Joker;
        }
        pk->setPoker_color((PokerColor)color);
        pk->setPoker_point((PokerPoint)num);
        
        movePoker(chair, pk);
        
        m_isSendSingle = false;
        ++m_IndexSend;
    }
}

void OnlinePokerDesk::movePoker(PokerChair* chair,PokerSprite* poker){
    float time = 0.5;
    poker->chairIndex = m_arrChairs.getIndex(chair);
    chair->pokerArray.pushBack(poker);
    MoveTo* move = MoveTo::create(time, chair->getPoint());
    RotateBy* rotate = RotateBy::create(time, 360);
    CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(OnlinePokerDesk::sendedSinglePoker, this, chair));
    Sequence* sequence = Sequence::create(Spawn::create(rotate,move, NULL),func,NULL);
    poker->runAction(sequence);
}

void OnlinePokerDesk::sendedSinglePoker(Node* pSender, void* pData){
    Global::getInstance()->playEffect_sendcard(false);
    
    this->reorderChild(pSender, 0);
    PokerChair* chair = (PokerChair* )pData;
    chair->updatePokerPosition();
    if (chair->pokerArray.size() == 2) {
        PokerSprite* pk0 = chair->pokerArray.at(0);
        PokerSprite* pk1 = chair->pokerArray.at(1);
        this->reorderChild(pk1, pk0->getLocalZOrder());
    }
    m_isSendSingle = true;
    if (m_IndexSend % 9 == 0) {
        m_isSendSet = true;
    }
}

void OnlinePokerDesk::turnedSinglePokerCallback(Node* pSender){
    PokerSprite* poker = (PokerSprite* )pSender;
    
    if (m_deskState == DeskState_SendPoker) {
        if (m_arrPokers.getIndex(poker) % 9 == 0) {
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at(i);
                chair->setHighlighted(i == (m_IndexStart % m_arrChairs.size()));
            }
            MoveTo* move = MoveTo::create(0.5, judgementPosition);
            Sequence* sequence = Sequence::create(move,NULL);
            poker->runAction(sequence);
            m_isSendSingle = true;
        }
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
            head->setPosition(0.25 * listCellWidth, 0.5 * height);
            cell->addChild(head, 0 , 2);
            
            Label* titleLabel = Label::createWithTTF("昵称:", "fonts/STKaiti.ttf", 8);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.75 * listCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.5 * listCellWidth, height);
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
    switch (post->cmd) {
        case cmd_removePlayer:{
            this->showMessageManager(false);
            
            Director::getInstance()->popScene();
        }
            break;
        case cmd_beginCountDownBeforeBureau:{
            this->showMessageManager(false);
            
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
            createPokers();
            reindexPoker();
            
            this->stepIn(DeskState_Start);
        }
            break;
        case cmd_countDownApplyBureauOwner:{
            //抢庄
            this->stepIn(DeskState_ChooseDealer);
        }
            break;
        case cmd_selectedBureauOwner:{
            //选中庄家通知
            btn_BeBankerItem->setVisible(false);
            showDealerInfo();
            
            if (Global::getInstance()->isDealer) {
                NoteTip::show("恭喜，抢庄成功！");
            }
            
            betLimiter->setVisible(!Global::getInstance()->isDealer);
        }
            break;
            
        case cmd_countDownApplyStabber:{
            //抢刺
            this->stepIn(DeskState_ChooseStabber);
        }
            break;
            
        case cmd_notifyStabber:{
            //选中刺通知
            for (int i = 0; i < Global::getInstance()->playerListCount; i++) {
                PlayerData player_data = Global::getInstance()->playerList[i];
                if (0 == strcmp(Global::getInstance()->table_data.stabberId, player_data.user.ID)) {
                    for (int i = 0; i < m_arrChairs.size(); i++) {
                        PokerChair* chairBuffer = m_arrChairs.at(i);
                        chairBuffer->showBeStabber(false);
                        if (i + 1 == Global::getInstance()->table_data.stabberIndex) {
                            chairBuffer->showStabber("images/default_head.png", player_data.user.nikename, player_data.remainCap);
                        }
                    }

                    
                    break;
                }
            }
            
        }
            break;
            
        case cmd_countDownBetStake:{
            //开始下注倒计时
            this->stepIn(DeskState_Bet);
        }
            break;
            
            
        case cmd_leaveRoom:{
            this->showMessageManager(false);
            
            Director::getInstance()->popScene();
        }
            break;
            
        case cmd_enterRoom:
        case cmd_enterRoomByPassword:{
            this->showMessageManager(false);
            
            switch (Global::getInstance()->table_data.code) {
                case state_enterRoom_success_wait:
                case state_enterRoom_success_prepare:{
                    sprintf(showTimer->prefixString,"%s", Global::getInstance()->table_data.description);
                    showTimer->showPrefix();
                    
                    scheduleUpdate();
                }
                    break;

                case state_enterRoom_fail_no_money:
                case state_enterRoom_fail_full:{
                    NoteTip::show(Global::getInstance()->table_data.description);
                }
                    break;

                default:
                    break;
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
            
        case cmd_countDownSendCard:{
            this->showMessageManager(false);
            
            sendPokerAction();
        }
            break;
            
        case cmd_sysBetStake:{
            for (int j = 1; j < m_arrChairs.size(); j++) {
                PokerChair* chairBuffer = m_arrChairs.at(j);
                chairBuffer->updateTotal(Global::getInstance()->betList[j]);
            }
        }
            break;
        
        case cmd_settle:{
            this->showMessageManager(false);
            
            this->stepIn(DeskState_Settle);
        }
            break;
            
        default:
            break;
    }
}
