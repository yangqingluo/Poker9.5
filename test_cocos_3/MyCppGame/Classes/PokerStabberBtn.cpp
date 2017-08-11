//
//  PokerStabberBtn.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/12/15.
//
//

#include "PokerStabberBtn.h"

PokerStabberBtn::PokerStabberBtn(){
    
}

PokerStabberBtn::~PokerStabberBtn(){
    
}

bool PokerStabberBtn::init(){
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    return true;
}

void PokerStabberBtn::onEnter(){
    LayerColor::onEnter();
    
    float radius = MIN(this->getContentSize().width, this->getContentSize().height);
    showSprite = JettonChosenSprite::create("images/btn_stabber.png", Size(radius, radius));
    showSprite->setPosition(0.5 * this->getContentSize().width, 0.5 * this->getContentSize().height);
    this->addChild(showSprite);
}
void PokerStabberBtn::onExit(){
    LayerColor::onExit();
}

PokerStabberBtn* PokerStabberBtn::create(const Color4B color, Size size){
    PokerStabberBtn* layer = PokerStabberBtn::create();
    layer->initWithColor(color, size.width, size.height);
    
    return layer;
}

void PokerStabberBtn::showBeStabber(bool yn){
    if (yn != this->isVisible()) {
        this->setVisible(yn);
        showSprite->setSelected(yn);
    }
}




