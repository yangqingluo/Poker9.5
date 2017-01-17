//
//  PokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "PokerDeskScene.h"
#include "PopAlertDialog.h"
#include "Global.h"

const float jetton_height_scale = 0.08;

PokerDesk::PokerDesk():m_deskState(0),m_IndexSend(0),m_IndexStart(0),m_isSendSingle(true),m_isSendSet(true),stabberPlayer(NULL),dealerPlayer(NULL){
    gamePlayer = new Player();
    gamePlayer->retain();
    
    pcPlayer = new Player();
    pcPlayer->retain();
    pcPlayer->infoConfig("电脑", "images/p2.png", 3000);
    
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(PokerDesk::onNotification_Socket), kNotification_Socket, NULL);
}

PokerDesk::~PokerDesk(){
    NotificationCenter::getInstance()->removeAllObservers(this);
    Global::getInstance()->table_data = {0};
    
    CC_SAFE_RELEASE(gamePlayer);
    CC_SAFE_RELEASE(pcPlayer);
//    CC_SAFE_RELEASE(stabberPlayer);
//    CC_SAFE_RELEASE(dealerPlayer);
}

Scene* PokerDesk::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PokerDesk::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool PokerDesk::init()
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
                                              CC_CALLBACK_1(PokerDesk::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    btn_PrepareItem = MenuItemImage::create(
                                              "images/btn_prepare.png",
                                              "images/btn_prepare.png",
                                              CC_CALLBACK_1(PokerDesk::buttonCallback, this, 1));
    
    btn_PrepareItem->setScale(0.09 * visibleSize.height / btn_PrepareItem->getContentSize().height);
    btn_PrepareItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 0.25 * visibleSize.height));
    
    
    btn_PrepareItem->setVisible(true);
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, btn_PrepareItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto lotus = Sprite::create("images/lotus.png");
    lotus->setScale(150.0 / 640.0 * visibleSize.height / lotus->getTextureRect().getMaxY());
    lotus->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - lotus->getBoundingBox().size.height / 2);
    this->addChild(lotus);
    
    upright_sprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.15 * visibleSize.width, 0.15 * visibleSize.width));
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
                                             CC_CALLBACK_1(PokerDesk::buttonCallback, this, 3));
    btn_BeBankerItem->setScale(0.8 * upright_sprite->getContentSize().width / btn_BeBankerItem->getContentSize().width);
    btn_BeBankerItem->setPosition(0.5 * upright_sprite->getContentSize().width, 0.75 * upright_sprite->getContentSize().height);
    btn_BeBankerItem->setVisible(false);
    
    countLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 8);
    countLabel->setColor(Color3B::BLACK);
//    countLabel->setHorizontalAlignment(TextHAlignment::LEFT);
//    countLabel->setVerticalAlignment(TextVAlignment::CENTER);
    countLabel->setPosition(0.5 * upright_sprite->getContentSize().width, 0.3 * upright_sprite->getContentSize().height);
    upright_sprite->addChild(countLabel);
    
    auto btn_playerList = MenuItemFont::create("点击查看玩家列表", CC_CALLBACK_1(PokerDesk::buttonCallback, this, 4));
    btn_playerList->setFontSizeObj(8);
    btn_playerList->setColor(Color3B::BLACK);
    btn_playerList->setPosition(0.5 * upright_sprite->getContentSize().width, 0.5 * btn_playerList->getContentSize().height);
    
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
    
