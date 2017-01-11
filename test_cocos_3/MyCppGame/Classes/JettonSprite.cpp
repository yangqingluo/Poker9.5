#include "JettonSprite.h"

JettonSprite::JettonSprite():m_value(10),isPlayer(false){
    
}

JettonSprite::~JettonSprite(){
    
}

JettonSprite* JettonSprite::create(int value, Size size){
    JettonSprite* sp = new JettonSprite();
    if (sp && sp->init()){
        sp->setContentSize(size);
        sp->m_value = value;
        sp->autorelease();
        
        if (sp->isValidValue(value)) {
            char Icon[30] = {0};
            sprintf(Icon, "jetton/jetton_%d.png",value);
            sp->valueSprite = Sprite::create(Icon);
            sp->valueSprite->setScale(sp->getContentSize().width / sp->valueSprite->getContentSize().width);
            sp->valueSprite->setPosition(sp->getContentSize().width / 2, sp->getContentSize().height / 2);
            sp->addChild(sp->valueSprite);
        }
        
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return sp;
}

JettonSprite* JettonSprite::createWithEmpty(int value, Size size){
    JettonSprite* sp = new JettonSprite();
    if (sp && sp->init()){
        sp->setContentSize(size);
        sp->m_value = value;
        sp->autorelease();
        
        int validValue = sp->validValueInEmpty(value);
        
        char Icon[30] = {0};
        sprintf(Icon, "jetton/jetton_empty_%d.png", validValue);
        sp->valueSprite = Sprite::create(Icon);
        sp->valueSprite->setScale(sp->getContentSize().width / sp->valueSprite->getContentSize().width);
        sp->valueSprite->setPosition(sp->getContentSize().width / 2, sp->getContentSize().height / 2);
        sp->addChild(sp->valueSprite);
        
        
        char m_string[30] = {0};
        sp->stringFromJettonValue(m_string, value);
        auto ltf = Label::createWithTTF(m_string, "fonts/STKaiti.ttf", 6);
        ltf->setColor(Color3B::WHITE);
        ltf->setPosition(sp->valueSprite->getPosition());
        ltf->setDimensions(sp->getContentSize().width, sp->getContentSize().height);
        ltf->setHorizontalAlignment(TextHAlignment::CENTER);
        ltf->setVerticalAlignment(TextVAlignment::CENTER);
        sp->addChild(ltf);
        
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return sp;
}

bool JettonSprite::isValidValue(int value){
    int arr[9] = {10,20,50,100,200,500,1000,2000,5000};
    for (int i = 0; i < 9; i++) {
        if (value == arr[i]) {
            return true;
        }
    }
    
    return false;
}

int JettonSprite::validValueInEmpty(int value){
    int valid = 10;
    
    int arr[9] = {10,20,50,100,200,500,1000,2000,5000};
    for (int i = 9; i > 0; i--) {
        if (value > arr[i - 1]) {
            valid = arr[i - 1];
            break;
        }
    }
    
    
    return valid;
}

void JettonSprite::stringFromJettonValue(char* mString, int value){
    if (value / 10000 > 0) {
        sprintf(mString,"%.2fw",value / 10000.0);
    }
    else{
        sprintf(mString,"%d",value);
    }
}
