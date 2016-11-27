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
    setOpacity(128);
    
    return true;
}

PokerChair* PokerChair::create(const char* backgoundImage,Size dialogSize){
    //创建弹出对话框，指定背景图和大小。
    
    PokerChair* layer = PokerChair::create();
    
    layer->setSpriteBackGround(Sprite::create(backgoundImage));
    layer->m_dialogContentSize = dialogSize;
    
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
    background->setPosition(Vec2(this->getContentSize().width / 2, this->getContentSize().height / 2));
    this->addChild(background,0,0);
}

void PokerChair::onExit(){
    LayerColor::onExit();
    
}
