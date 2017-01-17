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

//房间类型
enum BetType
{
    BetType_Default = 0,
    BetType_Addition,
};

class BetLimiter : public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    BetLimiter();
    ~BetLimiter();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(BetLimiter);
    
    static BetLimiter* create(const int* jettonValueArray, size_t count, Size size, BetType type = BetType_Default);
    void updateValue(int value);
    int minValue = 0;
    void reset();
private:
    CC_SYNTHESIZE_READONLY(int,m_value,SelectedJettonValue);//当前选中筹码面值
    BetType m_type;
    Label* m_label;
    
    void showValueLabel();
    
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void touchedJettonCallback(Node* pSender);
    JettonChosenSprite* selectedJetton;
};

#endif /* defined(__BetLimiter__) */
