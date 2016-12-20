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



class LoginShowItem : public Ref{
public:
    char title[20];
    char content[100];
};

class LoginScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(LoginScene);
    
    void buttonCallback(Ref* pSender, int index);
private:
    
    
};

#endif /* LoginScene_h */
