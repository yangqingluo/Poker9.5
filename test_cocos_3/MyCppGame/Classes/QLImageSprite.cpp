#include "QLImageSprite.h"

USING_NS_CC;

QLImageSprite::QLImageSprite(){
    
}

QLImageSprite::~QLImageSprite(){
    
}

QLImageSprite* QLImageSprite::create(const char* backgroudImage, cocos2d::Size size){
    QLImageSprite* sp = new QLImageSprite();
    if (sp && sp->init()){
        sp->setContentSize(size);
        sp->autorelease();
        
        if (backgroudImage) {
            sp->bgSprite = Sprite::create(backgroudImage);
            sp->bgSprite->setScale(sp->getContentSize().width / sp->bgSprite->getContentSize().width, sp->getContentSize().height / sp->bgSprite->getContentSize().height);
            sp->bgSprite->setPosition(sp->getContentSize().width / 2, sp->getContentSize().height / 2);
            sp->addChild(sp->bgSprite);
        }
        
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return sp;
}

void QLImageSprite::setBackgroundImage(const char* backgroudImage){
    this->bgSprite->setTexture(backgroudImage);
}
