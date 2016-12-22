//
//  RegistScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#ifndef RegistScene_h
#define RegistScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;



class RegistShowItem : public Ref{
public:
    char title[20];
    char content[100];
};

class RegistScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(RegistScene);
    
    void buttonCallback(Ref* pSender, int index);
private:
    
    
};

#endif /* RegistScene_h */
