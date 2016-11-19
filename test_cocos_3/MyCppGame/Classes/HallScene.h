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

class Hall : public cocos2d::Layer, public TableViewDataSource
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Hall);
    
    Size tableCellSizeForIndex(TableView *table, ssize_t idx);
    Size cellSizeForTable(TableView *table);
    TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView *table);
};

#endif /* HallScene_h */
