//
//  SettingScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "SettingScene.h"
#include "Global.h"
#include "QLImageSprite.h"

USING_NS_CC;

Scene* SettingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void SettingScene::onEnter(){
    Layer::onEnter();
    
    showSettingWithIndex(1);
}
void SettingScene::onExit(){
    Layer::onExit();
}

bool SettingScene::init()
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
                                              CC_CALLBACK_1(SettingScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    btn_infoSetting = YNButton::create();
    btn_infoSetting->configImage("images/btn_setting_userinfo.png", "", "images/btn_setting_userinfo_selected.png");
    btn_infoSetting->setScale9Enabled(true);
    btn_infoSetting->setScale((160.0 / 960.0) * visibleSize.width / btn_infoSetting->getContentSize().width);
    btn_infoSetting->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.3 * visibleSize.height + btn_infoSetting->getBoundingBox().size.height / 2));
    btn_infoSetting->addTouchEventListener(CC_CALLBACK_2(SettingScene::touchEvent, this));
    btn_infoSetting->setTag(1);
    this->addChild(btn_infoSetting);
    
    btn_systemSetting= YNButton::create();
    btn_systemSetting->configImage("images/btn_setting_system.png", "", "images/btn_setting_system_selected.png");
    btn_systemSetting->setScale9Enabled(true);
    btn_systemSetting->setScale(btn_infoSetting->getScale());
    btn_systemSetting->setPosition(Vec2(btn_infoSetting->getPositionX(), btn_infoSetting->getBoundingBox().getMinY() - btn_systemSetting->getBoundingBox().size.height));
    btn_systemSetting->addTouchEventListener(CC_CALLBACK_2(SettingScene::touchEvent, this));
    btn_systemSetting->setTag(2);
    this->addChild(btn_systemSetting);
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = Label::createWithTTF("设置", "fonts/STKaiti.ttf", 20);
    label->setTextColor(Color4B::BLACK);
    label->setPosition(origin.x + (560.0 / 960.0) * visibleSize.width, origin.y + (580.0 / 640.0) * visibleSize.height);
    this->addChild(label);
    
    auto listSprite = Sprite::create();
    listSprite->setContentSize(Size((680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height));
    listSprite->setPosition(origin.x + 0.95 * visibleSize.width - listSprite->getContentSize().width / 2, origin.y + (40.0 / 640.0 ) * visibleSize.height + listSprite->getContentSize().height / 2);
    this->addChild(listSprite, 1);
    
    infoSettingLayer = LayerColor::create(Color4B(0, 0, 0, 0), (680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height);
    infoSettingLayer->setPosition(0, 0);
    listSprite->addChild(infoSettingLayer);
    
    systemSettingLayer = LayerColor::create(Color4B(0, 0, 0, 0), (680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height);
    systemSettingLayer->setPosition(0, 0);
    listSprite->addChild(systemSettingLayer);
    
    float inputHeight = btn_infoSetting->getBoundingBox().size.height;
    
    
    auto inputBox = ui::EditBox::create(Size(0.6 * infoSettingLayer->getContentSize().width, inputHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
    inputBox->setPosition(Vec2(0.3 * infoSettingLayer->getContentSize().width, infoSettingLayer->getContentSize().height - inputHeight));
    infoSettingLayer->addChild(inputBox);
    
    //属性设置
    //    inputBox->setFontName("fonts/STKaiti.ttf");
    inputBox->setFontSize(12);
    inputBox->setFontColor(Color4B::BLACK);
    //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
    inputBox->setPlaceholderFontSize(12);
    inputBox->setPlaceholderFontColor(Color4B::GRAY);
    
    //模式类型设置
    inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
    inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
    inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    
    inputBox->setPlaceHolder("输入昵称");
    inputBox->setMaxLength(20);
    nikenameBox = inputBox;
    
    auto btn_changeNikename = Button::create("images/btn_green.png","images/btn_green_selected.png");
    btn_changeNikename->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_changeNikename->setTitleText("修改昵称");
    btn_changeNikename->setTitleFontSize(12);
    btn_changeNikename->setContentSize(Size(0.3 * infoSettingLayer->getContentSize().width, inputBox->getContentSize().height));
    btn_changeNikename->setPosition(Vec2(0.95 * infoSettingLayer->getContentSize().width - 0.5 * btn_changeNikename->getContentSize().width, inputBox->getPositionY()));
    btn_changeNikename->addTouchEventListener(CC_CALLBACK_2(SettingScene::touchEvent, this));
    btn_changeNikename->setTag(10);
    infoSettingLayer->addChild(btn_changeNikename);
    
    char showContent[2][100] = {"音效","背景音乐"};
    float cellHeight = MAX(60, systemSettingLayer->getContentSize().height / 5);
    for (int i = 0; i < 2; i++) {
        auto cellBG = QLImageSprite::create("images/white_cell_bg.png", Size(0.9 * systemSettingLayer->getContentSize().width, cellHeight));
        cellBG->setPosition(0.5 * systemSettingLayer->getContentSize().width, systemSettingLayer->getContentSize().height - (i * 1.0 + 0.8) * cellHeight);
        systemSettingLayer->addChild(cellBG);
        
        auto label = Label::createWithTTF(showContent[i], "fonts/STKaiti.ttf", 16);
        label->setTextColor(Color4B::BLACK);
        label->setPosition(0.3 * cellBG->getContentSize().width, 0.5 * cellBG->getContentSize().height);
        label->setHorizontalAlignment(TextHAlignment::LEFT);
        cellBG->addChild(label);
        
        auto btn_cell = YNButton::create();
        btn_cell->configImage("images/btn_switch_off.png", "", "images/btn_switch_on.png");
        btn_cell->setPosition(Vec2(cellBG->getContentSize().width - 0.8 * btn_cell->getContentSize().width, label->getPositionY()));
        btn_cell->setTag(i + 3);
        btn_cell->addTouchEventListener(CC_CALLBACK_2(SettingScene::touchEvent, this));
        cellBG->addChild(btn_cell);
        
        switch (i) {
            case 0:{
                btn_cell->setSelected(Global::getInstance()->isEffect());
            }
                break;
                
            case 1:{
                btn_cell->setSelected(Global::getInstance()->isBackgroundMusic());
            }
                break;
                
            default:
                break;
        }
    }
    
    return true;
}


void SettingScene::buttonCallback(cocos2d::Ref* pSender, int index){
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

void SettingScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 1:
                case 2:{
                    this->showSettingWithIndex(button->getTag());
                }
                    break;
                    
                case 3:{
                    YNButton* soundButton = (YNButton *)pSender;
                    if (Global::getInstance()->isEffect()) {
                        Global::getInstance()->setEffect(false);
                        soundButton->setSelected(false);
                    }
                    else {
                        Global::getInstance()->setEffect(true);
                        soundButton->setSelected(true);
                    }
                }
                    break;
                    
                case 4:{
                    YNButton* soundButton = (YNButton *)pSender;
                    if (Global::getInstance()->isBackgroundMusic()) {
                        Global::getInstance()->setBackgroundMusic(false);
                        soundButton->setSelected(false);
                    }
                    else {
                        Global::getInstance()->setBackgroundMusic(true);
                        soundButton->setSelected(true);
                    }
                }
                    break;
                    
                case 10:{
                    if (strlen(nikenameBox->getText()) == 0) {
                        NoteTip::show("请输入昵称");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_SettingNikename(nikenameBox->getText());
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

void SettingScene::showSettingWithIndex(int index){
    switch (index) {
        case 0:{
            
        }
            break;
            
        case 1:{
            btn_infoSetting->setSelected(true);
            infoSettingLayer->setVisible(true);
            
            btn_systemSetting->setSelected(false);
            systemSettingLayer->setVisible(false);
        }
            break;
            
        case 2:{
            btn_infoSetting->setSelected(false);
            infoSettingLayer->setVisible(false);
            
            btn_systemSetting->setSelected(true);
            systemSettingLayer->setVisible(true);
        }
            break;
            
        default:
            break;
    }
}

#pragma http
void SettingScene::onHttpRequest_SettingNikename(const char* nikename){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/updateinfo");
    
    
    size_t inlen = strlen(nikename);
    char * outbuf = new char[inlen * 2 + 2];
    Global::getInstance()->g2u((char* )nikename, inlen, outbuf, inlen * 2 + 2);
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "account=%s&nikename=%s", Global::getInstance()->user_data.account, outbuf);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(SettingScene::onHttpResponse, this));
    request->setTag("updatenikename");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

void SettingScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
                        memset(Global::getInstance()->user_data.nikename, 0, sizeof(Global::getInstance()->user_data.nikename));
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

