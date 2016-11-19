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

class Hall : public cocos2d::Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Hall);
    
    TableView *roomListTableView;
    
    Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
    
    void tableCellTouched(TableView* table, TableViewCell* cell);
//    void tableCellHighlight(TableView* table, TableViewCell* cell);
//    void tableCellUnhighlight(TableView* table, TableViewCell* cell);
//    void tableCellWillRecycle(TableView* table, TableViewCell* cell);
    
//    void scrollViewDidScroll(ScrollView* view) {};
//    void scrollViewDidZoom(ScrollView* view) {};
};

#endif /* HallScene_h */
