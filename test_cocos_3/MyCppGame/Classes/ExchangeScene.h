//
//  ExchangeScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef ExchangeScene_h
#define ExchangeScene_h

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
#include "Global.h"

class ExchangeItem : public Ref{
public:
    int price;//兑换价格
    char imagePath[50];
    char description[200];
    ValueVector colors;
};

class ExchangeRecordItem : public Ref{
public:
    char ID[Max_ID_Length];
    char receiveName[Max_Name_Length];
    char address[Max_Address_Length];
    int gameBitAmount;
    int status;
    char expressCode[Max_ExpressCode_Length];
    char expressType[Max_Name_Length];
    char remarks[Min_String_Length];
    char date[40];
};

class ExchangeScene : public cocos2d::Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ExchangeScene);
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void showSettingWithIndex(int index);
private:
    Vector<YNButton* > listButtons;
    Vector<LayerColor* > listLayers;
    float recordListCellWidth = 0;
    TableView* recordListTableView;
    TableView* exchangeListTableView;
    bool hasGetRecordList = false;
    Vector<ExchangeItem* > exchangeItems;
    Vector<ExchangeRecordItem* > recordItems;
    size_t m_itemIndex;
    size_t m_colorIndex;
    
    void showExchangeInput(size_t itemIndex, size_t colorIndex);
    void popButtonCallback(Node* pNode);
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    MessageManager* m_pMessage;
    
    void onHttpRequest_ExchangeRecords();
    void onHttpRequest_addAwardRecord(const char* name, const char* address, const char* tel, const char* remarks, int gold);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
};

#endif /* ExchangeScene_h */
