//
//  PokerChair.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "PokerChair.h"
#include "JettonSprite.h"



PokerChair::PokerChair():m_BeStabberSprite(NULL),m_Stabber(NULL),m_BankerSprite(NULL),m_betZoneBackGround(NULL),m_touchListener(NULL),m_touchCallback(NULL),betTotal(0),betPlayer(0),m_canTouch(false){
    
}

PokerChair::~PokerChair(){
    CC_SAFE_RELEASE(m_betZoneBackGround);
    
}

bool PokerChair::init(){
    if (!LayerColor::init()) {
        return false;
    }
    
    m_settlement = {0};
    //设置弹出层的颜色，指定为淡灰色
//    setColor(Color3B::GRAY);
//    setOpacity(0x50);
    
    return true;
}

bool PokerChair::onTouchBegan(Touch* touch,Event* event){
    auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode) && m_canTouch){
        if (m_touchCallback && m_touchListener) {
            (m_touchListener->*m_touchCallback)(this, target);
        }
        
        return true;
    }
    
    return false;
}

void PokerChair::onEnter(){
    LayerColor::onEnter();
    
    pokerTypeLabel->setPosition(0.5 * this->getContentSize().width, -0.08 * this->getContentSize().height);
    
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        background->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.5 * background->getContentSize().height);
        
        betTotalLabel->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.2 * background->getContentSize().height);
        
        betPlayerLabel->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.8 * background->getContentSize().height);
        
        settlementLabel->setPosition(0.5 * this->getContentSize().width, -0.24 * this->getContentSize().height);
        
        m_BeStabberSprite->setPosition(Vec2(background->getBoundingBox().getMinX(), background->getBoundingBox().getMinY()));
        m_Stabber->setPosition(m_BeStabberSprite->getPosition());
        
        //触摸响应注册
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(PokerChair::onTouchBegan, this);//触摸开始
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, background);//注册分发器
        
        //触摸响应注册
        auto stabber_listener = EventListenerTouchOneByOne::create();
        stabber_listener->setSwallowTouches(false);
        stabber_listener->onTouchBegan = CC_CALLBACK_2(PokerChair::onTouchBegan, this);//触摸开始
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(stabber_listener, m_BeStabberSprite);//注册分发器
        
    }
    else{
        pokerTypeLabel->setPosition(m_BankerSprite->getPositionX(), 0.3 * this->getContentSize().height);
    }
}

void PokerChair::onExit(){
    //移除触摸响应
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        getEventDispatcher()->removeEventListenersForTarget(background);
    }
    if (m_BeStabberSprite != NULL) {
        getEventDispatcher()->removeEventListenersForTarget(m_BeStabberSprite);
    }
    
    LayerColor::onExit();
}

PokerChair* PokerChair::create(const char* betZoneImage,Size size){
    PokerChair* layer = PokerChair::create();
    if (betZoneImage) {
        layer->setBetZoneBackGround(QLImageSprite::create(betZoneImage, size));
    }
    
    layer->initialSubViews();
    
    return layer;
}

void PokerChair::initialSubViews(){
    pokerTypeLabel = Label::createWithTTF("", "fonts/microsoftSong.ttf", 12);
    pokerTypeLabel->setVisible(false);
    pokerTypeLabel->setPosition(0.5 * this->getContentSize().width, -0.08 * this->getContentSize().height);
    this->addChild(pokerTypeLabel);
    
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        background->setTag(10);
        background->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.5 * background->getContentSize().height);
        this->addChild(background);
        
        betTotalLabel = Label::createWithSystemFont("", "Arial", 20.0);
        betTotalLabel->setColor(Color3B::WHITE);
        //        betTotalLabel->enableShadow(Color4B::BLACK, Size(1, 1)); //阴影
        //        betTotalLabel->enableOutline(Color4B::WHITE, 1);             //轮廓
        betTotalLabel->setVisible(false);
        betTotalLabel->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.2 * background->getContentSize().height);
        this->addChild(betTotalLabel);
        
        betPlayerLabel = Label::createWithSystemFont("", "Arial", 20.0);
        betPlayerLabel->setColor(Color3B::YELLOW);
        //        betPlayerLabel->enableShadow(Color4B::BLACK, Size(1, 1)); //阴影
        //        betPlayerLabel->enableOutline(Color4B::YELLOW, 1);             //轮廓
        betPlayerLabel->setVisible(false);
        betPlayerLabel->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.8 * background->getContentSize().height);
        this->addChild(betPlayerLabel);
        
        settlementLabel = Label::createWithTTF("", "fonts/microsoftSong.ttf", 12);
        settlementLabel->setVisible(false);
        settlementLabel->setPosition(0.5 * this->getContentSize().width, -0.24 * this->getContentSize().height);
        this->addChild(settlementLabel);
        
        Size size_stabber = background->getContentSize();
        
        m_BeStabberSprite = PokerStabberBtn::create(Color4B(0, 0, 0, 0), size_stabber);
        this->addChild(m_BeStabberSprite);
        m_BeStabberSprite->setVisible(false);
        
        m_Stabber = PokerStabber::create("images/default_head.png", size_stabber);
        this->addChild(m_Stabber);
        m_Stabber->setVisible(false);
        
        m_BeStabberSprite->setTag(11);        
    }
}

