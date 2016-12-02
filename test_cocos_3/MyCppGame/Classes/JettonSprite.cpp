#include "JettonSprite.h"

JettonSprite::JettonSprite():m_value(10),m_isSelected(false){
    
}

JettonSprite::~JettonSprite(){
    
}

JettonSprite* JettonSprite::create(int value, Size size){
    JettonSprite* sp = new JettonSprite();
    if (sp && sp->init()){
        sp->setContentSize(size);
        sp->m_value = value;
        sp->autorelease();
        
        Sprite* BG = Sprite::create("jetton/jetton_bg.png");
        BG->setScale(sp->getContentSize().width / BG->getContentSize().width);
        BG->setPosition(sp->getContentSize().width / 2, sp->getContentSize().height / 2);
        BG->setVisible(false);
        sp->addChild(BG);
        sp->bgSprite = BG;
        
        if (sp->isValidValue(value)) {
            char Icon[30] = {0};
            sprintf(Icon, "jetton/jetton_%d.png",value);
            sp->valueSprite = Sprite::create(Icon);
            sp->valueSprite->setScale(sp->getContentSize().width / sp->valueSprite->getContentSize().width);
            sp->valueSprite->setPosition(BG->getPosition());
            sp->addChild(sp->valueSprite);
        }
        
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

void JettonSprite::setSelected(bool select){
    if (select == m_isSelected) {
        return;
    }
    
    m_isSelected = select;
    if (select) {
        Blink* blink = Blink::create(INT_MAX, 2 * INT_MAX);
        blink->setTag(6);
        this->bgSprite->runAction(blink);
    }
    else{
        this->bgSprite->stopActionByTag(6);
    }
}
