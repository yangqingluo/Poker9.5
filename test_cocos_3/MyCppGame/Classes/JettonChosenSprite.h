#ifndef __JettonChosenSprite__
#define __JettonChosenSprite__

#include "JettonSprite.h"

class JettonChosenSprite : public Sprite
{
public:
    JettonChosenSprite();
    ~JettonChosenSprite();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onEnter();
    virtual void onExit();
    
    bool isPlayer;//是否当前玩家下注
    static JettonChosenSprite* create(int value, Size size);
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

#endif /* defined(__JettonChosenSprite__) */
