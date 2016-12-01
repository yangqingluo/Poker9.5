#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "PokerGlobal.h"

class PokerSprite : public Sprite
{
public:
    PokerSprite();
    ~PokerSprite();
//    static PokerSprite* create(const char *pszFileName, const Rect& rect);
    static PokerSprite* create(PokerColor color, PokerPoint point, Size size);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    void showPokerAnimated(bool showFront, bool animated);//显示扑克
    void setTouchPriority(int num);
    
    void selectedAction();
    void deselectedAction();
    
    void showedPoker();
    void removeAction(float delta);
    void setCallBackFunc(Ref* target,SEL_CallFuncN callfun);
private:
    CC_SYNTHESIZE(bool,p_isSelected,IsSelect);//是否已选
    CC_SYNTHESIZE(bool,p_canTouch,CanTouch);//是否能被点击
    CC_SYNTHESIZE_READONLY(bool,p_isFront,IsFront);//是否是正面
    CC_SYNTHESIZE(PokerColor,p_color,Poker_color);//花色
    CC_SYNTHESIZE(PokerPoint,p_point,Poker_point);//牌点
    EventListenerTouchOneByOne* touchListener;
    Sprite* bgSprite;
    
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
};
