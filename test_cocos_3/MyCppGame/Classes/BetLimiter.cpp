//
//  BetLimiter.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "BetLimiter.h"
using namespace ui;

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
    
    layer->m_type = type;
    float radius = 0.8 * size.height;
    float positionY = 0.5 * layer->getContentSize().height;
    float edgeScale = 0.1;
    float xOrigin = 0.0;
    
    if (type == BetType_Addition) {
        positionY = 0.4 * layer->getContentSize().height;
        xOrigin = 0.5 * (size.width - ((count + 2) + (count + 1) * edgeScale) * radius);
        
        auto ltf = Label::create("", "", 8);
        ltf->setColor(Color3B::WHITE);
        ltf->setPosition(0.5 * size.width, size.height - 0.8 * ltf->getContentSize().height);
        layer->addChild(ltf);
        layer->m_label = ltf;
    }
    
    for (int i = 0; i < count; i++) {
        int value = jettonValueArray[i];
        auto jetton = JettonChosenSprite::create(value, Size(radius, radius));
        jetton->setPosition(xOrigin + (0.5 + (1 + edgeScale) * i) * jetton->getContentSize().width, positionY);
        jetton->setCanTouch(true);
        jetton->setTouchCallBackFunc(layer, callfuncN_selector(BetLimiter::touchedJettonCallback));
        layer->addChild(jetton, 0, 10 + i);
        if (i == 0) {
            layer->touchedJettonCallback(jetton);
        }
        else if (i == count - 1) {
            if (type == BetType_Addition) {
                layer->m_label->setPosition(xOrigin + (0.5 + (1 + edgeScale) * (i + 1)) * jetton->getContentSize().width, positionY);
                
                auto btn_clearJetton = Button::create("jetton/jetton_clear.png","jetton/jetton_clear.png");
                btn_clearJetton->setScale(radius / btn_clearJetton->getContentSize().height);
                btn_clearJetton->setPosition(Vec2(xOrigin + (0.5 + (1 + edgeScale) * (i + 2)) * jetton->getContentSize().width, positionY));
                btn_clearJetton->addTouchEventListener(CC_CALLBACK_2(BetLimiter::touchEvent, layer));
                btn_clearJetton->setTag(10);
                layer->addChild(btn_clearJetton);
            }
        }
    }
    
    return layer;
}

void BetLimiter::touchedJettonCallback(Node* pSender){
    auto jetton = (JettonChosenSprite* )pSender;
    
    if (selectedJetton != NULL) {
        selectedJetton->setSelected(false);
    }
    
    jetton->setSelected(true);
    selectedJetton = jetton;
    
    if (m_type == BetType_Addition) {
        m_value += jetton->getJettonValue();
        this->showValueLabel();
    }
    else {
        m_value = jetton->getJettonValue();
    }
}

void BetLimiter::showValueLabel(){
    char m_string[50] = {0};
    sprintf(m_string, "=%d", m_value);
    
    m_label->setString(m_string);
}

void BetLimiter::touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 10:{
                    m_value = 0;
                    this->showValueLabel();
                }
                    break;
                    
                default:
                    break;
            }
            break;
            
        case Widget::TouchEventType::CANCELED:{
            
        }
            break;
            
        default:
            break;
    }
}

