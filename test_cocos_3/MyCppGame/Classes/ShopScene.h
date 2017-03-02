//
//  ShopScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef ShopScene_h
#define ShopScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"
#include "YNButton.h"

class RechargeItem : public Ref{
public:
    int rechargeWay;
    int amount;
    char date[40];
};

class BuyItem : public Ref{
public:
    int goldCount;
    int price_apple;
    int price_normal;
};

class ShopScene : public Layer, public TableViewDataSource, public TableViewDelegate, public ui::EditBoxDelegate
{
public:
    ShopScene();
    ~ShopScene();
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(ShopScene);
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void showSettingWithIndex(int index);
    void showBuyInfo();
    void popButtonCallback(Node* pNode);
private:
    Vector<YNButton* > listButtons;
    Vector<LayerColor* > listLayers;
    float recordListCellWidth = 0;
    Vector<RechargeItem* > rechargeItems;
    Vector<BuyItem* >buyList;
    bool hasGetRecordList = false;
    TableView* recordListTableView;
    int payIndex = 0;
    TableView* payListTableView;
    
    ui::EditBox* buyCountBox;
    ui::EditBox* userIDBox;
    ui::EditBox* giveCountBox;
    MessageManager* m_pMessage;
    
    void onHttpRequest_RechargeRecords();
    void onHttpRequest_DonateUserGold(const char* account, const char* count);
    void onHttpRequest_DonateUserDiamond(const char* account, const char* count);
    void onHttpRequest_SearchUser(const char* account);
    void onHttpRequest_GetOrderAndSign(float totalFee);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
    void onNotification_Pay(Ref* pSender);
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);//编辑框文字改变
    virtual void editBoxReturn(ui::EditBox* editBox); //触发return后的回调函数
};

#endif /* ShopScene_h */
