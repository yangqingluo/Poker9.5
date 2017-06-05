#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    ~HelloWorld();
    static cocos2d::Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void loginCallback(cocos2d::Ref* pSender, int index);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void doLoginQQ(const char* m_string);
private:
    MessageManager* m_pMessage;
    void showMessageManager(bool isShow);
    
    void onHttpRequest_LoginQQ(const char* m_string);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
    void onNotification_Socket(Ref* pSender);
    
};

#endif // __HELLOWORLD_SCENE_H__
