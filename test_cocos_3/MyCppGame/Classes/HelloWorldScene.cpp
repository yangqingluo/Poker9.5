#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LoginScene.h"
#include "HallScene.h"

USING_NS_CC;

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
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("九点半", "fonts/STKaiti.ttf", 36);
//    createWithSystemFont("九点半", "Arial", 24);
    label->setTextColor(Color4B(0xff, 0xff, 0xff, 0xff));
//    label->enableShadow(Color4B::YELLOW, Size(2, -2)); //阴影
    label->enableOutline(Color4B::BLACK, 1);             //轮廓
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("images/fullScreen_homePage.jpg");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    
    sprite->setScaleX(visibleSize.width / spx); //设置精灵宽度缩放比例
    sprite->setScaleY(visibleSize.height / spy);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    auto login_WechatItem = MenuItemImage::create(
                                           "images/login_wechat.png",
                                           "images/login_wechat.png",
                                           CC_CALLBACK_1(HelloWorld::loginCallback, this, 0));
    
    login_WechatItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 1.5 * login_WechatItem->getContentSize().width,
                                origin.y + 0.1 * visibleSize.height));
    
    
    auto login_QQItem = MenuItemImage::create(
                                              "images/login_qq.png",
                                              "images/login_qq.png",
                                              CC_CALLBACK_1(HelloWorld::loginCallback, this, 1));
    
    login_QQItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 0.1 * visibleSize.height));
    
    auto login_SystemItem = MenuItemImage::create(
                                              "images/login_system.png",
                                              "images/login_system.png",
                                              CC_CALLBACK_1(HelloWorld::loginCallback, this, 2));
    
    login_SystemItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 1.5 * login_SystemItem->getContentSize().width, origin.y + 0.1 * visibleSize.height));
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(login_WechatItem, login_QQItem, login_SystemItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    return true;
}

void HelloWorld::loginCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            auto scene = Hall ::createScene();
            Director::getInstance()->pushScene(scene);
        }
            break;
            
        case 1:{
            
        }
            break;
            
        case 2:{
            auto scene = LoginScene::createScene();
            
            Director::getInstance()->pushScene(scene);
        }
            break;
            
        default:
            break;
    }
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
