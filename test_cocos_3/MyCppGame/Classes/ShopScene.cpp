//
//  ShopScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "ShopScene.h"
#include "Global.h"

Scene* ShopScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ShopScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ShopScene::init()
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
                                              CC_CALLBACK_1(ShopScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    
    char showTitle[3][100] = {"充值","赠送","充值记录"};
    for (int i = 0; i < 3; i++) {
        auto btn_list = YNButton::create();
        btn_list->configImage("images/shop_btn_def.png", "", "images/shop_btn_select.png");
        btn_list->setScale9Enabled(true);
        btn_list->setContentSize(Size((160.0 / 960.0) * visibleSize.width, (65.0 / 173.0) * (160.0 / 960.0) * visibleSize.width));
        btn_list->setTitleColor(Color3B::WHITE);
        btn_list->setTitleText(showTitle[i]);
        btn_list->setTitleFontSize(12);
        btn_list->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.5 * visibleSize.height - (i * 1.2) * btn_list->getBoundingBox().size.height));
        btn_list->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
        btn_list->setTag(i);
        this->addChild(btn_list);
        
        listButtons.pushBack(btn_list);
        
        auto layer = LayerColor::create(Color4B(0, 0, 0, 0), (680.0 / 960.0) * visibleSize.width, visibleSize.height);
        layer->setPosition(origin.x + 0.95 * visibleSize.width - layer->getContentSize().width, origin.y);
        this->addChild(layer);
        listLayers.pushBack(layer);
        
        float inputHeight = MAX(32, layer->getContentSize().height / 8);
        switch (i) {
            case 0:{
                auto inputBox = ui::EditBox::create(Size(0.9 * layer->getContentSize().width, 0.8 * inputHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
                inputBox->setPosition(Vec2(0.5 * layer->getContentSize().width, layer->getContentSize().height - (i * 1.0 + 0.8) * inputHeight));
                layer->addChild(inputBox);
                
                //属性设置
                //    inputBox->setFontName("fonts/STKaiti.ttf");
                inputBox->setFontSize(12);
                inputBox->setFontColor(Color4B::BLACK);
                //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
                inputBox->setPlaceholderFontSize(12);
                inputBox->setPlaceholderFontColor(Color4B::GRAY);
                
                //模式类型设置
                inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
                inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
                inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
                
                inputBox->setPlaceHolder("请输入要充值的金币数目(10的整数倍)");
                
                buyCountBox = inputBox;
                
                auto btn_buy = Button::create("images/btn_green.png","images/btn_green_selected.png");
                btn_buy->setScale9Enabled(true);//打开scale9 可以拉伸图片
                btn_buy->setTitleText("确认充值");
                btn_buy->setTitleFontSize(16);
                btn_buy->setContentSize(inputBox->getContentSize());
                btn_buy->setPosition(Vec2(inputBox->getPositionX(), inputBox->getBoundingBox().getMinY() - btn_buy->getContentSize().height));
                btn_buy->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
                btn_buy->setTag(10);
                layer->addChild(btn_buy);
            }
                break;
                
            case 1:{
                char giveContent[2][50] = {"赠送的用户账号ID","赠送的钻石数目"};
                for (int j = 0; j < 2; j++) {
                    auto inputBox = ui::EditBox::create(Size(0.9 * layer->getContentSize().width, 0.8 * inputHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
                    inputBox->setPosition(Vec2(0.5 * layer->getContentSize().width, layer->getContentSize().height - (j * 1.0 + 0.8) * inputHeight));
                    layer->addChild(inputBox);
                    
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
                    
                    inputBox->setPlaceHolder(giveContent[j]);
                    switch (j) {
                        case 0:{
                            inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS);
                            inputBox->setMaxLength(14);
                            userIDBox = inputBox;
                            
                            inputBox->setContentSize(Size(0.45 * inputBox->getContentSize().width, inputBox->getContentSize().height));
                            inputBox->setPositionX(0.05 * layer->getContentSize().width + 0.5 * inputBox->getContentSize().width);
                            
                            auto btn_getUserInfo = Button::create("images/btn_green.png","images/btn_green_selected.png");
                            btn_getUserInfo->setScale9Enabled(true);//打开scale9 可以拉伸图片
                            btn_getUserInfo->setTitleText("查询");
                            btn_getUserInfo->setTitleFontSize(12);
                            btn_getUserInfo->setContentSize(inputBox->getContentSize());
                            btn_getUserInfo->setPosition(Vec2(0.95 * layer->getContentSize().width - 0.5 * btn_getUserInfo->getContentSize().width, inputBox->getPositionY()));
                            btn_getUserInfo->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
                            btn_getUserInfo->setTag(11);
                            layer->addChild(btn_getUserInfo);
                        }
                            break;
                            
                        case 1:{
                            inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::NUMERIC);
                            inputBox->setMaxLength(16);
                            giveCountBox = inputBox;
                            
                            auto btn_give = Button::create("images/btn_green.png","images/btn_green_selected.png");
                            btn_give->setScale9Enabled(true);//打开scale9 可以拉伸图片
                            btn_give->setTitleText("确认赠送");
                            btn_give->setTitleFontSize(16);
                            btn_give->setContentSize(inputBox->getContentSize());
                            btn_give->setPosition(Vec2(inputBox->getPositionX(), inputBox->getBoundingBox().getMinY() - btn_give->getContentSize().height));
                            btn_give->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
                            btn_give->setTag(12);
                            layer->addChild(btn_give);
                        }
                            break;
                            
                        default:
                            break;
                    }
                }
            }
                break;
                
            case 2:{
                auto label = Label::createWithTTF("充值记录", "fonts/STKaiti.ttf", 14);
                label->setTextColor(Color4B::BLACK);
                label->setPosition(0.5 * layer->getContentSize().width, (610.0 / 640.0) * layer->getContentSize().height);
                layer->addChild(label);
                
                recordListCellWidth = layer->getContentSize().width;
                recordListTableView = TableView::create(this, Size(recordListCellWidth,  (520.0 / 640.0) * visibleSize.height));
                recordListTableView->setPosition(0 , (40.0 / 640.0 ) * visibleSize.height);
                recordListTableView->setDirection(TableView::Direction::VERTICAL);
                recordListTableView->setDelegate(this);
                layer->addChild(recordListTableView);
                
                recordListTableView->reloadData();
            }
                break;
                
            default:
                break;
        }
    }
    
    showSettingWithIndex(0);
    
    return true;
}


