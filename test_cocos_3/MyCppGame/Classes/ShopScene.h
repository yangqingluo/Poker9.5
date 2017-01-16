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

class ShopScene : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ShopScene);
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void showSettingWithIndex(int index);
private:
    Vector<YNButton* > listButtons;
    Vector<LayerColor* > listLayers;
    float recordListCellWidth = 0;
    TableView* recordListTableView;
    
    ui::EditBox* buyCountBox;
    ui::EditBox* userIDBox;
    ui::EditBox* giveCountBox;
    MessageManager* m_pMessage;
    
    void onHttpRequest_DonateUserGold(const char* account, const char* count);
    void onHttpRequest_DonateUserDiamond(const char* account, const char* count);
    void onHttpRequest_SearchUser(const char* account);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
};

#endif /* ShopScene_h */
