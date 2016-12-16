//
//  PokerStabberBtn.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/12/15.
//
//

#ifndef PokerStabberBtn_h
#define PokerStabberBtn_h

#include "cocos2d.h"
USING_NS_CC;

#include "JettonChosenSprite.h"

class PokerStabberBtn : public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PokerStabberBtn();
    ~PokerStabberBtn();
    virtual bool init();
    CREATE_FUNC(PokerStabberBtn);
    
    virtual void onEnter();
    virtual void onExit();
    static PokerStabberBtn* create(const Color4B color, Size size);
    
    void showBeStabber(bool yn);
private:
    JettonChosenSprite* showSprite;
};

#endif /* PokerStabberBtn_h */
