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
    
    layer->infoLabel = Label::createWithTTF("", "fonts/微软简标宋.ttf", 6);
    layer->infoLabel->setColor(Color3B(255, 215, 0));
    layer->infoLabel->setPosition(0.5 * size.width, 0.2 * size.height);
    layer->addChild(layer->infoLabel);
    
    return layer;
}

void PokerStabber::showStabber(const char* imageName, const char* name, int jetton){
    head->setTexture(imageName);
    
    char mString[100];
    sprintf(mString, "刺:%s\n筹码:%d",name, jetton);
    infoLabel->setString(mString);
}




