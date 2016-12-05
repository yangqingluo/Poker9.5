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
        
        Sprite* BG = Sprite::create("poker/poker_back.png");
        BG->setScale(pk->getContentSize().width / BG->getContentSize().width);
        BG->setPosition(pk->getContentSize().width / 2, pk->getContentSize().height / 2);
        pk->addChild(BG);
        pk->bgSprite = BG;
        
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
    
    char Icon[30] = {"poker/poker_back.png"};
    if (showFront) {
        sprintf(Icon, "poker/poker_%d_%d.png",p_color,p_point);
    }
    
    if (animated) {
        auto scaleSmall = ScaleTo::create(0.2, 0, 1);
        auto scaleBig = ScaleTo::create(0.2, 1, 1);
        CallFunc* func1 = CallFunc::create([=]{
            bgSprite->setTexture(Icon);
        });
        
        DelayTime* delay = DelayTime::create(doneDelay);
        
        CallFunc* func2 = CallFunc::create([=]{
            this->showedPoker();
        });
        
        this->runAction(Sequence::create(scaleSmall, func1, scaleBig, delay, func2, NULL));
    }
    else{
        bgSprite->setTexture(Icon);
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

