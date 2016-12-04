//
//  PokerDeskScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#ifndef PokerDeskScene_h
#define PokerDeskScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "GameTimer.h"
#include "PokerChair.h"
#include "BetLimiter.h"

//牌局状态
enum DeskState
{
    DeskState_Default = 0,
    DeskState_Waiting = 1,//等待准备
    DeskState_Prepared,//已准备
    DeskState_SendPoker,//发牌
    DeskState_Bet,//下注
    DeskState_Settle,//结算
};

class PokerDesk : public cocos2d::Layer
{
public:
    PokerDesk();
    virtual ~PokerDesk();
    static cocos2d::Scene* createScene();
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC(PokerDesk);
    
    int deskType;
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void popButtonCallback(Node* pNode);
    void showSettingChip();
    
    QLImageSprite* upright_sprite;
    QLImageSprite* message_sprite;
    QLImageSprite* bottom_sprite;
    Label* countLabel;
    Label* messageLabel;
    
    BetLimiter* betLimiter;
    
    MenuItemImage* btn_PrepareItem;
    MenuItemImage* btn_AnotherdeskItem;
    
    GameTimer* showTimer;
    void showTimerDoneCallback(Node* pNode);
    
    void goBackAction();
    void waitForPrepareAction();
    void preparedAction();
    void betAction();
    void settleAction();
    
    void turnedSinglePokerCallback(Node* pSender);
    void sendedSinglePoker(Node* pSender, void* pData);
    void touchedChairCallback(Node* pSender);
private:
    bool m_isSendSingle;//是否发完一张牌
    int m_deskState;//当前状态
    int m_IndexSend;//已发出第几张牌
    int m_IndexStart;//发牌起始顺序
    Vector<PokerChair *> m_arrChairs;//所有的座位
    Vector<PokerSprite *> m_arrPokers;//所有的牌
    PokerChair* createChair(const char* backgroudImage, float widthScale, float heightScale, int index);//生成一个座位
    PokerSprite* createPoker(PokerColor color,PokerPoint point);//生成一个牌
    JettonSprite* createjetton(int value);//生成一个筹码
    
    bool createPokers();//创建一副扑克牌
    bool reindexPoker();//洗牌
    void sendPoker();//发牌
    void movePoker(PokerChair* chair,PokerSprite* poker);//发牌移动动画
    void addJetton(PokerChair* chair,JettonSprite* jetton);//添加筹码
    virtual void update(float delta);//系统scheduleupdate回调函数
};

#endif /* PokerDeskScene_h */
