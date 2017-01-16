//
//  InviteScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "InviteScene.h"
#include "QLImageSprite.h"
#include "InviterScene.h"
#include "CppToFunction.h"

#include "Cocos2dx/Common/CCUMSocialSDK.h"

USING_NS_UM_SOCIAL;

USING_NS_CC;
using namespace ui;

Scene* InviteScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = InviteScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void InviteScene::onEnter(){
    Layer::onEnter();
    if (!hasGetInviteList) {
        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
        this->onHttpRequest_GetInviteList();
    }
}
void InviteScene::onExit(){
    Layer::onExit();
}

bool InviteScene::init()
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
                                              CC_CALLBACK_1(InviteScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    auto inviteSprite = Sprite::create();
    inviteSprite->setContentSize(Size((680.0 / 960.0) * visibleSize.width, (620.0 / 640.0) * visibleSize.height));
    inviteSprite->setPosition(origin.x + 0.95 * visibleSize.width - inviteSprite->getContentSize().width / 2, origin.y + (625.0 / 640.0 ) * visibleSize.height - inviteSprite->getContentSize().height / 2);
    this->addChild(inviteSprite, 1);
    
    
    
    auto inviteBG = QLImageSprite::create("images/share_bg_share.png", Size(MIN((720.0 / 609.0) * inviteSprite->getContentSize().height, inviteSprite->getContentSize().width), inviteSprite->getContentSize().height));
    inviteBG->setPosition(Vec2(inviteSprite->getBoundingBox().size.width / 2, inviteSprite->getBoundingBox().size.height / 2));
    inviteSprite->addChild(inviteBG);
    
    auto btn_copy = Button::create("images/share_btn_s_copy.png", "images/share_btn_s_copy_hover.png");
//    btn_copy->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_copy->setScale(0.18 * inviteBG->getContentSize().width / btn_copy->getContentSize().width);
//    btn_copy->setContentSize(Size(0.2 * inviteBG->getContentSize().width, (68.0 / 149.0) * 0.2 * inviteBG->getContentSize().width));
    btn_copy->setPosition(Vec2((680.0 / 720.0) * inviteBG->getContentSize().width - 0.5 * btn_copy->getContentSize().width, (450.0 / 609.0) * inviteBG->getContentSize().height));
    btn_copy->addTouchEventListener(CC_CALLBACK_2(InviteScene::touchEvent, this));
    btn_copy->setTag(1);
    inviteBG->addChild(btn_copy);
    
    auto btn_share = Button::create("images/share_btn_share.png", "images/share_btn_share_hover.png");
//    btn_share->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_share->setScale(btn_copy->getScale());
    btn_share->setPosition(Vec2(btn_copy->getPositionX(), (355.0 / 609.0) * inviteBG->getContentSize().height));
    btn_share->addTouchEventListener(CC_CALLBACK_2(InviteScene::touchEvent, this));
    btn_share->setTag(2);
    inviteBG->addChild(btn_share);
    
    auto btn_inviter = Button::create("images/share_btn_invite.png", "images/share_btn_invite_hover.png");
    btn_inviter->setScale(btn_copy->getScale());
    btn_inviter->setPosition(Vec2(btn_copy->getBoundingBox().getMaxX() - 0.5 * btn_inviter->getBoundingBox().size.width, (560.0 / 609.0) * inviteBG->getContentSize().height));
    btn_inviter->addTouchEventListener(CC_CALLBACK_2(InviteScene::touchEvent, this));
    btn_inviter->setTag(3);
    inviteBG->addChild(btn_inviter);
    
    auto btn_code = Button::create("images/bg_yellow_square.png");
    btn_code->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_code->setTitleText(Global::getInstance()->user_data.inviteCode);
    btn_code->setTitleFontSize(12);
    btn_code->setTitleColor(Color3B::BLACK);
    btn_code->setContentSize(btn_copy->getBoundingBox().size);
    btn_code->setPosition(Vec2(btn_copy->getBoundingBox().getMinX() - 0.6 * btn_code->getContentSize().width, btn_copy->getPositionY()));
    btn_code->setEnabled(false);
    inviteBG->addChild(btn_code);
    
    recordListCellWidth = (667.0 / 720.0) * inviteBG->getContentSize().width;
    recordListTableView = TableView::create(this, Size(recordListCellWidth,  (90.0 / 609.0) * inviteBG->getContentSize().height));
    recordListTableView->setPosition((30.0 / 720.0) * inviteBG->getContentSize().width , (20.0 / 609.0 ) * inviteBG->getContentSize().height);
    recordListTableView->setDirection(TableView::Direction::VERTICAL);
    recordListTableView->setDelegate(this);
    inviteBG->addChild(recordListTableView);
    
    return true;
}


void InviteScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
        case 1:{
            
        }
            break;
            
        case 2:{
            
        }
            break;
            
        default:
            break;
    }
}

void InviteScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 1:{
                    CppToFunction::getInstance()->doCopyAction(Global::getInstance()->user_data.inviteCode);
                    NoteTip::show("复制成功");
                }
                    break;
                    
                case 2:{
                    this->boardShare(pSender);
                }
                    break;
                    
                case 3:{
                    m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                    onHttpRequest_GetInviter();
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

/*
 * 分享回调
 * @param platform 要分享到的目标平台
 * @param stCode 返回码, 200代表分享成功, 100代表开始分享
 * @param errorMsg 分享失败时的错误信息,android平台没有错误信息
 */
void shareCallback(int platform, int stCode, string& errorMsg) {
    

    string result = "";
    if (stCode == 200) {
        result = "分享成功";
        log("#### HelloWorld 分享成功 --> Cocos2d-x SDK ");
    } else if (stCode == -1) {
        result = "分享取消";
    	   log("#### HelloWorld 分享取消 --> Cocos2d-x SDK ");
    }
    else {
        result = "分享失败";
        log("#### HelloWorld 分享出错 --> Cocos2d-x SDK ");
    }
    
    istringstream is;
    is >> platform;
    result.append(is.str());
    log("#### callback!!!!!! %s\n",result.c_str());
    
    log("platform num is : %d, %d", platform, stCode);
    
}

void InviteScene::boardShare(Ref* pSender) {
    CCUMSocialSDK *sdk = CCUMSocialSDK::create( );
    vector<int>* platforms = new vector<int>();
    platforms->push_back(QZONE);
    platforms->push_back(QQ);
    platforms->push_back(WEIXIN);
    platforms->push_back(WEIXIN_CIRCLE);
    
    sdk->openShare(platforms, Global::getInstance()->user_data.inviteCode, "邀请加入九点半" ,"https://dev.umeng.com/images/tab2_1.png","https://www.fushoulu95.com",share_selector(shareCallback));
}

#pragma tableview
Size InviteScene::tableCellSizeForIndex(TableView* table, ssize_t idx){
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 20);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* InviteScene::tableCellAtIndex(TableView* table, ssize_t idx){
    if (table == recordListTableView) {
        TableViewCell* cell = table->dequeueCell();
        
        if(!cell){
            cell = new TableViewCell();
            cell->autorelease();
            
            Label* titleLabel = Label::create("", "", 10);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(recordListCellWidth / 2, 10);
            titleLabel->setDimensions(recordListCellWidth, 20);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->addChild(titleLabel, 0 , 1);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
        char content[100] = {0};
        InviteItem* item = inviteItems.at(idx);
        sprintf(content, "%d.\t%s\t%s", (int)idx + 1, item->account, item->nikename);
        label->setString(content);
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t InviteScene::numberOfCellsInTableView(TableView* table){
    if (table == recordListTableView) {
        return inviteItems.size();
    }
    
    return 0;
}

void InviteScene::tableCellTouched(TableView* table, TableViewCell* cell){
    
}

#pragma http
// 发送HTTP请求
void InviteScene::onHttpRequest_GetInviteList(){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/getInviteUsers");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s", Global::getInstance()->user_data.ID);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(InviteScene::onHttpResponse, this));
    request->setTag("GetInviteList");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}
void InviteScene::onHttpRequest_GetInviter(){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/getInvitedUser");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s", Global::getInstance()->user_data.ID);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(InviteScene::onHttpResponse, this));
    request->setTag("GetInviter");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}


// HTTP响应请求函数
void InviteScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
                    if (tag == "GetInviter") {
                        const rapidjson::Value& val_content = document["content"];
                        if (val_content.IsObject()) {
                            char msg[200] = {0};
                            if (val_content.HasMember("nickname") && val_content.HasMember("account")) {
                                const char* nickname = val_content["nickname"].GetString();
                                const char* account = val_content["account"].GetString();
                                sprintf(msg, "账号：%s\n昵称：%s", account, nickname);
                            }
                            MessageBox(msg, "您的邀请人");
                        }
                        else {
                            auto scene = InviterScene::createScene();
                            Director::getInstance()->pushScene(scene);
                        }
                    }
                    else if (tag == "GetInviteList") {
                        hasGetInviteList = true;
                        inviteItems.clear();
                        
                        const rapidjson::Value& val_content = document["content"];
                        if (val_content.IsArray()) {
                            for (int i = 0; i < val_content.Size(); ++i) {
                                const rapidjson::Value& val_user = val_content[i];
                                assert(val_user.IsObject());
                                
                                if (val_user.HasMember("nickname") && val_user.HasMember("account")) {
                                    InviteItem* item = new InviteItem();
                                    item->autorelease();
                                    
                                    sprintf(item->account, "%s", val_user["account"].GetString());
                                    sprintf(item->nikename, "%s", val_user["nickname"].GetString());
                                    
                                    inviteItems.pushBack(item);
                                }
                                
                                
                            }
                        }
                        
                        recordListTableView->reloadData();
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
