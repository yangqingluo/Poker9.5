//
//  PokerDeskScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#ifndef PokerDeskScene_h
#define PokerDeskScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "GameTimer.h"

class PokerDesk : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual void onEnter();
    virtual bool init();
    CREATE_FUNC(PokerDesk);
    
    int deskType;
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void popButtonCallback(Node* pNode);
    void showSettingChip();
    
    Sprite* upright_sprite;
    Sprite* message_sprite;
    Label* countLabel;
    Label* messageLabel;
    
    MenuItemImage* btn_PrepareItem;
    MenuItemImage* btn_AnotherdeskItem;
    
    GameTimer* showTimer;
    void showTimerDoneCallback(Node* pNode);
    
    void prepareAction();
};

#endif /* PokerDeskScene_h */
