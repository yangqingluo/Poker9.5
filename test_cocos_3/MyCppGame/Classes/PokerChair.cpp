//
//  PokerChair.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "PokerChair.h"

PokerChair::PokerChair():m__sfBackGround(NULL){
    
}

PokerChair::~PokerChair(){
    CC_SAFE_RELEASE(m__sfBackGround);
}

bool PokerChair::init(){
    if (!LayerColor::init()) {
        return false;
    }
    
    
    //设置弹出层的颜色，指定为淡灰色
    setColor(Color3B::GRAY);
    setOpacity(0x50);
    
    return true;
}

PokerChair* PokerChair::create(const char* backgoundImage,Size size){
    PokerChair* layer = PokerChair::create();
    
    layer->setSpriteBackGround(Sprite::create(backgoundImage));
    layer->m_dialogContentSize = size;
    
    return layer;
}

bool PokerChair::onTouchBegan(Touch* touch,Event* event){
    return true;
}

void PokerChair::onTouchMoved(Touch* touch,Event* event){
    
}

void PokerChair::onTouchEnded(Touch* touch,Event* event){
    
}

void PokerChair::onEnter(){
    LayerColor::onEnter();
    
    Sprite* background = getSpriteBackGround();
    if (background != NULL) {
        background->setPosition(0.1 * this->getContentSize().width, 0.1 * this->getContentSize().height);
        background->setScale(0.1 * this->getContentSize().width / background->getContentSize().width);
        this->addChild(background);
    }
    
}

void PokerChair::onExit(){
    LayerColor::onExit();
    
}

void PokerChair::setHighlighted(bool yn){
    setColor(yn ? Color3B::BLACK : Color3B::GRAY);
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
