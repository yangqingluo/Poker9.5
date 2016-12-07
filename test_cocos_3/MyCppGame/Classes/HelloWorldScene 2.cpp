#include "HelloWorldScene.h"
#include "editor-support\cocostudio\CocoStudio.h"
#include "ui\CocosGUI.h"

USING_NS_CC;
using namespace ui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "visible.png",
                                           "visible.png",
                                           CC_CALLBACK_1(HelloWorld::btVisbleCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width  - closeItem->getContentSize().width/2 ,
								origin.y + visibleSize.height - closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);


    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
	/*Frame�Ĳ���*/
	Widget* frame = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("UIRes/DemoLogin.ExportJson");
	frame->setScale(0.8f); //���ű��������������
	frame->setAnchorPoint(Vec2(0.5f,0.5f));//ê������������
	frame->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));//��������������
	this->addChild(frame);
	
	//��������
	Helper::seekWidgetByName(frame,"login_Button")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"close_Button")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"agree_CheckBox")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"name_TextField")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"password_TextField")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));
	Helper::seekWidgetByName(frame,"confirm_TextField")->addTouchEventListener(CC_CALLBACK_2(HelloWorld::btMenuCallBack,this));

	//frame��ʼ��,������һ�²���ʼ����ֱ��layer->setVisible(false);���ô��ڲ��ɼ�֮��,�����ԭ���İ�ť���򣬿����Ƿ��жԻ������
	m_frame.setFrame(frame,Helper::seekWidgetByName(frame,"backageImg"));

	//���õ���ڶԻ����ⲿʱ��ļ����¼������������ｫ�Ի�������
	m_frame.setTouchOutOfFrameEventListener(CC_CALLBACK_1(HelloWorld::touchOutOfFrame,this));
	
	m_frame.setFrameVisible(true);

    return true;
}

void HelloWorld::btVisbleCallback(Ref* pSender)
{
	m_frame.setFrameVisible(true);
}

void HelloWorld::btMenuCallBack(Ref* pObject,Widget::TouchEventType eventType)
{
	if(eventType == Widget::TouchEventType::ENDED)
	{
		MessageBox("touch ui widget","touch ui widget");
	}
}

void HelloWorld::touchOutOfFrame(cocos2d::ui::Widget*)
{
	//�������ڶԻ����⣬���Ի�������
	m_frame.setFrameVisible(false);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
