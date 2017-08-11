//
//  BetSlider.h
//
//  Created by yangqingluo on 16/11/9.
//
//

#ifndef __BetSlider__
#define __BetSlider__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "JettonChosenSprite.h"


class BetSlider : public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    BetSlider();
    ~BetSlider();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(BetSlider);
    static BetSlider* create(int min, int max, Size size);
    
    void updateMinMax(int min, int max);
    void sliderChangerCallBack(Ref* pSender, Control::EventType type);
private:
    CC_SYNTHESIZE_READONLY(int,m_value,SelectedJettonValue);//当前选中筹码面值
    ControlSlider* m_slider;
    Label* m_label;
    
    void touchedJettonCallback(Node* pSender);
    JettonChosenSprite* selectedJetton;
};

#endif /* defined(__BetSlider__) */
