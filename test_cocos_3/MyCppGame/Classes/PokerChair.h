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
#include "JettonSprite.h"
#include "QLImageSprite.h"

class PokerChair:public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PokerChair();
    ~PokerChair();
    virtual bool init();
    virtual bool onTouchBegan(Touch* touch,Event* event);
    virtual void onTouchMoved(Touch* touch,Event* event);
    virtual void onTouchEnded(Touch* touch,Event* event);
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(PokerChair);
    static PokerChair* create(const char* betZoneImage,Size size);
    
    void setIsBanker(bool yn);//设置是否是庄家
    void setHighlighted(bool yn);
    void updatePokerPosition();//更新牌的位置
    void addJetton(JettonSprite* jetton);//添加筹码
    void removeAllJettons();//移除筹码
    
    Vector<PokerSprite *> pokerArray;
    void setTouchCallBackFunc(Ref* target,SEL_CallFuncN callfun);
private:
    CC_SYNTHESIZE_RETAIN(QLImageSprite*, m_betZoneBackGround, BetZoneBackGround);
    CC_SYNTHESIZE(Point,m_point,Point);//牌在桌面的初始位置
    
    Sprite* m_BankerSprite;
    Sprite* m_StartPointSprite;//发牌起始顺序标识的精灵
    
    Ref* m_touchListener;
    SEL_CallFuncN m_touchCallback;
};

#endif /* defined(__PokerChair__) */
