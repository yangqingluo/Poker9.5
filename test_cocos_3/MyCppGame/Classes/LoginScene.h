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

class LoginScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoginScene);
};

#endif /* LoginScene_h */
