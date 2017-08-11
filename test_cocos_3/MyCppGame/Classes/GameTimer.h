#ifndef _GAME_TIMER_H_
#define _GAME_TIMER_H_

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class GameTimer : public cocos2d::Node
{
public:
    GameTimer();
    
    virtual ~GameTimer();
    
    static GameTimer* createTimer();
    
    void showPrefix();
    void stop();
    void start(float time);
    void update(float delta);
    
    virtual bool init();
    
    int showTag;
    char prefixString[100];
    
    void setCallBackFunc(Ref* target,SEL_CallFuncN callfun);
    
private:
    CC_SYNTHESIZE_READONLY(bool,m_valid,IsValid);//是否处于倒计时状态中
    Label*               label;
    float                   pTime;
    int soundID_timer;
    
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
};

#endif
