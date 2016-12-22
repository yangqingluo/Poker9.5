#include "MessageManager.h"
bool MessageManager::init()
{
    return true;
}

void MessageManager::show(Node* nodeParent, int type,const char *msg)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    m_pLayer = ShadowLayer::create();
    
    m_pLayer->setContentSize(visibleSize);
    
    switch (type) {
        case MESSAGETYPE_LOADING:
        {
            auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 60));
            m_pLayer->addChild(colorLayer);
//            auto sp = Sprite::create("images/loading.png");
//            sp->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
//            sp->runAction(RepeatForever::create(RotateTo::create(1.0, 720)));
//            m_pLayer->addChild(sp);
            
            auto sp = Sprite::create("animations/meituan/listheader_0.png");
            sp->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 0.65 * visibleSize.height));
            m_pLayer->addChild(sp);
            
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
            sp->runAction(Sequence::create(DelayTime::create(2),CallFuncN::create(this, callfuncN_selector(MessageManager::onWarningHidden)),NULL));
            m_pLayer->addChild(sp);
            CCLabelTTF *tf = CCLabelTTF::create(msg, "Arial", 20);
            tf->setDimensions(Size(sp->getContentSize().width-20, sp->getContentSize().height-40));
            tf->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
            tf->setVerticalAlignment(TextVAlignment::CENTER);
            m_pLayer->addChild(tf);
        }
            break;
        default:
            break;
    }
    nodeParent->addChild(m_pLayer,999);
}
void MessageManager::onWarningHidden(Node *node)
{
    hidden();
}
void MessageManager::hidden()
{
    m_pLayer->removeAllChildren();
    m_pLayer->removeFromParent();
}

bool ShadowLayer::onTouchBegan(Touch* touch, Event* event){
    
    
    return true;
}
void ShadowLayer::onEnter(){
    Layer::onEnter();
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ShadowLayer::onTouchBegan, this);//触摸开始
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);//注册分发器
}
void ShadowLayer::onExit(){
    Layer::onExit();
    
}
bool ShadowLayer::init()
{
    
    return true;
}
