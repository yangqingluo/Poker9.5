//
//  LoginScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#ifndef LoginScene_h
#define LoginScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "FrameManager.h"
#include "QLImageSprite.h"

class LoginShowItem : public Ref{
public:
    char title[20];
    char content[100];
};

class LoginScene : public Layer, public TableViewDataSource, public TableViewDelegate
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(LoginScene);
    
    void buttonCallback(Ref* pSender, int index);
private:
    FrameManager m_frame;
    Vector<LoginShowItem* > showItems;
    TableView* inputListTableView;
    
    Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    ssize_t numberOfCellsInTableView(TableView* table);
    void tableCellTouched(TableView* table, TableViewCell* cell);
    
    void scrollViewDidScroll(ScrollView* view);
    void scrollViewDidZoom(ScrollView* view);
};

#endif /* LoginScene_h */
