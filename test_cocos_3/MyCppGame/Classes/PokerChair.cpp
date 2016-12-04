//
//  PokerChair.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "PokerChair.h"
#include "JettonSprite.h"

PokerChair::PokerChair():m_BankerSprite(NULL),m_betZoneBackGround(NULL),m_touchListener(NULL),m_touchCallback(NULL),betTotal(0),betPlayer(0){
    
}

PokerChair::~PokerChair(){
    CC_SAFE_RELEASE(m_betZoneBackGround);
    
}

bool PokerChair::init(){
    if (!LayerColor::init()) {
        return false;
    }
    
    m_settlement = new SettlementRef();
    m_settlement->autorelease();
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
    
    if (rect.containsPoint(locationInNode)){
        if (m_touchCallback && m_touchListener) {
            (m_touchListener->*m_touchCallback)(this);
        }
        
        return true;
    }
    
    return false;
}

void PokerChair::onTouchMoved(Touch* touch,Event* event){
    
}

void PokerChair::onTouchEnded(Touch* touch,Event* event){
    
}

void PokerChair::onEnter(){
    LayerColor::onEnter();
    
    pokerTypeLabel = Label::createWithTTF("", "fonts/微软简标宋.ttf", 12);
    pokerTypeLabel->setVisible(false);
    pokerTypeLabel->setPosition(0.5 * this->getContentSize().width, -0.05 * this->getContentSize().height);
    this->addChild(pokerTypeLabel);
    
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        background->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.5 * background->getContentSize().height);
        this->addChild(background);
        
        betTotalLabel = Label::createWithSystemFont("", "Arial", 20.0);
        betTotalLabel->setColor(Color3B::WHITE);
        betTotalLabel->enableShadow(Color4B::BLACK, Size(1, 1)); //阴影
        betTotalLabel->enableOutline(Color4B::WHITE, 1);             //轮廓
        betTotalLabel->setVisible(false);
        betTotalLabel->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.2 * background->getContentSize().height);
        this->addChild(betTotalLabel);
        
        betPlayerLabel = Label::createWithSystemFont("", "Arial", 20.0);
        betPlayerLabel->setColor(Color3B::YELLOW);
        betPlayerLabel->enableShadow(Color4B::BLACK, Size(1, 1)); //阴影
        betPlayerLabel->enableOutline(Color4B::YELLOW, 1);             //轮廓
        betPlayerLabel->setVisible(false);
        betPlayerLabel->setPosition(0.5 * this->getContentSize().width, this->getContentSize().height - 0.8 * background->getContentSize().height);
        this->addChild(betPlayerLabel);
        
        settlementLabel = Label::createWithTTF("", "fonts/微软简标宋.ttf", 12);
        settlementLabel->setVisible(false);
        settlementLabel->setPosition(0.5 * this->getContentSize().width, -0.2 * this->getContentSize().height);
        this->addChild(settlementLabel);
        
        //触摸响应注册
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(PokerChair::onTouchBegan, this);//触摸开始
        listener->onTouchMoved = CC_CALLBACK_2(PokerChair::onTouchMoved, this);//触摸移动
        listener->onTouchEnded = CC_CALLBACK_2(PokerChair::onTouchEnded, this);//触摸结束
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, background);//注册分发器
    }
    else{
        pokerTypeLabel->setPosition(m_BankerSprite->getPositionX(), 0.3 * this->getContentSize().height);
    }
}

void PokerChair::onExit(){
    //移除触摸响应
    _eventDispatcher->removeEventListenersForTarget(this);
    LayerColor::onExit();
}

PokerChair* PokerChair::create(const char* betZoneImage,Size size){
    PokerChair* layer = PokerChair::create();
    if (betZoneImage) {
        layer->setBetZoneBackGround(QLImageSprite::create(betZoneImage, size));
    }
    return layer;
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
    
    int index = 0;
    float scale = 0.7;
    size_t count = pokerArray.size();
    //更新位置
    for (PokerSprite* pk : pokerArray) {
        pk->setPositionX(m_point.x - 0.5 * pk->getContentSize().width * (count - (count - 1) * scale) + index * pk->getContentSize().width * (1 - scale) + 0.5 * pk->getContentSize().width);
        ++index;
    }
}

void PokerChair::addJetton(JettonSprite* jetton){
    betTotal += jetton->getJettonValue();
    
    betTotalLabel->setVisible(true);
    betTotalLabel->setString(this->stringFromBetValue(betTotal));
    if (jetton->isPlayer) {
        betPlayer += jetton->getJettonValue();
        betPlayerLabel->setVisible(true);
        betPlayerLabel->setString(this->stringFromBetValue(betPlayer));
    }
    
    jetton->setPosition(0.1 * getRandomNumber(0, 10) * (m_betZoneBackGround->getContentSize().width - jetton->getContentSize().width) + 0.5 * jetton->getContentSize().width, 0.1 * getRandomNumber(0, 10) * (m_betZoneBackGround->getContentSize().height - jetton->getContentSize().height) + 0.5 * jetton->getContentSize().height);
    m_betZoneBackGround->addChild(jetton);
    jettonArray.pushBack(jetton);
}

