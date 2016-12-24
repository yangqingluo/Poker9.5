//
//  HelpScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "HelpScene.h"
#include "Global.h"
#include "QLImageSprite.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelpScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void HelpScene::onEnter(){
    Layer::onEnter();
    
    showSettingWithIndex(100);
}
void HelpScene::onExit(){
    Layer::onExit();
}

bool HelpScene::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("images/cmdz_bank_bg.jpg");
    
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
                                              CC_CALLBACK_1(HelpScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = Label::createWithTTF("帮助", "fonts/STKaiti.ttf", 20);
    label->setTextColor(Color4B::BLACK);
    label->setPosition(origin.x + (560.0 / 960.0) * visibleSize.width, origin.y + (580.0 / 640.0) * visibleSize.height);
    this->addChild(label);
    
    auto listSprite = Sprite::create();
    listSprite->setContentSize(Size((680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height));
    listSprite->setPosition(origin.x + 0.95 * visibleSize.width - listSprite->getContentSize().width / 2, origin.y + (40.0 / 640.0 ) * visibleSize.height + listSprite->getContentSize().height / 2);
    this->addChild(listSprite, 1);
    
    char showTitle[3][100] = {"游戏简介","牌型","赔率"};
    for (int i = 0; i < 3; i++) {
        auto btn_list = YNButton::create();
        btn_list->configImage("images/bg_item_hide.png", "", "images/bg_item_show.png");
        btn_list->setScale9Enabled(true);
        btn_list->setTitleFontSize(16);
        btn_list->setTitleColor(Color3B::WHITE);
        btn_list->setTitleText(showTitle[i]);
        btn_list->setScale((160.0 / 960.0) * visibleSize.width / btn_list->getContentSize().width);
        btn_list->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.5 * visibleSize.height - (i * 1.2) * btn_list->getBoundingBox().size.height));
        btn_list->addTouchEventListener(CC_CALLBACK_2(HelpScene::touchEvent, this));
        btn_list->setTag(100 + i);
        this->addChild(btn_list);
        
        listButtons.pushBack(btn_list);
    }
    
    return true;
}


void HelpScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
        case 1:
        case 2:{
            showSettingWithIndex(index);
        }
            break;
            
        default:
            break;
    }
}

void HelpScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            if (button->getTag() >= 100) {
                this->showSettingWithIndex(button->getTag());
            }
            switch (button->getTag()) {
                case 1:
                case 2:{
                    
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

void HelpScene::showSettingWithIndex(int index){
    if (index >= 100) {
        for (int i = 0; i < listButtons.size(); i++) {
            YNButton* button = listButtons.at(i);
            button->setSelected(button->getTag() == index);
        }
    }
}

#pragma http
void HelpScene::onHttpRequest_SettingNikename(string nikename){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/updateinfo");
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "account=%s&nikename=%s", Global::getInstance()->user_data.account, nikename.c_str());
    std::string data;
    data.assign(param);
    request->setRequestData(data.c_str(), data.length());
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(HelpScene::onHttpResponse, this));
    request->setTag("updatenikename");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

void HelpScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
                    if (tag == "updatenikename") {
                        NoteTip::show("修改成功");
                        memcpy(Global::getInstance()->user_data.nikename, nikenameBox->getText(), strlen(nikenameBox->getText()));
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

