//
//  BetSlider.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "BetSlider.h"

BetSlider::BetSlider():m_value(0),selectedJetton(NULL){
    
}

BetSlider::~BetSlider(){
    
}

bool BetSlider::init(){
    if (!LayerColor::init()) {
        return false;
    }
    
//    setColor(Color3B::RED);
//    setOpacity(0x50);
    
    return true;
}

void BetSlider::onEnter(){
    LayerColor::onEnter();
    
    
}

void BetSlider::onExit(){
    LayerColor::onExit();
}

BetSlider* BetSlider::create(int min, int max, Size size){
    BetSlider* layer = BetSlider::create();
    layer->setContentSize(size);
    
    layer->m_slider = ControlSlider::create("images/slider_jd.png", "images/slider_bg.png", "images/slider_hk.png");
    layer->m_slider->setPosition(0.5 * layer->m_slider->getContentSize().width, 0.5 * layer->m_slider->getContentSize().height);
    layer->m_slider->setMinimumValue(0);
    layer->m_slider->setMaximumValue(max);
    layer->m_slider->setMinimumAllowedValue(min);
    layer->addChild(layer->m_slider);
    
    auto ltf = Label::createWithTTF("下注数目：", "fonts/STKaiti.ttf", 12);
    ltf->setColor(Color3B::WHITE);
    ltf->setPosition(layer->m_slider->getPositionX(), size.height - 0.3 * ltf->getContentSize().height);
    layer->addChild(ltf);
    layer->m_label = ltf;
    
    layer->m_slider->addTargetWithActionForControlEvents(layer, cccontrol_selector(BetSlider::sliderChangerCallBack), Control::EventType::VALUE_CHANGED);
    layer->m_slider->setValue(min);
    return layer;
}

void BetSlider::touchedJettonCallback(Node* pSender){
    auto jetton = (JettonChosenSprite* )pSender;
    
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

void BetSlider::sliderChangerCallBack(Ref* pSender, Control::EventType type){
    m_value = m_slider->getValue();
    
    char content[200] = {0};
    sprintf(content, "下注数目：%d", m_value);
    m_label->setString(content);
}

void BetSlider::updateMinMax(int min, int max){
    m_slider->setMinimumValue(0);
    m_slider->setMaximumValue(max);
    
    m_slider->setMinimumAllowedValue(min);
    m_slider->setMaximumAllowedValue(max / 3);
    if (m_slider->getValue() < min) {
        m_slider->setValue(min);
    }
    else if (m_slider->getValue() > max / 3) {
        m_slider->setValue(max / 3);
    }
}



