#include "PokerSprite.h"

PokerSprite::PokerSprite():p_isSelected(false),p_isFront(false),m_callbackListener(NULL),m_callback(NULL){
    
}

PokerSprite::~PokerSprite(){
    
}

PokerSprite* PokerSprite::create(PokerColor color, PokerPoint point, Size size){
    PokerSprite* pk = new PokerSprite();
    if (pk && pk->init()){
        pk->setContentSize(size);
        pk->p_color = color;
        pk->p_point = point;
        pk->autorelease();
        
        pk->bgSprite = Sprite::create("poker.png", Rect(2 * pkWidth, 4 * pkHeight, pkWidth, pkHeight));
        pk->bgSprite->setScale(pk->getContentSize().width / pk->bgSprite->getContentSize().width);
        pk->bgSprite->setPosition(pk->getContentSize().width / 2, pk->getContentSize().height / 2);
        pk->addChild(pk->bgSprite);
        
        return pk;
    }
    
    CC_SAFE_DELETE(pk);
    return pk;
}

void PokerSprite::onEnter(){
    Sprite::onEnter();
    
}
void PokerSprite::onExit(){
    Sprite::onExit();
}

void PokerSprite::showPokerAnimated(bool showFront, bool animated, float doneDelay){
    if (showFront == p_isFront) {
        return;
    }
    
    Rect rect = Rect((p_point - 1) * pkWidth, (p_color - 1) * pkHeight, pkWidth, pkHeight);
    if (p_point == PokerPoint_Joker) {
        rect = Rect((p_color == PokerColor_JokerJunior ? 0 : 1) * pkWidth, 4 * pkHeight, pkWidth, pkHeight);
    }
    
    if (animated) {
        auto scaleSmall = ScaleTo::create(0.2, 0, 1);
        auto scaleBig = ScaleTo::create(0.2, 1, 1);
        CallFunc* func1 = CallFunc::create([=]{
            bgSprite->setTextureRect(rect);
        });
        
        DelayTime* delay = DelayTime::create(doneDelay);
        
        CallFunc* func2 = CallFunc::create([=]{
            this->showedPoker();
        });
        
        this->runAction(Sequence::create(scaleSmall, func1, scaleBig, delay, func2, NULL));
    }
    else{
        bgSprite->setTextureRect(rect);
        this->showedPoker();
    }
}

void PokerSprite::setSelected(bool select){
    if (select == p_isSelected) {
        return;
    }

    p_isSelected = select;
    if (select) {
        this->setPosition(Vec2(getPositionX(),getPositionY() + 10));
    }
    else{
        this->setPosition(Vec2(getPositionX(),getPositionY() - 10));
    }
}

void PokerSprite::showedPoker(){
    p_isFront = !p_isFront;
    if (m_callback && m_callbackListener) {
        (m_callbackListener->*m_callback)(this);
    }
}

void PokerSprite::removeAction(float delta){
    this->removeFromParent();
}

void PokerSprite::setCallBackFunc(Ref*target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;
}

