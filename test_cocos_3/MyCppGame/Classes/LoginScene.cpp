//
//  LoginScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "LoginScene.h"
#include "RegistInputView.h"
#include "QLImageSprite.h"
#include "RegistScene.h"
#include "PasswordScene.h"
#include "HallScene.h"
#include "Global.h"

USING_NS_CC;
using namespace ui;

LoginScene::LoginScene(){
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(LoginScene::onNotification_NoteTip), showNoteTipTag, NULL);
}
LoginScene::~LoginScene(){
    NotificationCenter::getInstance()->removeAllObservers(this);
}

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
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    sprite->setScaleX(visibleSize.width / spx);
    sprite->setScaleY(visibleSize.height / spy);
    this->addChild(sprite, 0);
    
    auto btn_BackItem = MenuItemImage::create(
                                              "images/window_close@2x.png",
                                              "images/window_close@2x.png",
                                              CC_CALLBACK_1(LoginScene::buttonCallback, this, 0));
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto inputListSprite = QLImageSprite::create("images/window_upright_bg.png", Size(0.5 * visibleSize.width, 0.95 * visibleSize.height));
    inputListSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(inputListSprite);
    
    char showContent[2][100] = {"手机号码(11位)","密码(6-16位)"};
    float inputHeight = MAX(32, inputListSprite->getContentSize().height / 8);
    float inputY = 0.0;
    for (int i = 0; i < 2; i++) {
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
                
            default:
                break;
        }
    }
    
    auto btn_login = Button::create("images/btn_green.png","images/btn_green_selected.png");
    btn_login->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_login->setTitleText("登录");
    btn_login->setTitleFontSize(16);
    btn_login->setContentSize(Size(0.9 * inputListSprite->getContentSize().width, 0.8 * inputHeight));
    btn_login->setPosition(Vec2(0.5 * inputListSprite->getContentSize().width, inputY - btn_login->getContentSize().height));
    btn_login->addTouchEventListener(CC_CALLBACK_2(LoginScene::touchEvent, this));
    btn_login->setTag(1);
    inputListSprite->addChild(btn_login);
    
    
    auto btn_GoToRegist = MenuItemFont::create("手机注册", CC_CALLBACK_1(LoginScene::buttonCallback, this, 2));
    btn_GoToRegist->setFontSizeObj(12);
    btn_GoToRegist->setColor(Color3B::GRAY);
    btn_GoToRegist->setPosition(Vec2(btn_login->getBoundingBox().getMinX() + 0.5 * btn_GoToRegist->getContentSize().width, btn_login->getBoundingBox().getMinY() -  btn_GoToRegist->getContentSize().height));
    
    auto btn_GoToPassword = MenuItemFont::create("忘记密码", CC_CALLBACK_1(LoginScene::buttonCallback, this, 3));
    btn_GoToPassword->setFontSizeObj(12);
    btn_GoToPassword->setColor(Color3B::GRAY);
    btn_GoToPassword->setPosition(Vec2(btn_login->getBoundingBox().getMaxX() - 0.5 * btn_GoToPassword->getContentSize().width, btn_GoToRegist->getPositionY()));
    
    auto menu_input = Menu::create(btn_GoToRegist, btn_GoToPassword, NULL);
    menu_input->setPosition(Vec2::ZERO);
    inputListSprite->addChild(menu_input, 1);
    
    return true;
}

void LoginScene::onEnter(){
    Layer::onEnter();
    
    std::string username = UserDefault::getInstance()->getStringForKey("username");
    if (strlen(username.c_str()) == 11) {
        usernameBox->setText(username.c_str());
    }
}

void LoginScene::onExit(){
    Layer::onExit();
    
}

void LoginScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            
            break;
            
        case 2:{
            auto scene = RegistScene::createScene();
            Director::getInstance()->pushScene(scene);
        }
            break;
            
        case 3:{
            auto scene = PasswordScene::createScene();
            Director::getInstance()->pushScene(scene);
        }
            break;
            
        default:{
        }
            break;
    }
}

void LoginScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
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
                    else if (strlen(passwordBox->getText()) < 6) {
                        NoteTip::show("密码输入有误");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        
                        onHttpRequest_Login(usernameBox->getText(), passwordBox->getText());
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
void LoginScene::editBoxEditingDidBegin(ui::EditBox* editBox){
}

//结束编辑
void LoginScene::editBoxEditingDidEnd(ui::EditBox* editBox){
}

//编辑框内容改变
void LoginScene::editBoxTextChanged(ui::EditBox* editBox, const std::string& text){
}

//触发return返回
void LoginScene::editBoxReturn(ui::EditBox* editBox){
}

#pragma http
// 发送HTTP请求
void LoginScene::onHttpRequest_Login(string username, string password){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::GET);
    // url后面附加数据信息
    char url[200] = {0};
    sprintf(url, "http://115.28.109.174:8181/game/user/login?username=%s&password=%s", username.c_str(), password.c_str());
    request->setUrl(url);
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(LoginScene::onHttpResponse, this));
    
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}


// HTTP响应请求函数
void LoginScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
    
//    // 获取数据
//    std::vector<char>* v = response->getResponseData();
//    for (int i = 0; i < v->size(); i++)
//    {
//        printf("%c", v->at(i));
//    }
//    printf("\n");
    
    
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
                //登录成功
                UserDefault::getInstance()->setStringForKey("username", usernameBox->getText());
                if (document.HasMember("content")) {
                    Global::getInstance()->saveLoginData(document["content"]);
                }
            }
            else {
                const rapidjson::Value& val_content = document["content"];
                const char* content = val_content.GetString();
                NoteTip::show(content);
            }
        }
        
//        if(document.HasMember("data")){
//            const rapidjson::Value& val_data = document["data"];
//            log("data: %s \n", val_data.GetString());
//        }
//        
//        if(document.HasMember("files")){
//            const rapidjson::Value& val_files = document["files"];
//            if(val_files.IsObject()){
//                log("files: \n");
//            }
//            
//        }
//
//        if(document.HasMember("json")){
//            const rapidjson::Value& val_form = document["json"];
//            if(val_form.IsObject()){
//                log("json:{");
//                if(val_form.HasMember("ID")){
//                    log("   ID: %d", val_form["ID"].GetInt());
//                }
//                
//                if(val_form.HasMember("info")){
//                    const rapidjson::Value& info = val_form["info"];
//                    CC_ASSERT(info.IsArray());
//                    log("   info: { ");
//                    for(unsigned int i = 0; i < info.Size(); ++i){
//                        // 获得一条记录对象
//                        const rapidjson::Value& record = info[i];
//                        CC_ASSERT(record.HasMember("no"));
//                        log("       no: %d", record["no"].GetInt());
//                        
//                        CC_ASSERT(record.HasMember("content"));
//                        log("       content: %s", record["content"].GetString());
//                        
//                    }
//                }
//                
//                log("   } \n");
//                
//            }
//        }
//        
//        if(document.HasMember("origin")){
//            const rapidjson::Value& val_visitor = document["origin"];
//            log("origin: %s \n", val_visitor.GetString());
//        }
//        
//        if(document.HasMember("url")){
//            const rapidjson::Value& val_headers = document["url"];
//            log("url: %s \n", val_headers.GetString());
//        }
        
    }
}

#pragma notification
void LoginScene::onNotification_NoteTip(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    NoteTip::show(this, post->description);
}

//#pragma tableview
//Size LoginScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
//{
//    if (table == inputListTableView) {
//        return InputCell::tableCellSizeForIndex(table, idx);
//    }
//    
//    return Size::ZERO;
//}
//
////定制每个cell的内容
//TableViewCell* LoginScene::tableCellAtIndex(TableView* table, ssize_t idx)
//{
//    if (table == inputListTableView) {
//        InputCell* cell = (InputCell* )table->dequeueCell();
//        
//        if(!cell)
//        {
//            cell = InputCell::create();
//            cell->setContentSize(tableCellSizeForIndex(table, idx));
//        }
//        
//        LoginShowItem* item = showItems.at(idx);
//        cell->inputBox->setPlaceHolder(item->content);
//        if (idx == 2 || idx == 3) {
//            cell->inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::PASSWORD);
//        }
//        else {
//            cell->inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
//        }
//        
//        return cell;
//    }
//    
//    return NULL;
//}
//
////确定这个tableview的cell行数
//ssize_t LoginScene::numberOfCellsInTableView(TableView* table)
//{
//    if (table == inputListTableView) {
//        return showItems.size();
//    }
//    
//    return 0;
//}
//
//void LoginScene::tableCellTouched(TableView* table, TableViewCell* cell){
//    
//}
//
//#pragma scrollview
//void LoginScene::scrollViewDidScroll(cocos2d::extension::ScrollView* view){
//    
//}
//void LoginScene::scrollViewDidZoom(cocos2d::extension::ScrollView* view){
//    
//}
