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
};

#endif /* PokerDeskScene_h */
