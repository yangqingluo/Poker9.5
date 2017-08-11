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
#include "PokerStabber.h"
#include "PokerStabberBtn.h"

struct SettlementRef {
    int multiple;//结算倍数
    int accounts;//结算数目
    bool winned;
};

class PokerChair : public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PokerChair();
    ~PokerChair();
    virtual bool init();
    virtual bool onTouchBegan(Touch* touch,Event* event);
    virtual void onEnter();
    virtual void onExit();
    
    CREATE_FUNC(PokerChair);
    static PokerChair* create(const char* betZoneImage,Size size);
    
    void initialSubViews();//初始化视图
    void setIsBanker(bool yn);//设置是否是庄家
    void setHighlighted(bool yn);
    void updatePokerPosition();//更新牌的位置
    void removeAllPokers();//移除扑克
    void addJetton(JettonSprite* jetton);//添加筹码
    void updateJetton(JettonSprite* jetton);//直接设置筹码
    void updateTotal(int value);//直接设置下注总和
    void removeAllJettons();//移除筹码
    void calculatePokerType();//计算牌型
    void showPokerType();//显示牌型
    void showSettlement();//显示结算
    void showSettlementForStabber();//显示刺结算
    void calculateSettlement(PokerChair* dealerChair);//正常结算
    void calculateSettlementForStabber(PokerChair* dealerChair, int jetton);//刺结算
    void showBeStabber(bool yn);//抢刺
    void showStabber(const char* imageName, const char* name, int jetton);//显示刺的信息
    void clearChair();//清除显示
    
    int betTotal;//总下注额
    int betPlayer;//玩家下注额
    PokerType m_PokerType;//牌型
    SettlementRef m_settlement;//结算数据
    Vector<PokerSprite *> pokerArray;
    Vector<JettonSprite *> jettonArray;
    void setTouchCallBackFunc(Ref* target,SEL_CallFuncND callfun);
    void stringFromBetValue(char* mString, int betValue);
private:
    CC_SYNTHESIZE_RETAIN(QLImageSprite*, m_betZoneBackGround, BetZoneBackGround);
    CC_SYNTHESIZE(Point,m_point,Point);//牌在桌面的初始位置
    CC_SYNTHESIZE(bool,m_canTouch,CanTouch);//是否能被点击
    
    PokerStabberBtn* m_BeStabberSprite;//抢刺按钮
    PokerStabber* m_Stabber;//刺标识
    Sprite* m_BankerSprite;//庄家标识
    Sprite* m_StartPointSprite;//发牌起始顺序标识的精灵
    Label* betTotalLabel;
    Label* betPlayerLabel;
    Label* pokerTypeLabel;//牌型文本
    Label* settlementLabel;//结算文本
    
    Ref* m_touchListener;
    SEL_CallFuncND m_touchCallback;
};

#endif /* defined(__PokerChair__) */
