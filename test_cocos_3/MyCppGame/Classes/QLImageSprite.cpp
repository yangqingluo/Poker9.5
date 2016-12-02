#include "QLImageSprite.h"

QLImageSprite::QLImageSprite(){
    
}

QLImageSprite::~QLImageSprite(){
    
}

QLImageSprite* QLImageSprite::create(const char* backgroudImage, Size size){
    QLImageSprite* sp = new QLImageSprite();
    if (sp && sp->init()){
        sp->setContentSize(size);
        sp->autorelease();
        
        if (backgroudImage) {
            Sprite* BG = Sprite::create(backgroudImage);
            BG->setScale(sp->getContentSize().width / BG->getContentSize().width);
            BG->setPosition(sp->getContentSize().width / 2, sp->getContentSize().height / 2);
            BG->setVisible(false);
            sp->addChild(BG);
            sp->bgSprite = BG;
        }
        
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return sp;
}

