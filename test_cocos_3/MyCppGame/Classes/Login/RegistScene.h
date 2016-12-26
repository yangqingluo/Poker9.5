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

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"

class RegistScene : public Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(RegistScene);
    
    void buttonCallback(Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    ui::EditBox* usernameBox;
    ui::EditBox* passwordBox;
    ui::EditBox* passwordAgainBox;
    ui::EditBox* vcodeBox;
    ui::EditBox* nikenameBox;
    
    MessageManager* m_pMessage;
    
    cocos2d::ui::Button* btn_getVCode;
    float pTime;
    void wait(float delta);
    
    void onHttpRequest_GetVCode(string username);
    void onHttpRequest_Regist(string username, string password, string vcode, string nikename);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
};

#endif /* RegistScene_h */