void PokerChair::setIsBanker(bool yn){
    if (yn) {
        if (m_BankerSprite == NULL) {
            m_BankerSprite = Sprite::create("images/banker.png");
            m_BankerSprite->setScale(0.5 * this->getContentSize().height / m_BankerSprite->getContentSize().height);
            m_BankerSprite->setPosition(this->getContentSize().width - 0.8 * m_BankerSprite->getBoundingBox().size.width, this->getContentSize().height - 0.5 * m_BankerSprite->getBoundingBox().size.height);
            this->addChild(m_BankerSprite);
        }
    }
    else{
        if (m_BankerSprite != NULL) {
            m_BankerSprite->removeFromParentAndCleanup(true);
        }
    }
}

void PokerChair::setHighlighted(bool yn){
//    setColor(yn ? Color3B::BLACK : Color3B::GRAY);
}

void PokerChair::updatePokerPosition(){
    if (pokerArray.size() < 2) {
        return;
    }
    
    float scale = 0.7;
    size_t count = pokerArray.size();
    //更新位置
    for (size_t i = 0; i < count; i++) {
        PokerSprite* pk = pokerArray.at(i);
        pk->setPositionX(m_point.x - 0.5 * pk->getContentSize().width * (count - (count - 1) * scale) + i * pk->getContentSize().width * (1 - scale) + 0.5 * pk->getContentSize().width);
    }
}

void PokerChair::removeAllPokers(){
    for (int i = 0; i < pokerArray.size(); i++) {
        PokerSprite* pk = pokerArray.at(i);
        pk->setVisible(false);
    }
    pokerArray.clear();
}

void PokerChair::addJetton(JettonSprite* jetton){
    updateTotal(betTotal + jetton->getJettonValue());
    
    char mString[100] = {0};
    
    if (jetton->isPlayer) {
        betPlayer += jetton->getJettonValue();
        if (!betPlayerLabel->isVisible()) {
            betPlayerLabel->setVisible(true);
        }
        
        this->stringFromBetValue(mString, betPlayer);
        betPlayerLabel->setString(mString);
    }
    
    jetton->setPosition(0.1 * getRandomNumber(0, 10) * (m_betZoneBackGround->getContentSize().width - jetton->getContentSize().width) + 0.5 * jetton->getContentSize().width, 0.1 * getRandomNumber(0, 10) * (m_betZoneBackGround->getContentSize().height - jetton->getContentSize().height) + 0.5 * jetton->getContentSize().height);
    m_betZoneBackGround->addChild(jetton);
    jettonArray.pushBack(jetton);
}

void PokerChair::updateJetton(JettonSprite* jetton){
    this->removeAllJettons();
    this->addJetton(jetton);
}

void PokerChair::updateTotal(int value){
    betTotal = value;
    
    if (betTotal > 0) {
        betTotalLabel->setVisible(true);
        char mString[100];
        this->stringFromBetValue(mString, betTotal);
        
        betTotalLabel->setString(mString);
    }
    else {
        betTotalLabel->setVisible(false);
    }
}

void PokerChair::removeAllJettons(){
    for (JettonSprite* jetton : jettonArray) {
        jetton->removeFromParentAndCleanup(true);
    }
    
    jettonArray.clear();
    betTotal = 0;
    betPlayer = 0;
}