//    message_sprite = QLImageSprite::create("images/message_bg.png", Size(928.0 / 104.0 * 0.05 * visibleSize.height, 0.05 * visibleSize.height));
//    message_sprite->setPosition(0.5 * message_sprite->getContentSize().width, 0.5 * bottom_sprite->getContentSize().height);
////    bottom_sprite->addChild(message_sprite);
//    
//    messageLabel = Label::createWithTTF("正在等待玩家加入...", "fonts/STKaiti.ttf", 10);
//    messageLabel->setColor(Color3B::BLACK);
//    messageLabel->setHorizontalAlignment(TextHAlignment::LEFT);
//    messageLabel->setVerticalAlignment(TextVAlignment::CENTER);
//    messageLabel->setDimensions(message_sprite->getContentSize().width, message_sprite->getContentSize().height);
//    messageLabel->setPosition(0.5 * message_sprite->getContentSize().width, 0.5 * message_sprite->getContentSize().height);
//    message_sprite->addChild(messageLabel);
    
    int betJettonArray[6] = {10,50,100,500,1000,5000};
    betLimiter = BetLimiter::create(betJettonArray, 6, Size(0.8 * bottom_sprite->getContentSize().width, 1.0 * bottom_sprite->getContentSize().height), BetType_Addition);
    betLimiter->setPosition(0.5 * bottom_sprite->getContentSize().width - 0.5 * betLimiter->getContentSize().width, 0.5 * bottom_sprite->getContentSize().height - 0.5 * betLimiter->getContentSize().height);
    bottom_sprite->addChild(betLimiter);
    
    showTimer = GameTimer::createTimer();
    showTimer->showTag = 0;
    showTimer->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.74 * visibleSize.height);
    showTimer->setCallBackFunc(this,callfuncN_selector(PokerDesk::showTimerDoneCallback));
    this->addChild(showTimer);
    
    char imageName[4][100] = {"","images/chair_bet_1_bg.png","images/chair_bet_2_bg.png","images/chair_bet_3_bg.png"};
    float scaleArray[4][2] = {{0.5,0.65},{0.24,0.4},{0.5,0.4},{0.76,0.4}};
    for (int i = 0; i < 4; i++) {
        PokerChair* chair = this->createChair(imageName[i], scaleArray[i][0], scaleArray[i][1], i);
        m_arrChairs.pushBack(chair);
        this->addChild(chair, 0);
    }
    
    srand((unsigned)time(NULL));//初始化随机种子
    
    return true;
}

void PokerDesk::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
            
            Global::getInstance()->sendLeaveRoom();
            
        }
            break;
            
        case 1:{
            this->preparedAction();
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

void PokerDesk::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        m_deskState = DeskState_Waiting;
        scheduleUpdate();
    }
    else if(pNode->getTag() == 1) {
        this->goBackAction();
    }
    pNode->removeFromParent();
}

void PokerDesk::onEnter(){
    Layer::onEnter();
    
//    betLimiter->minValue = this->perMin;
    betLimiter->reset();
    
    playerList_sprite = QLImageSprite::create("images/window_upright_bg.png", Size(upright_sprite->getContentSize().width, 0.9 * (upright_sprite->getBoundingBox().getMinY() - bottom_sprite->getBoundingBox().getMaxY())));
    playerList_sprite->setPosition(upright_sprite->getPositionX(), 0.5 * (upright_sprite->getBoundingBox().getMinY() + bottom_sprite->getBoundingBox().getMaxY()));
    playerList_sprite->setVisible(false);
    this->addChild(playerList_sprite);
    
    playerListTableView = TableView::create(this, Size(playerList_sprite->getContentSize().width,  playerList_sprite->getContentSize().height));
    playerListTableView->setPosition(0 , 0);
    playerListTableView->setDirection(TableView::Direction::VERTICAL);
    playerListTableView->setDelegate(this);
    playerList_sprite->addChild(playerListTableView);
    
    this->showGamePlayerInfo();
    this->showDealerInfo();
    
    m_deskState = DeskState_Waiting;
    scheduleUpdate();
}

void PokerDesk::onExit(){
    Layer::onExit();
    unscheduleUpdate();
    
}

void PokerDesk::showSettingChip(){
    PopAlertDialog* popup = PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
    popup->setTitle("");
    popup->setContentText("请设置本金数目",12,50,130);
    popup->setCallBackFunc(this,callfuncN_selector(PokerDesk::popButtonCallback));
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "",0);
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "",1);
    
    this->addChild(popup);
    
    ControlSlider* myslider = ControlSlider::create("images/slider_bg.png","images/slider_jd.png","images/slider_hk.png");
    myslider->setPosition(popup->getContentSize().width / 2, popup->getContentSize().height * 0.45);
    myslider->setMaximumValue(100);
    myslider->setMinimumValue(0);
    popup->addChild(myslider);
}

void PokerDesk::goBackAction(){
    Director::getInstance()->popScene();
}

void PokerDesk::waitForPrepareAction(){
    if (!showTimer->getIsValid()) {
        m_IndexSend = 0;
        
        btn_PrepareItem->setVisible(true);
        
        sprintf(showTimer->prefixString,"等待准备…");
        showTimer->start(30);
    }
}

