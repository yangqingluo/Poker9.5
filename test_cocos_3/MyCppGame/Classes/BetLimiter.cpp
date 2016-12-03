//
//  BetLimiter.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "BetLimiter.h"
#include "JettonSprite.h"

BetLimiter::BetLimiter():m_value(0),selectedJetton(NULL){
    
}

BetLimiter::~BetLimiter(){
    
}

bool BetLimiter::init(){
    if (!LayerColor::init()) {
        return false;
    }
    
    return true;
}

void BetLimiter::onEnter(){
    LayerColor::onEnter();
    
    
}

void BetLimiter::onExit(){
    LayerColor::onExit();
}

BetLimiter* BetLimiter::create(const int* jettonValueArray, size_t count, Size size){
    BetLimiter* layer = BetLimiter::create();
    layer->setContentSize(size);
    for (int i = 0; i < count; i++) {
        int value = jettonValueArray[i];
        JettonSprite* jetton = JettonSprite::create(value, Size(size.height, size.height));
        jetton->setPosition((0.5 + 1.2 * i) * jetton->getContentSize().width, 0.5 * layer->getContentSize().height);
        jetton->setCanTouch(true);
        jetton->setTouchCallBackFunc(layer, callfuncN_selector(BetLimiter::touchedJettonCallback));
        layer->addChild(jetton, 0, 10 + i);
        if (i == 0) {
            layer->touchedJettonCallback(jetton);
        }
    }
    
    return layer;
}

void BetLimiter::touchedJettonCallback(Node* pSender){
    JettonSprite* jetton = (JettonSprite* )pSender;
    
    if (selectedJetton == jetton) {
        return;
    }
    
    if (selectedJetton != NULL) {
        selectedJetton->setSelected(false);
    }
    
    jetton->setSelected(true);
    m_value = jetton->getJettonValue();
    selectedJetton = jetton;
}





