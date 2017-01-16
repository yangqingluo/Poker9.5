//
//  InviterScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#ifndef InviterScene_h
#define InviterScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;

#include "NoteTip.h"
#include "MessageManager.h"


class InviterScene : public Layer, ui::EditBoxDelegate
{
public:
    InviterScene();
    ~InviterScene();
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(InviterScene);
    
    void buttonCallback(Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    ui::EditBox* usernameBox;
    MessageManager* m_pMessage;
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text);//编辑框文字改变
    virtual void editBoxReturn(ui::EditBox* editBox); //触发return后的回调函数
    
    void onHttpRequest_Inviter(const char* code);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
};

#endif /* InviterScene_h */