void PokerDesk::preparedAction(){
    createPokers();
    reindexPoker();
    
    m_deskState = DeskState_Prepared;
    
    btn_PrepareItem->setVisible(false);
    
    sprintf(showTimer->prefixString,"等待开始…");
    showTimer->showPrefix();
}

void PokerDesk::waitForBetAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString,"设置筹码，选择过、天、坎下注");
        showTimer->start(10);
        
        Global::getInstance()->playEffect_place(false);
    }
}

void PokerDesk::settleAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString,"结算");
        showTimer->start(15);
        
        if (stabberPlayer == NULL) {
            int zeroCount = 0;//牌型为0点的座位计数
            int accountDealer = 0;
            int accountPlayer = 0;
            PokerChair* chair0 = m_arrChairs.at(0);
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at(i % m_arrChairs.size());
                chair->calculatePokerType();
                if (i > 0) {
                    chair->calculateSettlement(chair0);
                    accountDealer -= chair->m_settlement.accounts;
                    if (chair->betPlayer > 0) {
                        accountPlayer += chair->m_settlement.accounts;
                    }
                    chair->showSettlement();
                }
                
                if (chair->m_PokerType == PokerType_0) {
                    zeroCount++;
                }
                
                chair->showPokerType();
                
                for (int i = 0; i < chair->pokerArray.size(); i++) {
                    PokerSprite* poker = chair->pokerArray.at(i);
                    poker->showPokerAnimated(true, true, 0);
                }
            }
            
            if (zeroCount < 3) {
                dealerPlayer->setJettonCount(dealerPlayer->getJettonCount() + accountDealer);
                gamePlayer->setJettonCount(gamePlayer->getJettonCount() + accountPlayer);
            }
            else {
                //跳过结算
                
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
                        if (stabberPlayer == gamePlayer) {
                            accountDealer -= chair->m_settlement.accounts;
                            accountPlayer += chair->m_settlement.accounts;
                        }
                    }
                }
                
                chair->showPokerType();
                
                for (int i = 0; i < chair->pokerArray.size(); i++) {
                    PokerSprite* poker = chair->pokerArray.at(i);
                    poker->showPokerAnimated(true, true, 0);
                }
            }
            
            dealerPlayer->setJettonCount(dealerPlayer->getJettonCount() + accountDealer);
            gamePlayer->setJettonCount(gamePlayer->getJettonCount() + accountPlayer);
        }
        
        showDealerInfo();
        showGamePlayerInfo();
    }
}

void PokerDesk::waitForChooseDealerAction(){
    if (!showTimer->getIsValid()) {
        btn_BeBankerItem->setVisible(true);
        
        sprintf(showTimer->prefixString,"抢庄");
        showTimer->start(10);
    }
}

void PokerDesk::chooseDealerAction(){
    //单机模拟强制选择电脑为庄家
    if (dealerPlayer == NULL) {
        dealerPlayer = pcPlayer;
        dealerHead->setTexture(dealerPlayer->headImage);
        btn_BeBankerItem->setVisible(false);
        dealerHead->setVisible(true);
    }
    
    showDealerInfo();
}

void PokerDesk::dealerDidChoosedAction(){
    if (dealerPlayer->getJettonCount() * 3 < dealerPlayer->getJettonInitial()) {
        //庄家本金小于初始本金1/3，抢刺
        m_deskState = DeskState_ChooseStabber;
    }
    else {
        //下注
        m_deskState = DeskState_Bet;
    }
}

void PokerDesk::waitForChooseStabberAction(){
    if (!showTimer->getIsValid()) {
        if (gamePlayer->getJettonCount() >= dealerPlayer->getJettonCount()) {
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at((i + m_IndexStart) % m_arrChairs.size());
                chair->showBeStabber(true);
            }
            sprintf(showTimer->prefixString,"抢刺");
        }
        else {
            sprintf(showTimer->prefixString,"等待其他玩家抢刺");
        }
        
        showTimer->start(10);
    }
}

void PokerDesk::chooseStabberAction(int index){
    stabberPlayer = gamePlayer;
    for (int i = 0; i < m_arrChairs.size(); i++) {
        PokerChair* chair = m_arrChairs.at(i);
        chair->showBeStabber(false);
        if (i == index) {
            chair->showStabber(gamePlayer->headImage, gamePlayer->nickName, gamePlayer->getJettonCount());
        }
    }
}