void PokerChair::calculatePokerType(){
    m_PokerType = PokerType_Default;
    if (pokerArray.size() == 2) {
        PokerSprite* pk0 = pokerArray.at(0);
        PokerSprite* pk1 = pokerArray.at(1);
        
        //计算对子
        if (pk0->getPoker_point() == pk1->getPoker_point()) {
            if ((pk0->getPoker_color() == PokerColor_JokerJunior || pk0->getPoker_color() == PokerColor_JokerSenior) && (pk1->getPoker_color() == PokerColor_JokerJunior || pk1->getPoker_color() == PokerColor_JokerSenior)) {
                m_PokerType = PokerType_Pair;
            }
            else if ((pk0->getPoker_color() == PokerColor_Spade || pk0->getPoker_color() == PokerColor_Club) && (pk1->getPoker_color() == PokerColor_Spade || pk1->getPoker_color() == PokerColor_Club)){
                m_PokerType = PokerType_Pair;
            }
            else if ((pk0->getPoker_color() == PokerColor_Heart || pk0->getPoker_color() == PokerColor_Diamond) && (pk1->getPoker_color() == PokerColor_Heart || pk1->getPoker_color() == PokerColor_Diamond)){
                m_PokerType = PokerType_Pair;
            }
        }
        
        if (m_PokerType != PokerType_Pair) {
            int poker_point0 = (pk0->getPoker_point() == PokerPoint_Joker) ? 5 : 10 * pk0->getPoker_point();
            int poker_point1 = (pk1->getPoker_point() == PokerPoint_Joker) ? 5 : 10 * pk1->getPoker_point();
            
            int poker_point = (poker_point0 + poker_point1) % 100;
            if (poker_point == 95) {
                m_PokerType = PokerType_9_Half;
            }
            else{
                m_PokerType = (PokerType )(PokerType_0 - (poker_point / 5));
            }
        }
    }
}

void PokerChair::showPokerType(){
    char mString[20];
    switch (m_PokerType) {
        case PokerType_Pair:{
            sprintf(mString,"对子");
        }
            break;
            
        case PokerType_9_Half:{
            sprintf(mString,"九点半");
        }
            break;
            
        case PokerType_9:{
            sprintf(mString,"九点");
        }
            break;
        case PokerType_8_Half:{
            sprintf(mString,"八点半");
        }
            break;
        case PokerType_8:{
            sprintf(mString,"八点");
        }
            break;
        case PokerType_7_Half:{
            sprintf(mString,"七点半");
        }
            break;
        case PokerType_7:{
            sprintf(mString,"七点");
        }
            break;
        case PokerType_6_Half:{
            sprintf(mString,"六点半");
        }
            break;
        case PokerType_6:{
            sprintf(mString,"六点");
        }
            break;
        case PokerType_5_Half:{
            sprintf(mString,"五点半");
        }
            break;
        case PokerType_5:{
            sprintf(mString,"五点");
        }
            break;
        case PokerType_4_Half:{
            sprintf(mString,"四点半");
        }
            break;
        case PokerType_4:{
            sprintf(mString,"四点");
        }
            break;
        case PokerType_3_Half:{
            sprintf(mString,"三点半");
        }
            break;
        case PokerType_3:{
            sprintf(mString,"三点");
        }
            break;
        case PokerType_2_Half:{
            sprintf(mString,"两点半");
        }
            break;
        case PokerType_2:{
            sprintf(mString,"两点");
        }
            break;
        case PokerType_1_Half:{
            sprintf(mString,"一点半");
        }
            break;
        case PokerType_1:{
            sprintf(mString,"一点");
        }
            break;
        case PokerType_0_Half:{
            sprintf(mString,"半点");
        }
            break;
        case PokerType_0:{
            sprintf(mString,"没点");
        }
            break;
            
        default:{
            sprintf(mString,"未知");
        }
            break;
    }
    pokerTypeLabel->setString(mString);
    pokerTypeLabel->setVisible(true);
}

