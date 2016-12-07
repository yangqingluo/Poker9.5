//
//  PokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "PokerDeskScene.h"
#include "PopAlertDialog.h"

const float jetton_height_scale = 0.08;

PokerDesk::PokerDesk():m_deskState(0),m_IndexSend(0),m_IndexStart(0),m_isSendSingle(true){
    dealerPlayer = new Player();
    gamePlayer = new Player();
}

PokerDesk::~PokerDesk(){
    CC_SAFE_DELETE(dealerPlayer);
    CC_SAFE_DELETE(gamePlayer);
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
    
    btn_PrepareItem->setScale(this->getScaleX(), this->getScaleY());
    btn_PrepareItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 0.8 * btn_PrepareItem->getContentSize().width, origin.y + 0.25 * visibleSize.height));
    
    btn_AnotherdeskItem = MenuItemImage::create(
                                                 "images/btn_anotherdesk.png",
                                                 "images/btn_anotherdesk.png",
                                                 CC_CALLBACK_1(PokerDesk::buttonCallback, this, 2));
    
    btn_AnotherdeskItem->setScale(this->getScaleX(), this->getScaleY());
    btn_AnotherdeskItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 0.8 * btn_AnotherdeskItem->getContentSize().width, btn_PrepareItem->getPositionY()));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, btn_PrepareItem, btn_AnotherdeskItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 0);
    
    auto lotus = Sprite::create("images/lotus.png");
    lotus->setScale(150.0 / 640.0 * visibleSize.height / lotus->getTextureRect().getMaxY());
    lotus->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - lotus->getBoundingBox().size.height / 2);
    this->addChild(lotus);
    
    upright_sprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.15 * visibleSize.width, 0.15 * visibleSize.width));
    upright_sprite->setPosition(origin.x + visibleSize.width - upright_sprite->getContentSize().width * 0.6, origin.y + visibleSize.height - upright_sprite->getContentSize().height * 0.6);
    this->addChild(upright_sprite);
    
    countLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 8);
    countLabel->setColor(Color3B::BLACK);
//    countLabel->setHorizontalAlignment(TextHAlignment::LEFT);
//    countLabel->setVerticalAlignment(TextVAlignment::CENTER);
    countLabel->setPosition(0.5 * upright_sprite->getContentSize().width, 0.2 * upright_sprite->getContentSize().height);
    upright_sprite->addChild(countLabel);
    chooseDealerAction();
    
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
    
    int betJettonArray[3] = {10,100,1000};
    betLimiter = BetLimiter::create(betJettonArray, 3, Size(bottom_sprite->getContentSize().width, 0.8 * bottom_sprite->getContentSize().height));
    betLimiter->setPosition(2 * bottom_sprite->getContentSize().height, 0.5 * bottom_sprite->getContentSize().height - 0.5 * betLimiter->getContentSize().height);
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
        this->addChild(chair);
    }
    
    srand((unsigned)time(NULL));//初始化随机种子
    
    return true;
}

void PokerDesk::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            this->goBackAction();
        }
            break;
            
        case 1:{
            this->preparedAction();
        }
            break;
            
        case 2:{
            
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
    this->showSettingChip();
    this->showGamePlayerInfo();
    this->showDealerInfo();
}

void PokerDesk::onExit(){
    Layer::onExit();
    unscheduleUpdate();
}

void PokerDesk::showSettingChip(){
    PopAlertDialog* popup = PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
    popup->setTitle("");
    popup->setContentText("请设置带入的金币数目",12,50,150);
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
        btn_AnotherdeskItem->setVisible(true);
        
        sprintf(showTimer->prefixString,"等待准备…");
        showTimer->start(3);
    }
}

void PokerDesk::preparedAction(){
    createPokers();
    reindexPoker();
    
    m_deskState = DeskState_Prepared;
    
    btn_PrepareItem->setVisible(false);
    btn_AnotherdeskItem->setVisible(false);
    
    sprintf(showTimer->prefixString,"等待开始…");
    showTimer->showPrefix();
}

void PokerDesk::betAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString,"选择筹码，点击选择过门、天门、坎门下注");
        showTimer->start(3);
    }
}

