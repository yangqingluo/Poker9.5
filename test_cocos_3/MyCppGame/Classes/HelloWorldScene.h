#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ODSocket.h"
#include "tcpcommand.h"

#include "network/HttpClient.h"
using namespace cocos2d::network;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void loginCallback(cocos2d::Ref* pSender, int index);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
public:
    void connectServer();
    void receiveData();
    void onHttpRequest(std::string type);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
private:
    ODSocket socket;
};

#endif // __HELLOWORLD_SCENE_H__