void PokerChair::calculateSettlement(PokerChair* dealerChair){
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL){
        if (!m_Stabber->isVisible()) {
            if (dealerChair->m_PokerType <= m_PokerType) {
                //庄家赢
                if (dealerChair->m_PokerType <= PokerType_9) {
                    m_settlement.multiple = 3;
                }
                else if (dealerChair->m_PokerType <= PokerType_8) {
                    m_settlement.multiple = 2;
                }
                else {
                    m_settlement.multiple = 1;
                }
                
                m_settlement.winned = false;
                m_settlement.accounts = -m_settlement.multiple * betPlayer;
            }
            else {
                //闲家赢
                if (m_PokerType <= PokerType_9_Half) {
                    m_settlement.multiple = 9;
                }
                else if (m_PokerType <= PokerType_Pair) {
                    m_settlement.multiple = 6;
                }
                else if (m_PokerType <= PokerType_9) {
                    m_settlement.multiple = 3;
                }
                else if (m_PokerType <= PokerType_8) {
                    m_settlement.multiple = 2;
                }
                else {
                    m_settlement.multiple = 1;
                }
                
                m_settlement.winned = true;
                m_settlement.accounts = m_settlement.multiple * betPlayer;
            }
        }
    }
}

void PokerChair::calculateSettlementForStabber(PokerChair* dealerChair, int jetton){
    m_settlement.winned = (dealerChair->m_PokerType > m_PokerType);
    if (m_Stabber->isVisible()) {
        m_settlement.accounts = (m_settlement.winned ? jetton : -jetton);
    }
}

void PokerChair::showSettlement(){
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        pokerTypeLabel->setColor(m_settlement.winned ? Color3B::RED : Color3B::GREEN);
        char mString[100];
        if (m_settlement.accounts == 0) {
            settlementLabel->setColor(Color3B::WHITE);
            pokerTypeLabel->setColor(Color3B::WHITE);
            sprintf(mString,"无成绩");
        }
        else{
            settlementLabel->setColor(m_settlement.winned ? Color3B::RED : Color3B::GREEN);
            
            if (m_settlement.winned) {
                sprintf(mString,"%d倍 +%d",m_settlement.multiple, m_settlement.accounts);
            }
            else{
                sprintf(mString,"%d倍 %d",m_settlement.multiple, m_settlement.accounts);
            }
        }
        
        settlementLabel->setString(mString);
        settlementLabel->setVisible(true);
    }
    
}

void PokerChair::showSettlementForStabber(){
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        if (m_Stabber->isVisible()) {
            char mString[100];
            if (m_settlement.winned) {
                sprintf(mString,"刺 +%d", m_settlement.accounts);
            }
            else{
                sprintf(mString,"刺 %d", m_settlement.accounts);
            }
            
            settlementLabel->setColor(m_settlement.winned ? Color3B::RED : Color3B::GREEN);
            settlementLabel->setString(mString);
            settlementLabel->setVisible(true);
            
        }
        
        pokerTypeLabel->setColor(m_settlement.winned ? Color3B::RED : Color3B::GREEN);
    }
}

void PokerChair::showBeStabber(bool yn){
    if (m_BeStabberSprite != NULL) {
        m_BeStabberSprite->showBeStabber(yn);
    }
}

void PokerChair::showStabber(const char* imageName, const char* name, int jetton){
    if (m_Stabber != NULL) {
        m_Stabber->showStabber(imageName, name, jetton);
        m_Stabber->setVisible(true);
    }
}

void PokerChair::clearChair(){
    removeAllPokers();
    
    if (pokerTypeLabel->isVisible()) {
        pokerTypeLabel->setVisible(false);
    }
    
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        if (betTotalLabel->isVisible()) {
            betTotalLabel->setVisible(false);
        }
        
        if (betPlayerLabel->isVisible()) {
            betPlayerLabel->setVisible(false);
        }
        
        if (settlementLabel->isVisible()) {
            settlementLabel->setVisible(false);
        }
        
        if (m_BeStabberSprite->isVisible()) {
            m_BeStabberSprite->showBeStabber(false);
        }
        
        if (m_Stabber->isVisible()) {
            m_Stabber->setVisible(false);
        }
        pokerTypeLabel->setColor(Color3B::WHITE);
        settlementLabel->setColor(Color3B::WHITE);
        removeAllJettons();
    }
}

void PokerChair::setTouchCallBackFunc(Ref* target,SEL_CallFuncND callfun){
    m_touchListener = target;
    m_touchCallback = callfun;
}

void PokerChair::stringFromBetValue(char* mString, int betValue){
    if (betValue / 10000 > 0) {
        sprintf(mString,"%.2fw",betValue / 10000.0);
    }
    else{
        sprintf(mString,"%d",betValue);
    }
}


