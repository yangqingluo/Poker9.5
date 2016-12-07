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
#include "FrameManager.h"

class LoginScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchOutOfFrame(cocos2d::ui::Widget*);
    
    void btVisbleCallback(cocos2d::Ref*);
    void btMenuCallBack(cocos2d::Ref*,cocos2d::ui::Widget::TouchEventType);
    // implement the "static create()" method manually
    CREATE_FUNC(LoginScene);
    
private:
    FrameManager m_frame;
};

#endif /* LoginScene_h */
