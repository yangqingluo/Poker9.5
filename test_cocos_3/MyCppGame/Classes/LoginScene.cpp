//
//  LoginScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "LoginScene.h"

USING_NS_CC;
using namespace ui;

Scene* LoginScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoginScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool LoginScene::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("images/new_bg.jpg");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    
    sprite->setScaleX(visibleSize.width / spx); //设置精灵宽度缩放比例
    sprite->setScaleY(visibleSize.height / spy);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    auto btn_BackItem = MenuItemImage::create(
                                              "images/window_close@2x.png",
                                              "images/window_close@2x.png",
                                              CC_CALLBACK_1(LoginScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    auto closeItem = MenuItemImage::create(
                                           "UIRes/visible.png",
                                           "UIRes/visible.png",
                                           CC_CALLBACK_1(LoginScene::btVisbleCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width  - closeItem->getContentSize().width/2 ,
                                origin.y + visibleSize.height - closeItem->getContentSize().height/2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /*Frame的测试*/
    Widget* frame = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("UIRes/DemoLogin.ExportJson");
    frame->setScale(0.8); //缩放比例可以随便设置
    frame->setAnchorPoint(Vec2(0.5f,0.5f));//锚点可以随便设置
    frame->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//坐标可以随便设置
    this->addChild(frame);
    
    //触碰测试
    Helper::seekWidgetByName(frame,"login_Button")->addTouchEventListener(CC_CALLBACK_2(LoginScene::btMenuCallBack,this));
    Helper::seekWidgetByName(frame,"close_Button")->addTouchEventListener(CC_CALLBACK_2(LoginScene::btMenuCallBack,this));
    Helper::seekWidgetByName(frame,"agree_CheckBox")->addTouchEventListener(CC_CALLBACK_2(LoginScene::btMenuCallBack,this));
    Helper::seekWidgetByName(frame,"name_TextField")->addTouchEventListener(CC_CALLBACK_2(LoginScene::btMenuCallBack,this));
    Helper::seekWidgetByName(frame,"password_TextField")->addTouchEventListener(CC_CALLBACK_2(LoginScene::btMenuCallBack,this));
    Helper::seekWidgetByName(frame,"confirm_TextField")->addTouchEventListener(CC_CALLBACK_2(LoginScene::btMenuCallBack,this));
    
    //frame初始化,可以试一下不初始化，直接layer->setVisible(false);设置窗口不可见之后,鼠标点击原本的按钮区域，看看是否有对话框出现
    m_frame.setFrame(frame,Helper::seekWidgetByName(frame,"backageImg"));
    
    //设置点击在对话框外部时候的监听事件，可以在这里将对话框隐藏
    m_frame.setTouchOutOfFrameEventListener(CC_CALLBACK_1(LoginScene::touchOutOfFrame,this));
    
    m_frame.setFrameVisible(true);
    
    
    return true;
}

void LoginScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
            break;
            
        default:{
        }
            break;
    }
}

void LoginScene::btMenuCallBack(Ref* pObject,Widget::TouchEventType eventType)
{
    if(eventType == Widget::TouchEventType::ENDED)
    {
        MessageBox("touch ui widget","touch ui widget");
    }
}

void LoginScene::btVisbleCallback(Ref* pSender)
{
    m_frame.setFrameVisible(true);
}

void LoginScene::touchOutOfFrame(cocos2d::ui::Widget*)
{
    //触碰点在对话框外，将对话框隐藏
    m_frame.setFrameVisible(false);
}
