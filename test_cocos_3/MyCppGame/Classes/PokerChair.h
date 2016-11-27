//
//  PokerChair.h
//
//  Created by yangqingluo on 16/11/9.
//
//

#ifndef __PokerChair__
#define __PokerChair__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class PokerChair:public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PokerChair();
    ~PokerChair();
    virtual bool init();
    CREATE_FUNC(PokerChair);
    
    static PokerChair* create(const char* backgroudImage,Size dialogSize);
    
    bool onTouchBegan(Touch* touch,Event* event);
    void onTouchMoved(Touch* touch,Event* event);
    void onTouchEnded(Touch* touch,Event* event);
    
    virtual void onEnter();
    virtual void onExit();
private:
    Size m_dialogContentSize;
    
    CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
};

#endif /* defined(__PokerChair__) */
