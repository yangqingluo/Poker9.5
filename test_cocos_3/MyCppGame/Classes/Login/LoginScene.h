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

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"
#include "json.h"

class LoginScene : public Layer, ui::EditBoxDelegate
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(LoginScene);
    
    void buttonCallback(Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    ui::EditBox* usernameBox;
    ui::EditBox* passwordBox;
    MessageManager* m_pMessage;
    Json::Reader reader;
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);//编辑框文字改变
    virtual void editBoxReturn(ui::EditBox* editBox); //触发return后的回调函数
    
    void onHttpRequest_Login(string username, string password);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
};

#endif /* LoginScene_h */
