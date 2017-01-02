#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "network/HttpClient.h"
using namespace cocos2d::network;

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    ~HelloWorld();
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    void loginCallback(cocos2d::Ref* pSender, int index);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    
    void onHttpRequest(std::string type);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);

};

#endif // __HELLOWORLD_SCENE_H__