void ShopScene::buttonCallback(cocos2d::Ref* pSender, int index){
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

void ShopScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 0:
                case 1:
                case 2:{
                    showSettingWithIndex(button->getTag());
                }
                    break;
                    
                case 10:{
                    //充值
                    if (strlen(buyCountBox->getText()) == 0) {
                        NoteTip::show("请输入充值的金币数目");
                    }
                    else {
                        NoteTip::show("精彩功能敬请期待");
                    }
                }
                    break;
                    
                case 11:{
                    //查询用户
                    if (strlen(userIDBox->getText()) == 0) {
                        NoteTip::show("请输入查询的用户账号ID");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_SearchUser(userIDBox->getText());
                    }
                }
                    break;
                    
                case 12:{
                    //赠送
                    if (strlen(giveCountBox->getText()) == 0) {
                        NoteTip::show("请输入赠送的钻石数目");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_DonateUser(userIDBox->getText(), giveCountBox->getText());
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

void ShopScene::showSettingWithIndex(int index){
    if (listButtons.size() != listLayers.size()) {
        return;
    }
    for (int i = 0; i < listButtons.size(); i++) {
        YNButton* button = listButtons.at(i);
        button->setSelected(button->getTag() == index);
        
        LayerColor* layer = listLayers.at(i);
        layer->setVisible(i == index);
    }
}

#pragma tableview
Size ShopScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 30);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* ShopScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        TableViewCell* cell = table->dequeueCell();
        
        if(!cell)
        {
            cell = new TableViewCell();
            cell->autorelease();
            
            Label* titleLabel = Label::createWithTTF("test", "fonts/STKaiti.ttf", 12);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(recordListCellWidth / 2, 15);
            titleLabel->setDimensions(recordListCellWidth, 30);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::TOP);
            cell->addChild(titleLabel, 0 , 1);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
        char content[100];
        if (idx%2 == 0) {
            sprintf(content, "2016-10-%d\t+25金币\tVIP奖励",(int)idx);
        }
        else{
            sprintf(content, "2016-10-%d\t+1000金币\t充值",(int)idx);
        }
        
        label->setString(content);
        
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t ShopScene::numberOfCellsInTableView(TableView* table)
{
    if (table == recordListTableView) {
        return 20;
    }
    
    return 0;
}

void ShopScene::tableCellTouched(TableView* table, TableViewCell* cell){
    
}

#pragma http
// 发送HTTP请求
void ShopScene::onHttpRequest_SearchUser(const char* account){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/userinfo");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "account=%s", account);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("searchuser");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

void ShopScene::onHttpRequest_DonateUser(const char* account, const char* count){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/gamebit/donategoldbit");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s&account=%s&count=%s", Global::getInstance()->user_data.ID,account, count);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("donate");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

// HTTP响应请求函数
void ShopScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
                if (0 != strlen(response->getHttpRequest()->getTag())){
                    std::string tag = response->getHttpRequest()->getTag();
                    if (tag == "searchuser") {
                        if (document.HasMember("content")) {
                            const rapidjson::Value& val_content = document["content"];
                            
                            UserData user_data = {0};
                            user_data.gameTimes = val_content["gameTimes"].GetInt();
                            
                            const char* nikename = val_content["nikename"].GetString();
                            memcpy(user_data.nikename, nikename, strlen(nikename));
                            
                            const char* account = val_content["account"].GetString();
                            memcpy(user_data.account, account, strlen(account));
                            
                            const char* winningPercent = val_content["winningPercent"].GetString();
                            memcpy(user_data.winningPercent, winningPercent, strlen(winningPercent));
                            
                            const char* inviteCode = val_content["inviteCode"].GetString();
                            memcpy(user_data.inviteCode, inviteCode, strlen(inviteCode));
                            
                            if (val_content.HasMember("diamondGameBit")) {
                                const rapidjson::Value& val_diamondGameBit = val_content["diamondGameBit"];
                                user_data.diamond = val_diamondGameBit["amount"].GetInt();
                            }
                            
                            if (val_content.HasMember("silverGameBit")) {
                                const rapidjson::Value& val_silverGameBit = val_content["silverGameBit"];
                                user_data.silver = val_silverGameBit["amount"].GetInt();
                            }
                            
                            if (val_content.HasMember("goldGameBit")) {
                                const rapidjson::Value& val_goldGameBit = val_content["goldGameBit"];
                                user_data.gold = val_goldGameBit["amount"].GetInt();
                            }
                            
                            char msg[200] = {0};
                            sprintf(msg, "ID：%s\n昵称：%s", user_data.account, user_data.nikename);
                            MessageBox(msg, "用户信息");
                        }
                        
                    }
                    else if (tag == "donate") {
                        MessageBox("赠送成功", "赠送");
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