void PokerChair::removeAllJettons(){
    for (JettonSprite* jetton : jettonArray) {
        jetton->removeFromParentAndCleanup(true);
    }
    
    jettonArray.clear();
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
    switch (m_PokerType) {
        case PokerType_Pair:{
            pokerTypeLabel->setString("对子");
        }
            break;
            
        case PokerType_9_Half:{
            pokerTypeLabel->setString("9点半");
        }
            break;
            
        case PokerType_9:
        case PokerType_8_Half:
        case PokerType_8:
        case PokerType_7_Half:
        case PokerType_7:
        case PokerType_6_Half:
        case PokerType_6:
        case PokerType_5_Half:
        case PokerType_5:
        case PokerType_4_Half:
        case PokerType_4:
        case PokerType_3_Half:
        case PokerType_3:
        case PokerType_2_Half:
        case PokerType_2:
        case PokerType_1_Half:
        case PokerType_1:
        case PokerType_0_Half:
        case PokerType_0:{
            char* mString = new char[20];
            if ((PokerType_0 - m_PokerType) % 2 == 0) {
                sprintf(mString,"%d点",(PokerType_0 - m_PokerType) / 2);
            }
            else {
                sprintf(mString,"%d点半",(PokerType_0 - m_PokerType) / 2);
            }
            pokerTypeLabel->setString(mString);
        }
            break;
            
        default:{
            pokerTypeLabel->setString("未知");
        }
            break;
    }
    pokerTypeLabel->setVisible(true);
}

void PokerChair::calculateSettlement(PokerChair* dealerChair){
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL){
        
        if (dealerChair->m_PokerType <= this->m_PokerType) {
            //庄家赢
            
            if (dealerChair->m_PokerType <= PokerType_9) {
                this->m_settlement->multiple = 3;
            }
            else if (dealerChair->m_PokerType <= PokerType_8) {
                this->m_settlement->multiple = 2;
            }
            else {
                this->m_settlement->multiple = 1;
            }
            
            m_settlement->winned = false;
            m_settlement->accounts = -m_settlement->multiple * betPlayer;
        }
        else {
            //闲家赢
            
            if (this->m_PokerType <= PokerType_9_Half) {
                this->m_settlement->multiple = 9;
            }
            else if (this->m_PokerType <= PokerType_Pair) {
                this->m_settlement->multiple = 6;
            }
            else if (this->m_PokerType <= PokerType_9) {
                this->m_settlement->multiple = 3;
            }
            else if (this->m_PokerType <= PokerType_8) {
                this->m_settlement->multiple = 2;
            }
            else {
                this->m_settlement->multiple = 1;
            }
            
            m_settlement->winned = true;
            m_settlement->accounts = m_settlement->multiple * betPlayer;
        }
        
//        log("牌型:%d 倍数：%d 赔付额%d",this->m_PokerType ,m_settlement->multiple, m_settlement->accounts);
    }
    else{
        CCLOG("no******************");
    }
}

void PokerChair::showSettlement(){
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        settlementLabel->setVisible(true);
        pokerTypeLabel->setColor(m_settlement->winned ? Color3B::RED : Color3B::GREEN);
        if (betPlayer <= 0) {
            settlementLabel->setColor(Color3B::WHITE);
            settlementLabel->setString("无成绩");
        }
        else{
            settlementLabel->setColor(m_settlement->winned ? Color3B::RED : Color3B::GREEN);
            
            char* mString = new char[100];
            if (m_settlement->winned) {
                sprintf(mString,"%d倍 +%d",m_settlement->multiple, m_settlement->accounts);
            }
            else{
                sprintf(mString,"%d倍 %d",m_settlement->multiple, m_settlement->accounts);
            }
            settlementLabel->setString(mString);
        }
    }
    
}

void PokerChair::clearChair(){
    pokerArray.clear();
    pokerTypeLabel->setVisible(false);
    
    QLImageSprite* background = getBetZoneBackGround();
    if (background != NULL) {
        betTotalLabel->setVisible(false);
        betPlayerLabel->setVisible(false);
        settlementLabel->setVisible(false);
        betTotal = 0;
        betPlayer = 0;
        removeAllJettons();
    }
}

void PokerChair::setTouchCallBackFunc(Ref* target,SEL_CallFuncN callfun){
    m_touchListener = target;
    m_touchCallback = callfun;
}

char* PokerChair::stringFromBetValue(int betValue){
    char* mString = new char[100];
    if (betValue / 10000 > 0) {
        sprintf(mString,"%.2fw",betValue / 10000.0);
    }
    else{
        sprintf(mString,"%d",betValue);
    }
    
    return mString;
}