void PokerDesk::sendPokerAction(){
    m_isSendSet = false;
    m_deskState = DeskState_SendPoker;
    
    sprintf(showTimer->prefixString,"发牌");
    showTimer->start(10);
}

void PokerDesk::showGamePlayerInfo(){
    char mString[100];
    sprintf(mString,"%s\n%d",gamePlayer->nickName, gamePlayer->getJettonCount());
    gamePlayerInfoLabel->setString(mString);
}
void PokerDesk::showDealerInfo(){
    if (dealerPlayer != NULL) {
        char mString[100];
        sprintf(mString,"庄家：%s\n本金：%d\n玩家总数：2\n",dealerPlayer->nickName, dealerPlayer->getJettonCount());
        countLabel->setString(mString);
    }
    else{
        countLabel->setString("庄家：无\n玩家总数：2\n");
    }
    
    playerListTableView->reloadData();
}

void PokerDesk::showTimerDoneCallback(Node* pNode){
    switch (m_deskState) {
        case DeskState_Waiting:{
            goBackAction();
        }
            break;
        case DeskState_Prepared:{
            if (dealerPlayer != NULL) {
                dealerDidChoosedAction();
            }
            else{
                m_deskState = DeskState_ChooseDealer;
            }
        }
            break;
            
        case DeskState_ChooseDealer:{
            chooseDealerAction();
            
            dealerDidChoosedAction();
        }
            break;
            
        case DeskState_ChooseStabber:{
            if (stabberPlayer != NULL) {
                sendPokerAction();
            }
            else {
                for (int i = 0; i < m_arrChairs.size(); i++) {
                    PokerChair* chair = m_arrChairs.at(i % m_arrChairs.size());
                    chair->showBeStabber(false);
                }
                
                m_deskState = DeskState_Bet;
            }
        }
            break;
            
        case DeskState_Bet:{
            sendPokerAction();
        }
            break;
            
        case DeskState_SendPoker:{
            if (m_isSendSet) {
                //发完一把牌
                m_deskState = DeskState_Settle;
            }
            else {
                //取消动画，强制发完牌
                
            }
        }
            break;
            
        case DeskState_Settle:{
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at((i + m_IndexStart) % m_arrChairs.size());
                chair->clearChair();
            }
            if (judgementPokerIndex < m_arrPokers.size()) {
                PokerSprite* poker = m_arrPokers.at(judgementPokerIndex);
                poker->setVisible(false);
            }
            
            if (stabberPlayer != NULL) {
                stabberPlayer = NULL;
            }
            
            //测试
            if (dealerPlayer->getJettonCount() <= 0) {
                dealerPlayer->setJettonCount(900);
                showDealerInfo();
            }
            
            if (m_IndexSend < m_arrPokers.size()) {
                dealerDidChoosedAction();
            }
            else{
                m_deskState = DeskState_Waiting;
            }
        }
            break;
            
        default:
            break;
    }
}


void PokerDesk::update(float delta){
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
void PokerDesk::touchedChairCallback(Node* pSender, void* pTarget){
    PokerChair* chair = (PokerChair* )pSender;
    Node* node = (Node* )pTarget;
    switch (node->getTag()) {
        case 10:{
            if (m_deskState == DeskState_Bet) {
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
                
                if (totalBet * 3 > gamePlayer->getJettonCount()) {
                    NoteTip::show("下注不能超过本金1/3");
//                    betLimiter->reset();
                }
                else {
                    JettonSprite* sp = this->createjetton(betLimiter->getSelectedJettonValue());
                    chair->updateJetton(sp);
                    Global::getInstance()->playEffect_add_gold(false);
                }
                
            }
        }
            break;
            
        case 11:{
            if (m_deskState == DeskState_ChooseStabber) {
                chooseStabberAction((int)m_arrChairs.getIndex(chair));
            }
        }
            break;
            
        default:
            break;
    }
    
}

PokerChair* PokerDesk::createChair(const char* backgroudImage, float xScale, float yScale, int index){
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
        chair->setTouchCallBackFunc(this, callfuncND_selector(PokerDesk::touchedChairCallback));
    }
    
    return chair;
}

