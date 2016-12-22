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

class LoginScene : public Layer, ui::EditBoxDelegate
{
public:
    static Scene* createScene();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(LoginScene);
    
    void buttonCallback(Ref* pSender, int index);
private:
    ui::EditBox* usernameBox;
    ui::EditBox* passwordBox;
    
    virtual void editBoxEditingDidBegin(ui::EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(ui::EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(ui::EditBox* editBox, const std::string& text); //编辑框文字改变
    virtual void editBoxReturn(ui::EditBox* editBox); //触发return后的回调函数
};

#endif /* LoginScene_h */
