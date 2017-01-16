//
//  BetLimiter.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "BetLimiter.h"

BetLimiter::BetLimiter():m_value(0), m_type(BetType_Default), selectedJetton(NULL){
    
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

BetLimiter* BetLimiter::create(const int* jettonValueArray, size_t count, Size size, BetType type){
    BetLimiter* layer = BetLimiter::create();
    layer->setContentSize(size);
    
    if (type == BetType_Addition) {
        auto ltf = Label::createWithTTF("", "fonts/STKaiti.ttf", 12);
        ltf->setColor(Color3B::WHITE);
        ltf->setPosition(0.5 * size.width, size.height - 0.3 * ltf->getContentSize().height);
        layer->addChild(ltf);
        layer->m_label = ltf;
    }
    
    for (int i = 0; i < count; i++) {
        int value = jettonValueArray[i];
        auto jetton = JettonChosenSprite::create(value, Size(size.height, size.height));
        jetton->setPosition((0.5 + 1.1 * i) * jetton->getContentSize().width, 0.5 * layer->getContentSize().height);
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
    auto jetton = (JettonChosenSprite* )pSender;
    
    if (m_type == BetType_Addition) {
        m_value += jetton->getJettonValue();
        this->showValueLabel();
    }
    else {
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
}

void BetLimiter::showValueLabel(){
    char m_string[50] = {0};
    sprintf(m_string, "%d", m_value);
    
    m_label->setString(m_string);
}



