//
//  InviterScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "InviterScene.h"
#include "QLImageSprite.h"
#include "Global.h"

USING_NS_CC;
using namespace ui;

InviterScene::InviterScene(){
}
InviterScene::~InviterScene(){
}

Scene* InviterScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = InviterScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool InviterScene::init()
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
                                              CC_CALLBACK_1(InviterScene::buttonCallback, this, 0));
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto inputListSprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.5 * visibleSize.width, 0.95 * visibleSize.height));
    inputListSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(inputListSprite);
    
    char showContent[1][100] = {"请输入邀请码"};
    float inputHeight = MAX(32, inputListSprite->getContentSize().height / 8);
    float inputY = 0.0;
    for (int i = 0; i < 1; i++) {
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
        
        //委托代理对象this
        inputBox->setDelegate(this);
        
        inputBox->setPlaceHolder(showContent[i]);
        switch (i) {
            case 0:{
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
                inputBox->setMaxLength(length_invite_code);
                usernameBox = inputBox;
            }
                break;
                
            default:
                break;
        }
    }
    
    auto btn_login = Button::create("images/btn_green.png","images/btn_green_selected.png");
    btn_login->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_login->setTitleText("绑定邀请人");
    btn_login->setTitleFontSize(16);
    btn_login->setContentSize(Size(0.9 * inputListSprite->getContentSize().width, 0.8 * inputHeight));
    btn_login->setPosition(Vec2(0.5 * inputListSprite->getContentSize().width, inputY - btn_login->getContentSize().height));
    btn_login->addTouchEventListener(CC_CALLBACK_2(InviterScene::touchEvent, this));
    btn_login->setTag(1);
    inputListSprite->addChild(btn_login);
    
    return true;
}

void InviterScene::onEnter(){
    Layer::onEnter();
    
    
}

void InviterScene::onExit(){
    Layer::onExit();
    
}

void InviterScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            
            break;
            
        default:{
        }
            break;
    }
}

void InviterScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 1:{
                    if (strlen(usernameBox->getText()) != length_invite_code) {
                        NoteTip::show("邀请码输入有误");
                    }
                    else if (0 == strcmp(usernameBox->getText(), Global::getInstance()->user_data.inviteCode)) {
                        NoteTip::show("自己不能邀请自己");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示                        
                        onHttpRequest_Inviter(usernameBox->getText());
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

#pragma edixBox
//开始编辑
void InviterScene::editBoxEditingDidBegin(ui::EditBox* editBox){
}

//结束编辑
void InviterScene::editBoxEditingDidEnd(ui::EditBox* editBox){
}

//编辑框内容改变
void InviterScene::editBoxTextChanged(ui::EditBox* editBox, const std::string& text){
}

//触发return返回
void InviterScene::editBoxReturn(ui::EditBox* editBox){
}

#pragma http
// 发送HTTP请求
void InviterScene::onHttpRequest_Inviter(const char* code){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/updateInviteCode");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s&inviteCode=%s", Global::getInstance()->user_data.ID, code);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(InviterScene::onHttpResponse, this));
    
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}


// HTTP响应请求函数
void InviterScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
                const rapidjson::Value& val_content = document["content"];
                if (val_content.IsObject()) {
                    if (val_content.HasMember("id")) {
                        strcpy(Global::getInstance()->user_data.inviteUser, val_content["id"].GetString());
                    }
                    PostRef* post = new PostRef();
                    sprintf(post->description, "绑定邀请人成功");
                    
                    MTNotificationQueue::sharedNotificationQueue()->postNotification(showNoteTipTag, post);
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

