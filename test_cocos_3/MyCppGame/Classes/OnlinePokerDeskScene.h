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
//    int chipMin;//带入筹码最小值
//    int perMin;//单注下注最小值
    char roomPassword[length_room_password + 1];
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void popButtonCallback(Node* pNode);
    void showSettingChip();
    
    QLImageSprite* upright_sprite;
    QLImageSprite* bottom_sprite;
    QLImageSprite* playerList_sprite;
    TableView* playerListTableView;
    Label* countLabel;
    Label* gamePlayerInfoLabel;
    Label* roomInfoLabel;
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
//    void chooseDealerAction();//选择庄家
    void waitForChooseDealerAction();//等待选择庄家
//    void dealerDidChoosedAction();//庄家已选择
    void waitForChooseStabberAction();//等待抢刺
    void chooseStabberAction(int index);//抢刺
    void sendPokerAction();//发牌
    
    void showGamePlayerInfo();
    void showDealerInfo();
    
    void turnedSinglePokerCallback(Node* pSender);
    void sendedSinglePoker(Node* pSender, void* pData);
    void touchedChairCallback(Node* pSender, void* pTarget);
    
private:
    MessageManager* m_pMessage;
    void showMessageManager(bool isShow);
    
    bool m_isSendSingle;//是否发完一张牌
    bool m_isSendSet;//是否发完一把牌(8张)
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
    bool reindexPoker();//洗牌
    void updatePokerWithData(PokerSprite* poker, PokerData data);
    void sendPoker();//发牌
    void movePoker(PokerChair* chair,PokerSprite* poker);//发牌移动动画
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
