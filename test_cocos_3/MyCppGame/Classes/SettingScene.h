//
//   SettingScene.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef  SettingScene_h
#define  SettingScene_h

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "network/HttpClient.h"
using namespace cocos2d::network;
using namespace std;
using namespace ui;

#include "NoteTip.h"
#include "MessageManager.h"
#include "YNButton.h"

class  SettingScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual void onEnter();
    virtual void onExit();
    virtual bool init();
    CREATE_FUNC( SettingScene);
    
    void buttonCallback(cocos2d::Ref* pSender, int index);
    void touchEvent(Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
//    MenuItemImage* btn_InfoSettingItem;
//    MenuItemImage* btn_SystemSettingItem;
    YNButton* btn_infoSetting;
    YNButton* btn_systemSetting;
    LayerColor* infoSettingLayer;
    LayerColor* systemSettingLayer;
    
    ui::EditBox* nikenameBox;
    
    MessageManager* m_pMessage;
    
    void showSettingWithIndex(int index);
    
    void onHttpRequest_SettingNikename(string nikename);
    void onHttpResponse(HttpClient* sender, HttpResponse* response);
};

#endif /*  SettingScene_h */
