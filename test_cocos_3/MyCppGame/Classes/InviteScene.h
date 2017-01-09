//
//  InviteScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef InviteScene_h
#define InviteScene_h

#include "cocos2d.h"

#include "cocos-ext.h"
USING_NS_CC_EXT;

class InviteScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(InviteScene);
    
    void boardShare(Ref* pSender);
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif /* InviteScene_h */
