#include "JettonSprite.h"

JettonSprite::JettonSprite():m_value(10){
    
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
            sp->valueSprite->setScale(0.9 * sp->getContentSize().width / sp->valueSprite->getContentSize().width);
            sp->setPosition(BG->getPosition());
            sp->addChild(sp->valueSprite);
        }
        
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return sp;
}

bool JettonSprite::isValidValue(int value){
    int arr[12] = {10,20,50,100,200,500,1000,2000,5000,10000,20000,50000};
    for (int i = 0; i < 12; i++) {
        if (value == arr[i]) {
            return true;
        }
    }
    
    return false;
}
