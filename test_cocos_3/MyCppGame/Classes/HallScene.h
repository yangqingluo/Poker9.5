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

class RoomItem : public Ref{
public:
    char title[20];
    char content[100];
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

class Hall : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Hall);
    
    int roomTypeSelected;
    MenuItem* room_TianItem;
    MenuItem* room_DiItem;
    MenuItem* room_XuanItem;
    Vector<RoomItem *> tianItems;
    Vector<RoomItem *> diItems;
    Vector<RoomItem *> xuanItems;
    Vector<NoteItem *> noteItems;
    void roomTypeCallback(cocos2d::Ref* pSender, int index);
    void roomTypeSelectedAction(int type);
    
    float roomListCellHeight = 0;
    float roomListCellScale = 0.66;
    TableView* roomListTableView;
    
    float noteListCellHeight = 0;
    TableView* noteListTableView;
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    
    void tableCellTouched(TableView* table, TableViewCell* cell);
//    void tableCellHighlight(TableView* table, TableViewCell* cell);
//    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
//    void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
//    void scrollViewDidScroll(ScrollView* view) {};
//    void scrollViewDidZoom(ScrollView* view) {};
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
    Sprite* bg_sprite;
};

#endif /* HallScene_h */
