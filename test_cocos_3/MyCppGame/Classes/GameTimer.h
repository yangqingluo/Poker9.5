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
    
    static GameTimer* createTimer(float time);
    
    void showPrefix();
    void start(float time);
    void update(float delta);
    
    bool init(float time);
    
    int showTag;
    char prefixString[100];
    
    void setCallBackFunc(Ref* target,SEL_CallFuncN callfun);
    
private:
    Label*               label;
    float                   pTime;
    
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
};

#endif
