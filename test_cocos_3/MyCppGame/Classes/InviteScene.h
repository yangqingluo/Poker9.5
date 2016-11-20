//
//  InviteScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef InviteScene_h
#define InviteScene_h

#include "cocos2d.h"

class InviteScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void buttonCallback(cocos2d::Ref* pSender, int index);
    
    // implement the "static create()" method manually
    CREATE_FUNC(InviteScene);
};

#endif /* InviteScene_h */