void PokerDesk::settleAction(){
    if (!showTimer->getIsValid()) {
        sprintf(showTimer->prefixString,"结算");
        showTimer->start(5);
        
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
            }
            
            if (chair->m_PokerType == PokerType_0) {
                zeroCount++;
            }
            
            chair->showPokerType();
            chair->showSettlement();
            for (int i = 0; i < chair->pokerArray.size(); i++) {
                PokerSprite* poker = chair->pokerArray.at(i);
                poker->showPokerAnimated(true, true, 0);
            }
        }
        
        if (zeroCount < 3) {
            dealerPlayer->setJettonCount(dealerPlayer->getJettonCount() + accountDealer);
            gamePlayer->setJettonCount(gamePlayer->getJettonCount() + accountPlayer);
        }
        
        showDealerInfo();
        showGamePlayerInfo();
        
//        for (int i = 0; i < m_arrChairs.size(); i++) {
//            PokerChair* chair = m_arrChairs.at((i + m_IndexStart) % m_arrChairs.size());
//            chair->calculatePokerType();
//            for (PokerSprite* poker : chair->pokerArray) {
//                poker->showPokerAnimated(true, true, 0);
//            }
//        }
    }
}

void PokerDesk::chooseDealerAction(){
    sprintf(dealerPlayer->nickName,"电脑");
    sprintf(dealerPlayer->headImage,"p2");
    
    dealerPlayer->setJettonCount(3000000);
    
    auto dealerHead = Sprite::create("images/p2.png");
    dealerHead->setScale(0.4 * upright_sprite->getContentSize().width / dealerHead->getContentSize().width);
    dealerHead->setPosition(0.5 * upright_sprite->getContentSize().width, 0.95 * upright_sprite->getContentSize().height - 0.5 * dealerHead->getBoundingBox().size.height);
    upright_sprite->addChild(dealerHead);
}

void PokerDesk::showGamePlayerInfo(){
    char mString[100];
    sprintf(mString,"%s\n筹码：%d",gamePlayer->nickName, gamePlayer->getJettonCount());
    gamePlayerInfoLabel->setString(mString);
}
void PokerDesk::showDealerInfo(){
    char mString[100];
    sprintf(mString,"庄家：%s\n筹码：%d\n桌子人数：2\n",dealerPlayer->nickName, dealerPlayer->getJettonCount());
    countLabel->setString(mString);
}

void PokerDesk::showTimerDoneCallback(Node* pNode){
    switch (m_deskState) {
        case DeskState_Waiting:{
            goBackAction();
        }
            break;
        case DeskState_Prepared:{
            m_deskState = DeskState_Bet;
        }
            break;
            
        case DeskState_Bet:{
            m_deskState = DeskState_SendPoker;
        }
            break;
            
        case DeskState_Settle:{
            for (int i = 0; i < m_arrChairs.size(); i++) {
                PokerChair* chair = m_arrChairs.at((i + m_IndexStart) % m_arrChairs.size());
                chair->clearChair();
            }
            
            if (m_IndexSend < m_arrPokers.size()) {
                m_deskState = DeskState_Bet;
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
            
        case DeskState_SendPoker:{
            sendPoker();
        }
            break;
            
        case DeskState_Bet:{
            betAction();
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
void PokerDesk::touchedChairCallback(Node* pSender){
    if (m_deskState == DeskState_Bet) {
        PokerChair* chair = (PokerChair* )pSender;
        
        JettonSprite* sp = this->createjetton(betLimiter->getSelectedJettonValue());
        chair->addJetton(sp);
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
        chair->setTouchCallBackFunc(this, callfuncN_selector(PokerDesk::touchedChairCallback));
    }
    
    return chair;
}

#pragma jetton
JettonSprite* PokerDesk::createjetton(int value){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    JettonSprite* sp = JettonSprite::create(value, Size(jetton_height_scale * visibleSize.height, jetton_height_scale * visibleSize.height));
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
        sprintf(showTimer->prefixString,"翻牌决定发牌顺序");
        showTimer->showPrefix();
        
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        pk->showPokerAnimated(true, true, 0.5);
        
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
        sprintf(showTimer->prefixString,"发牌");
        showTimer->showPrefix();
        
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
        m_deskState = DeskState_Settle;
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
            poker->setVisible(false);
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
