#include "JettonChosenSprite.h"

JettonChosenSprite::JettonChosenSprite():m_value(10),m_isSelected(false),m_canTouch(false),m_touchListener(NULL),m_touchCallback(NULL),isPlayer(false){
    
}

JettonChosenSprite::~JettonChosenSprite(){
    
}

bool JettonChosenSprite::onTouchBegan(Touch *touch, Event *event){
    auto target = static_cast<Sprite* >(event->getCurrentTarget());//获取的当前触摸的目标
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
    Rect rect = Rect(0, 0, s.width, s.height);
    
    if (rect.containsPoint(locationInNode) && m_canTouch){
        if (m_touchCallback && m_touchListener) {
            (m_touchListener->*m_touchCallback)(this);
        }
        
        return true;
    }
    
    return false;
}

void JettonChosenSprite::onEnter(){
    Sprite::onEnter();
    
    if (m_canTouch) {
        //触摸响应注册
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = CC_CALLBACK_2(JettonChosenSprite::onTouchBegan, this);//触摸开始
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);//注册分发器
    }
}

void JettonChosenSprite::onExit(){
    if (m_canTouch) {
        getEventDispatcher()->removeEventListenersForTarget(this);
    }
    Sprite::onExit();
}

JettonChosenSprite* JettonChosenSprite::create(int value, Size size){
    JettonChosenSprite* sp = new JettonChosenSprite();
    if (sp && sp->init()){
        sp->setContentSize(size);
        sp->m_value = value;
        sp->autorelease();
        
        sp->bgSprite = Sprite::create("jetton/jetton_bg.png");
        sp->bgSprite->setScale(sp->getContentSize().width / sp->bgSprite->getContentSize().width);
        sp->bgSprite->setPosition(sp->getContentSize().width / 2, sp->getContentSize().height / 2);
        sp->bgSprite->setVisible(false);
        sp->addChild(sp->bgSprite);
        
        if (sp->isValidValue(value)) {
            char Icon[30] = {0};
            sprintf(Icon, "jetton/jetton_%d.png",value);
            sp->valueSprite = Sprite::create(Icon);
            sp->valueSprite->setScale(sp->getContentSize().width / sp->valueSprite->getContentSize().width);
            sp->valueSprite->setPosition(sp->bgSprite->getPosition());
            sp->addChild(sp->valueSprite);
        }
        
        return sp;
    }
    
    CC_SAFE_DELETE(sp);
    return sp;
}

bool JettonChosenSprite::isValidValue(int value){
    int arr[9] = {10,20,50,100,200,500,1000,2000,5000};
    for (int i = 0; i < 9; i++) {
        if (value == arr[i]) {
            return true;
        }
    }
    
    return false;
}

void JettonChosenSprite::setSelected(bool select){
    if (select == m_isSelected) {
        return;
    }
    
    m_isSelected = select;
    bgSprite->setVisible(select);
    if (select) {
        Blink* blink = Blink::create(MAX_INPUT, 2 * MAX_INPUT);
        blink->setTag(6);
        this->bgSprite->runAction(blink);
    }
    else{
        this->bgSprite->stopActionByTag(6);
    }
}

void JettonChosenSprite::setTouchCallBackFunc(Ref* target,SEL_CallFuncN callfun){
    m_touchListener = target;
    m_touchCallback = callfun;
}
