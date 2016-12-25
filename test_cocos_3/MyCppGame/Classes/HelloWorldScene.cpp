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
    sprite->setScale(visibleSize.width / spx, visibleSize.height / spy);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
    
    
    auto login_WechatItem = MenuItemImage::create(
                                           "images/login_wechat.png",
                                           "images/login_wechat.png",
                                           CC_CALLBACK_1(HelloWorld::loginCallback, this, 0));
    if (login_WechatItem->getContentSize().width > 0.2 * visibleSize.width) {
        login_WechatItem->setScale(0.2 * visibleSize.width / login_WechatItem->getContentSize().width);
    }
    
    login_WechatItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 1.5 * login_WechatItem->getBoundingBox().size.width,
                                origin.y + 0.1 * visibleSize.height));
    
    
    auto login_QQItem = MenuItemImage::create(
                                              "images/login_qq.png",
                                              "images/login_qq.png",
                                              CC_CALLBACK_1(HelloWorld::loginCallback, this, 1));
    login_QQItem->setScale(login_WechatItem->getScale());
    login_QQItem->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + 0.1 * visibleSize.height));
    
    auto login_SystemItem = MenuItemImage::create(
                                              "images/login_system.png",
                                              "images/login_system.png",
                                              CC_CALLBACK_1(HelloWorld::loginCallback, this, 2));
    login_SystemItem->setScale(login_WechatItem->getScale());
    login_SystemItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 1.5 * login_SystemItem->getBoundingBox().size.width, origin.y + 0.1 * visibleSize.height));
    
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(login_WechatItem, login_QQItem, login_SystemItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
//    Label* txt = Label::createWithTTF("大江东去浪淘尽，千古风流人物，故垒西边，人道是，三国周郎赤壁。乱石穿空，惊涛拍岸，卷起千堆雪。江山如画，一时多少豪杰。\n遥想公瑾当年，小乔初嫁了，雄姿英发，羽扇纶巾。谈笑间，樯橹灰飞烟灭。故国神游，多情应笑我，早生华发。人生如梦，一樽还酹江月。","fonts/STKaiti.ttf",30);
//    txt->setColor(Color3B::RED);
//    txt->setPosition(Vec2(visibleSize.width + origin.x + txt->getContentSize().width / 2, visibleSize.height / 2 + origin.y));
//    this->addChild(txt);
//
//    float width = visibleSize.width + txt->getContentSize().width;
//    MoveBy* to = MoveBy::create(0.01 * width, Vec2(-width, 0));
//    CallFunc* func1 = CallFunc::create([=]{
//        txt->setPositionX(visibleSize.width + origin.x + txt->getContentSize().width / 2);
//    });
//    txt->runAction(RepeatForever::create(Sequence::create(to, func1, NULL)));
    
//    //使用两张图片分别创建精灵
//    auto logo1 = Sprite::create("card/card_bg.png");
//    auto logo2 = Sprite::create("card/1.jpg");
//    //设置居中
//    logo1->setPosition(visibleSize/2);
//    logo2->setPosition(visibleSize/2);
//    //将第二张图片x轴缩放至隐藏
//    logo2->setScale(0, 1);
//    //加入图层
//    addChild(logo1);
//    addChild(logo2);
//    
//    //创建监听器
//    auto touchListener1 = EventListenerTouchOneByOne::create();
//    //设置点击事件
//    touchListener1->onTouchBegan = [logo1, logo2](Touch* touch, Event* event){
//        //判断点击位置是否在精灵显示范围内
//        if(event->getCurrentTarget()->getBoundingBox().containsPoint(
//                                                                     touch->getLocation()))
//        {
//            //执行缩放动作
//            auto move1 = ScaleTo::create(0.1, 0, 1);
//            auto move2 = ScaleTo::create(0.1, 1, 1);
//            logo1->runAction(move1);
//            logo2->runAction(move2);
//        }
//        return false;
//    };
//    //同上
//    auto touchListener2 = EventListenerTouchOneByOne::create();
//    touchListener2->onTouchBegan = [logo1, logo2](Touch* touch, Event* event){
//        if(event->getCurrentTarget()->getBoundingBox().containsPoint(
//                                                                     touch->getLocation()))
//        {
//            auto move1 = ScaleTo::create(0.1, 0, 1);
//            auto move2 = ScaleTo::create(0.1, 1, 1);
//            logo2->runAction(move1);
//            logo1->runAction(move2);
//        }
//        return false;
//    };
//    
//    //分别将两个监听器与两个精灵绑定
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener1, logo1);
//    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener2, logo2);

    
    return true;
}

void HelloWorld::loginCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            NoteTip::show("精彩功能敬请期待");
        }
            break;
            
        case 1:{
            NoteTip::show("精彩功能敬请期待");
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
}


// Socker连接
void HelloWorld::connectServer()
{
    // 初始化
    // ODSocket socket;
    socket.Init();
    socket.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 设置服务器的IP地址，端口号
    // 并连接服务器 Connect
    const char* ip = "222.128.13.159";
    int port = 8989;
    bool result = socket.Connect(ip, port);
    
    if (result) {
        //发送数据 Send
        SEND_PACKAGE package = {0};
        const char* handle = "{\"id\":1000}";
        int length = (int)strlen(handle);
        package.valueLength = reversebytes_uint32t(length);
        memcpy(package.value, handle, package.valueLength);
        
        socket.Send((const char *)&package, sizeof(int) + length);
        CCLOG("connect to server success!");
        // 开启新线程，在子线程中，接收数据
        std::thread recvThread = std::thread(&HelloWorld::receiveData, this);
        recvThread.detach(); // 从主线程分离
    }
    else {
        CCLOG("can not connect to server");
        return;
    }
}

// 接收数据
void HelloWorld::receiveData()
{
    // 因为是强联网
    // 所以可以一直检测服务端是否有数据传来
    while (true) {
        // 接收数据 Recv
        char data[512] = "";
        int result = socket.Recv(data, 512, 0);
        printf("%d", result);
        // 与服务器的连接断开了
        if (result <= 0) break;
        
        CCLOG("%s", data);
    }
    // 关闭连接
    socket.Close();
}

// 发送HTTP请求
void HelloWorld::onHttpRequest(std::string type)
{
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    if (type == "get")
    {
        request->setRequestType(HttpRequest::Type::GET);
        // url后面附加数据信息
        request->setUrl("http://115.28.109.174:8181/game/user/login?username=15928622486&password=123456");
    }
    else if(type == "post")
    {
        request->setRequestType(HttpRequest::Type::POST);
        request->setUrl("http://httpbin.org/post");
        // 设置post发送请求的数据信息
        std::string data = "hello world!";
        request->setRequestData(data.c_str(), data.length());
    }
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpResponse, this));
    
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}


// HTTP响应请求函数
void HelloWorld::onHttpResponse(HttpClient* sender, HttpResponse* response)
{
    // 没有收到响应
    if (!response)
    {
        CCLOG("no response");
        return;
    }
    
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLOG("response code: %s", statusString);
    
    // 链接失败
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    // 获取数据
    std::vector<char>* v = response->getResponseData();
    for (int i = 0; i < v->size(); i++)
    {
        printf("%c", v->at(i));
    }
    printf("\n");
}

