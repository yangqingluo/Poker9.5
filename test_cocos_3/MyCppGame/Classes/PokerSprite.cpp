#include "PokerSprite.h"

PokerSprite::PokerSprite():p_isSelected(false),p_canTouch(false),p_isFront(false){
    
}

PokerSprite::~PokerSprite(){
    
}

//PokerSprite* PokerSprite::create(const char *pszFileName, const Rect& rect){
//    PokerSprite* pk = new PokerSprite();
//    if (pk && pk->initWithFile(pszFileName,rect))
//    {
//        pk->autorelease();
//        return pk;
//    }
//    CC_SAFE_DELETE(pk);
//    return pk;
//}

PokerSprite* PokerSprite::create(PokerColor color, PokerPoint point){
    PokerSprite* pk = new PokerSprite();
    if (pk && pk->init()){
        pk->setContentSize(Size(27, 36));
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
    //触摸响应注册
    touchListener = EventListenerTouchOneByOne::create();//创建单点触摸事件监听器
    touchListener->onTouchBegan = CC_CALLBACK_2(PokerSprite::onTouchBegan, this);//触摸开始
    touchListener->onTouchMoved = CC_CALLBACK_2(PokerSprite::onTouchMoved, this);//触摸移动
    touchListener->onTouchEnded = CC_CALLBACK_2(PokerSprite::onTouchEnded, this);//触摸结束
    touchListener->setSwallowTouches(true);//不向下吞并触摸
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);//注册分发器
}
void PokerSprite::onExit(){
    //移除触摸响应
    _eventDispatcher->removeEventListenersForTarget(this);
    Sprite::onExit();
}
bool PokerSprite::onTouchBegan(Touch *pTouch, Event *pEvent){
    Size size = getContentSize();
    Rect rect(-size.width/2,-size.height/2,size.width,size.height);
    Point ptouch = convertTouchToNodeSpaceAR(pTouch); //由英文之意转换 触摸 到 节点空间
    if(rect.containsPoint(ptouch) && p_canTouch)
    {
        if (p_isSelected) {
            this->deselectedAction();
        }
        else {
            this->selectedAction();
        }
        
        return true;
    }
    //如果这里返回false触摸不被吞掉
    return false;
}
void PokerSprite::onTouchMoved(Touch *pTouch, Event *pEvent){
    
}
void PokerSprite::onTouchEnded(Touch *pTouch, Event *pEvent){
    
}
void PokerSprite::onTouchCancelled(Touch *pTouch, Event *pEvent){
    
}

void PokerSprite::showPokerAnimated(bool showFront, bool animated){
    if (showFront == p_isFront) {
        return;
    }
    
    char Icon[30] = {"poker/poker_back.png"};
    if (showFront) {
        sprintf(Icon, "poker/poker_%d_%d.png",p_color,p_point);
    }
    
    Texture2D* texture = TextureCache::sharedTextureCache()->addImage(Icon);
    CallFunc* func = CallFunc::create([=]{
                                          bgSprite->setTexture(texture);
                                      });
    if (animated) {
        auto scaleSmall = ScaleTo::create(1.0, 0, 1);
        auto scaleBig = ScaleTo::create(1.0, 1, 1);
        this->runAction(Sequence::create(scaleSmall, func, scaleBig, NULL));
    }
    else{
        this->runAction(Sequence::create(func, NULL));
    }
    p_isFront = !p_isFront;
}

void PokerSprite::setTouchPriority(int num){
    _eventDispatcher->setPriority(touchListener, num);
}

void PokerSprite::selectedAction(){
    this->p_isSelected = true;
    this->setPosition(Vec2(getPositionX(),getPositionY() + 10));
    
    
}

void PokerSprite::deselectedAction(){
    this->p_isSelected = false;
    this->setPosition(Vec2(getPositionX(),getPositionY() - 10));
    
    
}




