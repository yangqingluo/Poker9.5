//
//  OnlinePokerDeskScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#ifndef OnlinePokerDeskScene_h
#define OnlinePokerDeskScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

using namespace ui;

#include "Global.h"

#include "GameTimer.h"
#include "PokerChair.h"
#include "BetLimiter.h"
#include "Player.h"

#include "NoteTip.h"
#include "MessageManager.h"

class OnlinePokerDesk : public cocos2d::Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    OnlinePokerDesk();
    ~OnlinePokerDesk();
    static cocos2d::Scene* createScene();
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC(OnlinePokerDesk);
    
    int roomType;//房间类型
    char roomTypeId[Max_ID_Length];//房间类型Id
    int jettonToEnter;//带入的筹码数量
    char roomPassword[length_room_password + 1];
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void popButtonCallback(Node* pNode);
    void showSettingChip();
    
    Button* btn_addJetton;
    QLImageSprite* upright_sprite;
    QLImageSprite* bottom_sprite;
    QLImageSprite* playerList_sprite;
    TableView* playerListTableView;
    Label* countLabel;
    Label* gamePlayerInfoLabel;
    Label* roundInfoLabel;
    BetLimiter* betLimiter;
    Sprite* dealerHead;
    MenuItemImage* btn_BeBankerItem;
    
    MenuItemImage* btn_PrepareItem;
    
    GameTimer* showTimer;
    void showTimerDoneCallback(Node* pNode);
    
    Player* dealerPlayer;//庄家
    Player* pcPlayer;//电脑玩家
//    Player* gamePlayer;//玩家
    Player* stabberPlayer;//刺
    
    void goBackAction();
    void waitForPrepareAction();
    void preparedAction();
    void waitForBetAction();
    void settleAction();
    void waitForChooseDealerAction();//等待选择庄家
    void waitForChooseStabberAction();//等待抢刺
    void sendPokerAction();//发牌
    
    void showGamePlayerInfo();//显示玩家信息（包括玩家列表）
    void showDealerInfo();//显示庄家信息
    void resetShowDealerInfo();//重置显示庄家信息
    
    void turnedSinglePokerCallback(Node* pSender);
    void sendedSinglePoker(Node* pSender, void* pData);
    void touchedChairCallback(Node* pSender, void* pTarget);
    
private:
    MessageManager* m_pMessage;
    void showMessageManager(bool isShow);
    
    bool m_isStarted;//牌局是否开始
    bool m_isSendSingle;//是否发完一张牌
    bool m_isSendSet;//是否发完一把牌(8+1张)
    int m_deskState;//当前状态
    int m_IndexSend;//已发出第几张牌
    int m_IndexStart;//发牌起始顺序
    Vec2 judgementPosition;//判定牌位置
    size_t judgementPokerIndex;
    Vector<PokerChair *> m_arrChairs;//所有的座位
    Vector<PokerSprite *> m_arrPokers;//所有的牌
    PokerChair* createChair(const char* backgroudImage, float widthScale, float heightScale, int index);//生成一个座位
    PokerSprite* createPoker(PokerColor color,PokerPoint point);//生成一个牌
    JettonSprite* createjetton(int value);//生成一个筹码
    
    bool createPokers();//创建一副扑克牌
    void updatePokerData();
    void updatePokerWithData(PokerSprite* poker, PokerData data);
    void adjustPoker(int index);//发牌前调整
    void sendPoker();//发牌
    void sendPokerWithoutAnimation();//发牌(没有动画)
    void movePoker(PokerChair* chair,PokerSprite* poker);//发牌移动动画
    void movePokerWithoutAnimation(PokerChair* chair,PokerSprite* poker);//发牌移动(没有动画)
    void addJetton(PokerChair* chair,JettonSprite* jetton);//添加筹码
    
    virtual void update(float delta);//系统scheduleupdate回调函数
    
    void updateDeskState(DeskState state);
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void onNotification_Socket(Ref* pSender);
    void stepIn(DeskState state);
};

#endif /* OnlinePokerDeskScene_h */
