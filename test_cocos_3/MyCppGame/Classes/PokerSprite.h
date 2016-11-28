#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "PokerGlobal.h"

class PokerSprite : public Sprite
{
public:
    PokerSprite();
    ~PokerSprite();
    static PokerSprite* create(const char *pszFileName, const Rect& rect);
    virtual void onEnter();
    virtual void onExit();
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    void showPokerAnimated(bool isBack, bool animated);//显示扑克
    void setTouchPriority(int num);
    
private:
    CC_SYNTHESIZE(bool,p_isSelected,isSelect);//是否已选
    CC_SYNTHESIZE(bool,p_canTouch,canTouch);//是否能被点击
    CC_SYNTHESIZE(PokerColor,p_color,poker_color);//花色
    CC_SYNTHESIZE(PokerPoint,p_point,poker_point);//牌点
    EventListenerTouchOneByOne* touchListener;
};
