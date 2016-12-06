//
//  BetLimiter.h
//
//  Created by yangqingluo on 16/11/9.
//
//

#ifndef __BetLimiter__
#define __BetLimiter__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "JettonChosenSprite.h"


class BetLimiter:public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    BetLimiter();
    ~BetLimiter();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(BetLimiter);
    static BetLimiter* create(const int* jettonValueArray, size_t count, Size size);
    
private:
    CC_SYNTHESIZE_READONLY(int,m_value,SelectedJettonValue);//当前选中筹码面值
    
    void touchedJettonCallback(Node* pSender);
    JettonChosenSprite* selectedJetton;
};

#endif /* defined(__BetLimiter__) */
