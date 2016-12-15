//
//  PokerStabber.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/12/15.
//
//

#ifndef PokerStabber_h
#define PokerStabber_h

#include "cocos2d.h"
USING_NS_CC;

class PokerStabber : public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PokerStabber();
    ~PokerStabber();
    virtual bool init();
    CREATE_FUNC(PokerStabber);
    
    virtual void onEnter();
    virtual void onExit();
    static PokerStabber* create(const char* imageName, Size size);
    
private:
    Sprite* head;
    Label* infoLabel;
};

#endif /* PokerStabber_h */
