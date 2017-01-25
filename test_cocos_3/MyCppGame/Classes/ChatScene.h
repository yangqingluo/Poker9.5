//
//   ChatScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef  ChatScene_h
#define  ChatScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

using namespace ui;

#include "NoteTip.h"
#include "MessageManager.h"

class  ChatScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC( ChatScene);
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    EditBox* msgBox;
};

#endif /*  ChatScene_h */
