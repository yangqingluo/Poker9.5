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

#include "PokerSprite.h"

class PokerChair:public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PokerChair();
    ~PokerChair();
    virtual bool init();
    CREATE_FUNC(PokerChair);
    
    static PokerChair* create(const char* backgroudImage,Size size);
    
    bool onTouchBegan(Touch* touch,Event* event);
    void onTouchMoved(Touch* touch,Event* event);
    void onTouchEnded(Touch* touch,Event* event);
    
    virtual void onEnter();
    virtual void onExit();
    
    void setIsBanker(bool yn);//设置是否是庄家
    void setHighlighted(bool yn);
    void updatePokerPosition();//更新牌的位置
    
    Sprite* m_BankerSprite;
    Vector<PokerSprite *> pokerArray;
private:
    Size m_dialogContentSize;
    
    CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE(Point,m_point,Point);//牌在桌面的初始位置
};

#endif /* defined(__PokerChair__) */
