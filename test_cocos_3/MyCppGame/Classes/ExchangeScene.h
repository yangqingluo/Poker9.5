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

class ExchangeScene : public cocos2d::Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void buttonCallback(cocos2d::Ref* pSender, int index);
    CREATE_FUNC(ExchangeScene);
    
    float recordListCellWidth = 0;
    TableView* recordListTableView;
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    
    void tableCellTouched(TableView* table, TableViewCell* cell);
};

#endif /* ExchangeScene_h */
