//
//  PokerStabber.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/12/15.
//
//

#include "PokerStabber.h"

PokerStabber::PokerStabber(){
    
}

PokerStabber::~PokerStabber(){
    
}

bool PokerStabber::init(){
    if ( !LayerColor::init() )
    {
        return false;
    }
    
    return true;
}

void PokerStabber::onEnter(){
    LayerColor::onEnter();
    
}
void PokerStabber::onExit(){
    LayerColor::onExit();
}

PokerStabber* PokerStabber::create(const char* imageName, Size size){
    PokerStabber* layer = PokerStabber::create();
    layer->setContentSize(size);
    
    layer->head = Sprite::create(imageName);
    layer->head->setScale(0.5 * size.height / layer->head->getContentSize().height);
    layer->head->setPosition(0.5 * size.width, 0.75 * size.height);
    layer->addChild(layer->head);
    
    layer->infoLabel = Label::createWithTTF("刺：测试玩家\n筹码：20000", "fonts/STKaiti.ttf", 6);
    layer->infoLabel->setColor(Color3B::WHITE);
    layer->infoLabel->setPosition(0.5 * size.width, 0.2 * size.height);
    layer->addChild(layer->infoLabel);
    
    return layer;
}
