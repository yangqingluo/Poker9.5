//
//  RegistScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "RegistScene.h"
#include "RegistInputView.h"
#include "QLImageSprite.h"
#include "HallScene.h"


USING_NS_CC;
using namespace ui;

Scene* RegistScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = RegistScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool RegistScene::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("images/new_bg.jpg");
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    sprite->setScaleX(visibleSize.width / spx);
    sprite->setScaleY(visibleSize.height / spy);
    this->addChild(sprite, 0);
    
    auto btn_BackItem = MenuItemImage::create(
                                              "images/window_close@2x.png",
                                              "images/window_close@2x.png",
                                              CC_CALLBACK_1(RegistScene::buttonCallback, this, 0));
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto inputListSprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.5 * visibleSize.width, 0.95 * visibleSize.height));
    inputListSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(inputListSprite);
    
    char showContent[5][100] = {"手机号码(11位)","密码(6-16位)","确认密码","昵称","验证码"};
    float inputHeight = MAX(32, inputListSprite->getContentSize().height / 8);
    float inputY = 0.0;
    for (int i = 0; i < 5; i++) {
        auto inputBox = ui::EditBox::create(Size(0.9 * inputListSprite->getContentSize().width, 0.8 * inputHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
        inputBox->setPosition(Vec2(0.5 * inputListSprite->getContentSize().width, inputListSprite->getContentSize().height - (i * 1.0 + 0.8) * inputHeight));
        inputListSprite->addChild(inputBox);
        inputY = inputBox->getBoundingBox().getMinY();
        
        //属性设置
        //    inputBox->setFontName("fonts/STKaiti.ttf");
        inputBox->setFontSize(12);
        inputBox->setFontColor(Color4B::BLACK);
        //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
        inputBox->setPlaceholderFontSize(12);
        inputBox->setPlaceholderFontColor(Color4B::GRAY);
        
        //模式类型设置
        inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
        inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
        inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
        
        inputBox->setPlaceHolder(showContent[i]);
        switch (i) {
            case 0:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                inputBox->setMaxLength(11);
                usernameBox = inputBox;
            }
                break;
                
            case 1:{
                inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
                inputBox->setMaxLength(16);
                passwordBox = inputBox;
            }
                break;
                
            case 2:{
                inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
                inputBox->setMaxLength(16);
                passwordAgainBox = inputBox;
            }
                break;
                
            case 3:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
                inputBox->setMaxLength(20);
                nikenameBox = inputBox;
            }
                break;
                
            case 4:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                inputBox->setMaxLength(6);
                vcodeBox = inputBox;
                
                inputBox->setContentSize(Size(0.45 * inputBox->getContentSize().width, inputBox->getContentSize().height));
                inputBox->setPositionX(0.05 * inputListSprite->getContentSize().width + 0.5 * inputBox->getContentSize().width);
                
                btn_getVCode = Button::create("images/btn_green.png","images/btn_green_selected.png");
                btn_getVCode->setScale9Enabled(true);//打开scale9 可以拉伸图片
                btn_getVCode->setTitleText("获取验证码");
                btn_getVCode->setTitleFontSize(12);
                btn_getVCode->setContentSize(inputBox->getContentSize());
                btn_getVCode->setPosition(Vec2(0.95 * inputListSprite->getContentSize().width - 0.5 * btn_getVCode->getContentSize().width, inputBox->getPositionY()));
                btn_getVCode->addTouchEventListener(CC_CALLBACK_2(RegistScene::touchEvent, this));
                btn_getVCode->setTag(1);
                inputListSprite->addChild(btn_getVCode);
                
            }
                break;
                
            default:
                break;
        }
    }
    
    auto btn_changePassword = Button::create("images/btn_green.png","images/btn_green_selected.png");
    btn_changePassword->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_changePassword->setTitleText("注册");
    btn_changePassword->setTitleFontSize(12);
    btn_changePassword->setContentSize(Size(0.9 * inputListSprite->getContentSize().width, 0.8 * inputHeight));
    btn_changePassword->setPosition(Vec2(0.5 * inputListSprite->getContentSize().width, inputY - btn_changePassword->getContentSize().height));
    btn_changePassword->addTouchEventListener(CC_CALLBACK_2(RegistScene::touchEvent, this));
    btn_changePassword->setTag(2);
    inputListSprite->addChild(btn_changePassword);
    
    return true;
}

void RegistScene::onEnter(){
    Layer::onEnter();
    
}

void RegistScene::onExit(){
    Layer::onExit();
    
}

void RegistScene::buttonCallback(cocos2d::Ref* pSender, int index){
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

void RegistScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 1:{
                    if (strlen(usernameBox->getText()) != 11) {
                        NoteTip::show("手机号码输入有误");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_GetVCode(usernameBox->getText());
                    }
                }
                    break;
                    
                case 2:{
                    if (strlen(usernameBox->getText()) != 11) {
                        NoteTip::show("手机号码输入有误");
                    }
                    else if (strlen(passwordBox->getText()) < 6) {
                        NoteTip::show("密码输入有误");
                    }
                    else if (0 != strcmp(passwordBox->getText(), passwordAgainBox->getText())) {
                        NoteTip::show("两次密码输入不一致");
                    }
                    else if (strlen(passwordBox->getText()) == 0) {
                        NoteTip::show("请输入昵称");
                    }
                    else if (strlen(vcodeBox->getText()) != 6) {
                        NoteTip::show("验证码输入有误");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_Regist(usernameBox->getText(), passwordBox->getText(), vcodeBox->getText(), nikenameBox->getText());
                    }
                }
                    break;
                    
                default:
                    break;
            }
            break;
            
        case Widget::TouchEventType::CANCELED:{
            
        }
            break;
            
        default:
            break;
    }
}

void RegistScene::wait(float delta){
    pTime -= delta;
    char mtime[100];
    sprintf(mtime,"%ds重新获取",(int)pTime % 60);
    btn_getVCode->setTitleText(mtime);
    
    if (pTime <= 1) {
        unschedule(schedule_selector(RegistScene::wait));
        btn_getVCode->setTouchEnabled(true);
        btn_getVCode->setTitleText("获取验证码");
    }
}

#pragma http
void RegistScene::onHttpRequest_GetVCode(string username){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/getverifycode");
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "mobile=%s", username.c_str());
    std::string data;
    data.assign(param);
    request->setRequestData(data.c_str(), data.length());
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(RegistScene::onHttpResponse, this));
    request->setTag("getverifycode");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}
void RegistScene::onHttpRequest_Regist(string username, string password, string vcode, string nikename){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/register");
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "mobile=%s&verifyCode=%s&password=%s&nikename=%s", username.c_str(), vcode.c_str() ,password.c_str(), nikename.c_str());
    std::string data;
    data.assign(param);
    request->setRequestData(data.c_str(), data.length());
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(RegistScene::onHttpResponse, this));
    request->setTag("register");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}
void RegistScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
    m_pMessage->hidden();
    
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
                //成功
                if (0 != strlen(response->getHttpRequest()->getTag())){
                    std::string tag = response->getHttpRequest()->getTag();
                    if (tag == "getverifycode") {
                        btn_getVCode->setTouchEnabled(false);
                        pTime = 60;
                        schedule(schedule_selector(RegistScene::wait));
                    }
                    else if (tag == "register"){
                        UserDefault::getInstance()->setStringForKey("username", usernameBox->getText());
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

