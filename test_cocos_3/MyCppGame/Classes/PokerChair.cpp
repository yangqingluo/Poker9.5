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
    
    pokerTypeLabel = Label::createWithTTF("对子", "fonts/STKaiti.ttf", 14);
    //        pokerTypeLabel->setVisible(false);
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
        
        settlementLabel = Label::createWithTTF("2倍 +15000", "fonts/STKaiti.ttf", 14);
//        settlementLabel->setVisible(false);
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
            m_BankerSprite->setPosition(this->getContentSize().width - 0.6 * m_BankerSprite->getBoundingBox().size.width, this->getContentSize().height - 0.5 * m_BankerSprite->getBoundingBox().size.height);
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
    m_betZoneBackGround->addChild(jetton,0,99);
}

void PokerChair::removeAllJettons(){
    m_betZoneBackGround->removeChildByTag(99);
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


