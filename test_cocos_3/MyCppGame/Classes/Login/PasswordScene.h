//
//  PasswordScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#ifndef PasswordScene_h
#define PasswordScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"


class PasswordShowItem : public Ref{
public:
    char title[20];
    char content[100];
};

class PasswordScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(PasswordScene);
    
    void buttonCallback(Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    ui::EditBox* usernameBox;
    ui::EditBox* passwordBox;
    ui::EditBox* passwordAgainBox;
    ui::EditBox* vcodeBox;
    
    MessageManager* m_pMessage;
    
};

#endif /* PasswordScene_h */
