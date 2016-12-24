#include "MessageManager.h"

MessageManager::MessageManager(){
    
}
MessageManager::~MessageManager(){
    
}

bool MessageManager::init()
{
    if (!LayerColor::init()) {
        return false;
    }
    
    return true;
}

MessageManager* MessageManager::show(Node* nodeParent, int type,const char *msg)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    MessageManager* m_Layer = MessageManager::create();
    m_Layer->setContentSize(visibleSize);
    m_Layer->setPosition(origin.x, origin.y);
    
    switch (type) {
        case MESSAGETYPE_LOADING:
        {
            m_Layer->setColor(Color3B(0, 0, 0));
            m_Layer->setOpacity(60);
//            auto sp = Sprite::create("images/loading.png");
//            sp->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
//            sp->runAction(RepeatForever::create(RotateTo::create(1.0, 720)));
//            m_pLayer->addChild(sp);
            
            auto sp = Sprite::create("animations/meituan/listheader_0.png");
            sp->setPosition(Vec2(visibleSize.width / 2, 0.55 * visibleSize.height));
            m_Layer->addChild(sp);
            
            auto animation = Animation::create();
            animation->setDelayPerUnit(0.5);
            animation->setLoops(-1);
            animation->setRestoreOriginalFrame(true);
            for (int i = 0; i < 2; i++) {
                char name[100] = {0};
                sprintf(name, "animations/meituan/listheader_%d.png", i);
                animation->addSpriteFrameWithFile(name);
            }
            
            sp->runAction(Animate::create(animation));
        }
            break;
        case MESSAGETYPE_WARNING:
        {
            auto sp = Sprite::create("images/warning.png");
            sp->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            sp->runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(m_Layer, callfuncN_selector(MessageManager::onWarningHidden)),NULL));
            m_Layer->addChild(sp);
            CCLabelTTF *tf = CCLabelTTF::create(msg, "Arial", 20);
            tf->setDimensions(Size(sp->getContentSize().width-20, sp->getContentSize().height-40));
            tf->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            tf->setVerticalAlignment(TextVAlignment::CENTER);
            m_Layer->addChild(tf);
        }
            break;
        default:
            break;
    }
    nodeParent->addChild(m_Layer,999);
    
    return m_Layer;
}
void MessageManager::onWarningHidden(Node *node)
{
    hidden();
}
void MessageManager::hidden()
{
    removeAllChildren();
    removeFromParent();
}

bool MessageManager::onTouchBegan(Touch* touch, Event* event){
    
    
    return true;
}
void MessageManager::onEnter(){
    LayerColor::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(MessageManager::onTouchBegan, this);//触摸开始
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);//注册分发器
}
void MessageManager::onExit(){
    LayerColor::onExit();
    
}

