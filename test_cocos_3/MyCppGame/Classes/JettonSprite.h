#pragma once

#include "cocos2d.h"
USING_NS_CC;

class JettonSprite : public Sprite
{
public:
    JettonSprite();
    ~JettonSprite();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onEnter();
    virtual void onExit();
    
    static JettonSprite* create(int value, Size size);
    void setSelected(bool select);
    bool isValidValue(int value);
    
    void setTouchCallBackFunc(Ref* target,SEL_CallFuncN callfun);
private:
    CC_SYNTHESIZE_READONLY(bool,m_isSelected,IsSelect);//是否已选
    CC_SYNTHESIZE_READONLY(int,m_value,JettonValue);//筹码值
    CC_SYNTHESIZE(bool,m_canTouch,CanTouch);//是否能被点击
    
    Sprite* bgSprite;
    Sprite* valueSprite;
    
    Ref* m_touchListener;
    SEL_CallFuncN m_touchCallback;
};
