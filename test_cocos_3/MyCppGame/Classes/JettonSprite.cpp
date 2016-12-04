#include "JettonSprite.h"

JettonSprite::JettonSprite():m_value(10),m_isSelected(false),m_canTouch(false),m_touchListener(NULL),m_touchCallback(NULL),isPlayer(false){
    
}

JettonSprite::~JettonSprite(){
    
}

bool JettonSprite::onTouchBegan(Touch *touch, Event *event){
    auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
    
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

void JettonSprite::onEnter(){
    Sprite::onEnter();
    
    //触摸响应注册
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(JettonSprite::onTouchBegan, this);//触摸开始
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);//注册分发器
}

void JettonSprite::onExit(){
    //移除触摸响应
    Sprite::onExit();
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

void JettonSprite::setTouchCallBackFunc(Ref* target,SEL_CallFuncN callfun){
    m_touchListener = target;
    m_touchCallback = callfun;
}
