//
//  HallScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/19.
//
//

#ifndef HallScene_h
#define HallScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "Global.h"
#include "QLImageSprite.h"
#include "NoteTip.h"
#include "MessageManager.h"

class RoomItem : public Ref{
public:
    char title[100];
    char content[100];
    char typeID[33];
    int type;
    int status;
    int chipMin;//带入筹码最小值
    int perMin;//单注下注最小值
};

class NoteItem : public Ref{
public:
    char title[20];
    char content[100];
    char image[40];
    int type;
};

class Hall : public Layer, public TableViewDataSource, public TableViewDelegate, public ui::EditBoxDelegate
{
public:
    Hall();
    ~Hall();
    static Scene* createScene();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC(Hall);
    
    
    Vector<MenuItem* > roomMenuItems;
    Vector<NoteItem* > noteItems;
    Vector<RoomItem* > tianItems;
    Vector<RoomItem* > diItems;
    Vector<RoomItem* > xuanItems;
    Vector<RoomItem* > huangItems;
    
    Label* roominfoLabel;
    void roomTypeCallback(Ref* pSender, int index);
    void roomTypeSelectedAction(int type);
    
    Label* userNameLabel;
    Label* userinfoLabel;
    void showUserInfo();
    
    void buttonCallback(Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
    void sliderChangerCallBack(Ref* pSender, Control::EventType type);
    
    float roomListCellHeight = 0;
    float roomListCellScale = 0.66;
    TableView* roomListTableView;
    
    float noteListCellHeight = 0;
    TableView* noteListTableView;
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
private:
    void popButtonCallback(Node* pNode);
    void showSettingChip();
    
    MessageManager* m_pMessage;
    Label* msgLabel;
    ui::EditBox* msgBox;
    int roomTypeSelected;
    int roomIndexSelected;
    int jettonToEnter;
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);//编辑框文字改变
    virtual void editBoxReturn(ui::EditBox* editBox); //触发return后的回调函数
    
    void onHttpRequest_SearchUser(const char* account);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
    void onNotification_Socket(Ref* pSender);
};

class RoomListCell : public TableViewCell
{
public:
//    static TableViewCell *createCell();
//    virtual bool init();
//    CREATE_FUNC(RoomListCell);
    
    Sprite* bg_sprite;
    Label* titleLabel;
    Label* contentLabel;
    Sprite* stateImage;
};

class NoteListCell : public TableViewCell
{
public:
    QLImageSprite* bg_sprite;
};

#endif /* HallScene_h */
