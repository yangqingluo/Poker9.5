//
//  InviteScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef InviteScene_h
#define InviteScene_h

#include "cocos2d.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"

#include "Global.h"

class InviteItem : public Ref{
public:
//    char userId[Max_ID_Length];
    char nikename[Min_String_Length];
    char account[Max_ID_Length];
//    char inviteCode[20];
};

class InviteScene : public cocos2d::Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    InviteScene();
    ~InviteScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(InviteScene);
    
    void boardShare(Ref* pSender);
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    
    float recordListCellWidth = 0;
    TableView* recordListTableView;
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
private:
    MessageManager* m_pMessage;
    Vector<InviteItem* > inviteItems;
    bool hasGetInviteList = false;
    
    void onHttpRequest_GetInviteList();
    void onHttpRequest_GetInviter();
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
    void onNotification_NoteTip(Ref* pSender);
};

#endif /* InviteScene_h */
