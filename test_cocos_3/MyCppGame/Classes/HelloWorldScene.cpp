#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "LoginScene.h"
#include "HallScene.h"
#include "Cocos2dx/Common/CCUMSocialSDK.h"
USING_NS_UM_SOCIAL;

USING_NS_CC;

int getHelloWorldTag = 444;

HelloWorld::HelloWorld(){
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(HelloWorld::onNotification_Socket), kNotification_Socket, NULL);
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(HelloWorld::onNotification_LoginQQ), kNotification_LoginQQ, NULL);
}
HelloWorld::~HelloWorld(){
    NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    layer->setTag(getHelloWorldTag);
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
/*
 *授权回调
 * @param platform 要授权的平台
 * @param stCode 返回码, 200代表授权成功, 100代表开始授权, 0代表授权出错, -1代表取消授权
 * @param data 授权时返回的数据
 */
void authCallback(int platform, int stCode, map<string, string>& data) {
    log("#### 授权回调");
    string result = "";
    if (stCode == 200) {
        log("#### 授权完成");
        result = "授权完成";
        
        map<string, string>::iterator it = data.begin();
        
        for (; it != data.end(); ++it) {
            log("#### data  %s -> %s.", it->first.c_str(), it->second.c_str());
            
        }
        
        //           	item->setString("auth or delete success");
    } else if (stCode == 0) {
        //    	item->setString("auth or delete fail");
        result = "授权出错";
        log("#### 授权出错");
    } else if (stCode == -1) {
        //    	item->setString("auth or delete cancel");
        result = "取消授权";
        log("#### 取消授权");
    }else {
    	   log("#### 未知类型");
    }
}

void getCallback(int platform, int stCode, map<string, string>& data) {
    PostRef* post = new PostRef();
    post->cmd = stCode;
    memset(post->description, 0, sizeof(post->description));
    
    if (stCode == 200) {
        char uid[Max_ID_Length] = {0};
        char name[Max_Name_Length] = {0};
        char accessToken[Max_ID_Length] = {0};
        
        // 输入授权数据, 如果授权失败,则会输出错误信息
        map<string, string>::iterator it = data.begin();
        for (; it != data.end(); ++it) {
            log("#### data  %s -> %s.", it->first.c_str(), it->second.c_str());
            if (0 == strcmp(it->first.c_str(), "name")) {
                strcpy(name, it->second.c_str());
            }
            
            if (0 == strcmp(it->first.c_str(), "uid")) {
                strcpy(uid, it->second.c_str());
            }
            
            if (0 == strcmp(it->first.c_str(), "accessToken")) {
                strcpy(accessToken, it->second.c_str());
            }
            
            if (strlen(name) > 0 && strlen(uid) > 0 && strlen(accessToken) > 0 ) {
                sprintf(post->description, "uid=%s&name=%s&accessToken=%s", uid, name, accessToken);
            }
        }
    }
//    else {
//        NoteTip::show("登陆失败");
//    }
    
    HelloWorld* hwLayer =(HelloWorld* ) Director::getInstance()->getRunningScene()->getChildByTag(
                                                                                   getHelloWorldTag);
    hwLayer->onNotification_LoginQQ(post);
//    MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_LoginQQ, post);
}

void HelloWorld::loginCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            CCUMSocialSDK *sdk = CCUMSocialSDK::create( );
            sdk->authorize(WEIXIN, auth_selector(authCallback));
        }
            break;
            
        case 1:{
            CCUMSocialSDK *sdk = CCUMSocialSDK::create( );
//            sdk->authorize(QQ, auth_selector(authCallback));
            sdk->getPlatformInfo(QQ, auth_selector(getCallback));
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

// 发送HTTP请求
void HelloWorld::onHttpRequest_LoginQQ(char* m_string)
{
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/qqlogin");
    
    // 设置post发送请求的数据信息
    request->setRequestData(m_string, strlen(m_string));
    
//    request->setRequestType(HttpRequest::Type::GET);
//    // url后面附加数据信息
//    char url[200] = {0};
//    sprintf(url, "http://115.28.109.174:8181/game/user/qqlogin?uid=B40B57B7CEB780C9A8DDA51F79AB7324&name=fighting&accessToken=0DD873B16CA6865B462EA3AEE654393E&iconurl=http://q.qlogo.cn/qqapp/1105893963/B40B57B7CEB780C9A8DDA51F79AB7324/100");
//    request->setUrl(url);
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(HelloWorld::onHttpResponse, this));
    request->setTag("loginqq");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}


// HTTP响应请求函数
void HelloWorld::onHttpResponse(HttpClient* sender, HttpResponse* response)
{
    if (m_pMessage != NULL) {
        m_pMessage->hidden();
        m_pMessage = NULL;
    }
    
    // 没有收到响应
    if (!response){
        NoteTip::show("请检查网络");
        return;
    }
    
    long statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %ld, tag = %s", statusCode, response->getHttpRequest()->getTag());
    CCLOG("response code: %s", statusString);
    
    if (statusCode > 200) {
        NoteTip::show("网络错误");
        return;
    }
    // 链接失败
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        NoteTip::show("请检查网络");
        return;
    }
    
    std::vector<char>* responseData = response -> getResponseData();
    std::string responseStr = std::string(responseData -> begin(), responseData -> end());
    log("%s\n", responseStr.c_str());
    rapidjson::Document document;
    document.Parse<0>(responseStr.c_str());
    CCASSERT(!document.HasParseError(), "Parsing to document failed");
    
    if(document.IsObject()){
        if(document.HasMember("code")){
            const rapidjson::Value& val_code = document["code"];
            int code = val_code.GetInt();
            if (code == 1) {
                if (0 != strlen(response->getHttpRequest()->getTag())){
                    std::string tag = response->getHttpRequest()->getTag();
                    if (tag == "loginqq") {
                        if (document.HasMember("content")) {
                            Global::getInstance()->saveLoginData(document["content"]);
                        }
                    }
                }
            }
            else {
                const rapidjson::Value& val_content = document["content"];
                const char* content = val_content.GetString();
                NoteTip::show(content);
            }
        }
    }
}

#pragma notification
void HelloWorld::onNotification_Socket(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    switch (post->cmd) {
        case cmd_disconnect:{
            if (Global::getInstance()->isInitiativeLogout) {
                Global::getInstance()->isInitiativeLogout = false;
            }
            else {
                NoteTip::show(this, "与服务器连接已断开");
            }
        }
            break;
            
        default:
            break;
    }
}

void HelloWorld::onNotification_LoginQQ(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    if (post->cmd == 200) {
        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
        onHttpRequest_LoginQQ(post->description);
    }
    else {
        NoteTip::show(this, "登陆失败");
    }
}

