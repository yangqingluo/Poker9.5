//
//   HelpScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef  HelpScene_h
#define  HelpScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "YNButton.h"

class  HelpScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC(HelpScene);
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
    Vector<YNButton* > listButtons;
    Vector<LayerColor* > listLayers;
    Vector<ScrollView* > listScrolls;
    
    void showSettingWithIndex(int index);
};

#endif /*  HelpScene_h */