#pragma jetton
JettonSprite* PokerDesk::createjetton(int value){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    JettonSprite* sp = JettonSprite::createWithEmpty(value, Size(jetton_height_scale * visibleSize.height, jetton_height_scale * visibleSize.height));
    sp->isPlayer = true;
    return sp;
}

void PokerDesk::addJetton(PokerChair* chair,JettonSprite* jetton){
    
}

#pragma poker
PokerSprite* PokerDesk::createPoker(PokerColor color,PokerPoint point){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PokerSprite* pk = PokerSprite::create(color, point, Size(0.09375 * visibleSize.height, 0.125 * visibleSize.height));
    
    return pk;
}
bool PokerDesk::createPokers(){
//    m_arrPokers.clear();
    if (m_arrPokers.size() == 0) {
        //创建52个牌
        for (int i = PokerColor_Spade; i <= PokerColor_Diamond; ++i){
            for (int j = PokerPoint_Ace; j <= PokerPoint_King; ++j){
                PokerSprite* pk = createPoker((PokerColor)i, (PokerPoint)j);
                m_arrPokers.pushBack(pk);
                this->addChild(pk);
                pk->setCallBackFunc(this, callfuncN_selector(PokerDesk::turnedSinglePokerCallback));
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
        joker_junior->setCallBackFunc(this, callfuncN_selector(PokerDesk::turnedSinglePokerCallback));
        joker_senior->setCallBackFunc(this, callfuncN_selector(PokerDesk::turnedSinglePokerCallback));
    }
    
    return true;
}

bool PokerDesk::reindexPoker(){
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

void PokerDesk::sendPoker(){
    if (m_IndexSend >= m_arrPokers.size()) {
        return;
    }
    
    int index = m_IndexSend % 9;
    if (index == 0 && m_isSendSingle) {
//        sprintf(showTimer->prefixString,"翻牌决定发牌顺序");
//        showTimer->showPrefix();
        
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
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
//        sprintf(showTimer->prefixString,"发牌");
//        showTimer->showPrefix();
        
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        PokerChair* chair = m_arrChairs.at(((index - 1) % m_arrChairs.size() + m_IndexStart) % m_arrChairs.size());
        movePoker(chair, pk);
        
        m_isSendSingle = false;
        ++m_IndexSend;
    }
}

void PokerDesk::movePoker(PokerChair* chair,PokerSprite* poker){
    float time = 0.5;
    poker->chairIndex = m_arrChairs.getIndex(chair);
    chair->pokerArray.pushBack(poker);
    MoveTo* move = MoveTo::create(time, chair->getPoint());
    RotateBy* rotate = RotateBy::create(time, 360);
    CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(PokerDesk::sendedSinglePoker, this, chair));
    Sequence* sequence = Sequence::create(Spawn::create(rotate,move, NULL),func,NULL);
    poker->runAction(sequence);
}

void PokerDesk::sendedSinglePoker(Node* pSender, void* pData){
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

void PokerDesk::turnedSinglePokerCallback(Node* pSender){
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
Size PokerDesk::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == playerListTableView) {
        return Size(playerList_sprite->getContentSize().width, 30);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* PokerDesk::tableCellAtIndex(TableView* table, ssize_t idx)
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
            
            Label* titleLabel = Label::createWithTTF("test", "fonts/STKaiti.ttf", 8);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.75 * listCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.5 * listCellWidth, height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->addChild(titleLabel, 0 , 1);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
        Sprite* head = (Sprite* )cell->getChildByTag(2);
        char content[100];
        if (idx == 0) {
            sprintf(content, "电脑\n%d",pcPlayer->getJettonCount());
            head->setTexture(pcPlayer->headImage);
        }
        else{
            sprintf(content, "%s\n%d", gamePlayer->nickName, gamePlayer->getJettonCount());
            head->setTexture(gamePlayer->headImage);
        }
        
        label->setString(content);
        
        
        
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t PokerDesk::numberOfCellsInTableView(TableView* table)
{
    if (table == playerListTableView) {
        return 2;
    }
    
    return 0;
}

void PokerDesk::tableCellTouched(TableView* table, TableViewCell* cell){
    
}

#pragma notification
void PokerDesk::onNotification_Socket(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    switch (post->cmd) {
        case cmd_leaveRoom:{
            if (m_pMessage != NULL) {
                m_pMessage->hidden();
            }
            
            Director::getInstance()->popScene();
        }
            break;
            
        default:
            break;
    }
}
