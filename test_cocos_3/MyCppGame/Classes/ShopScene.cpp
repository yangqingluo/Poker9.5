//
//  ShopScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "ShopScene.h"
#include "Global.h"
#include "CppToFunction.h"
#include "BaseCell.h"
#include "PopAlertDialog.h"
#include "GamePayment.h"

int getShopTag = 555;

ShopScene::ShopScene(){
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ShopScene::onNotification_Pay), kNotification_Pay, NULL);
}

ShopScene::~ShopScene(){
    NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* ShopScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ShopScene::create();
    layer->setTag(getShopTag);
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void ShopScene::onEnter(){
    Layer::onEnter();
    
}
void ShopScene::onExit(){
    Layer::onExit();
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
    
    int buy[7][3] = {{120,12,12},{250,25,25},{500,50,50},{980,98,98},{1980,198,198},{4880,488,488},{9980,998,998}};
    for (int i = 0; i < 7; i++) {
        BuyItem* item = new BuyItem();
        item->autorelease();
        
        item->goldCount = buy[i][0];
        item->price_normal = buy[i][1];
        item->price_apple = buy[i][2];
        sprintf(item->ID, "com.fulushou.ninehalf.pay%d", item->price_apple);
        
        buyList.pushBack(item);
    }
    
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
        
        recordListCellWidth = layer->getContentSize().width;
        float inputHeight = MAX(32, layer->getContentSize().height / 8);
        switch (i) {
            case 0:{
                auto inputBox = ui::EditBox::create(Size(0.9 * layer->getContentSize().width, 0.8 * inputHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
                inputBox->setPosition(Vec2(0.5 * layer->getContentSize().width, layer->getContentSize().height - (i * 1.0 + 0.8) * inputHeight));
//                layer->addChild(inputBox);
                
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
                
                inputBox->setDelegate(this);
                inputBox->setPlaceHolder("请输入要充值的金币数目");
                
                buyCountBox = inputBox;
                auto btn_buy = Button::create("images/btn_green.png","images/btn_green_selected.png");
                btn_buy->setScale9Enabled(true);//打开scale9 可以拉伸图片
                btn_buy->setTitleText("确认购买");
                btn_buy->setTitleFontSize(16);
                btn_buy->setContentSize(inputBox->getContentSize());
                btn_buy->setPosition(Vec2(inputBox->getPositionX(), inputBox->getBoundingBox().getMinY() - btn_buy->getContentSize().height));
                btn_buy->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
                btn_buy->setTag(10);
                layer->addChild(btn_buy);
                
                float payHeight = (400.0 / 640.0) * visibleSize.height;
                payListTableView = TableView::create(this, Size(recordListCellWidth,  payHeight));
                payListTableView->setPosition(0 , btn_buy->getBoundingBox().getMinY() - 5 - payHeight);
                payListTableView->setDirection(TableView::Direction::VERTICAL);
                payListTableView->setDelegate(this);
                layer->addChild(payListTableView);
                
                payListTableView->reloadData();
            }
                break;
                
            case 1:{
                char giveContent[2][50] = {"赠送的用户账号ID","赠送的数目"};
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
                            btn_give->setTitleText("赠送金币");
                            btn_give->setTitleFontSize(16);
                            btn_give->setContentSize(inputBox->getContentSize());
                            btn_give->setPosition(Vec2(inputBox->getPositionX(), inputBox->getBoundingBox().getMinY() - btn_give->getContentSize().height));
                            btn_give->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
                            btn_give->setTag(12);
                            layer->addChild(btn_give);
                            
                            auto btn_give_diamond = Button::create("images/btn_green.png","images/btn_green_selected.png");
                            btn_give_diamond->setScale9Enabled(true);//打开scale9 可以拉伸图片
                            btn_give_diamond->setTitleText("赠送钻石");
                            btn_give_diamond->setTitleFontSize(16);
                            btn_give_diamond->setContentSize(inputBox->getContentSize());
                            btn_give_diamond->setPosition(Vec2(inputBox->getPositionX(), btn_give->getBoundingBox().getMinY() - btn_give->getContentSize().height));
                            btn_give_diamond->addTouchEventListener(CC_CALLBACK_2(ShopScene::touchEvent, this));
                            btn_give_diamond->setTag(13);
                            layer->addChild(btn_give_diamond);
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
                    if (button->getTag() == 2) {
                        if (!hasGetRecordList) {
                            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                            this->onHttpRequest_RechargeRecords();
                        }
                    }
                }
                    break;
                    
                case 10:{
                    //充值
//                    int payCount = atoi(buyCountBox->getText());
//                    if (strlen(buyCountBox->getText()) == 0 || payCount <= 0) {
//                        NoteTip::show("请正确输入充值的金币数目");
//                    }
//                    else {
//                        Global::getInstance()->goldToRecharge = payCount;
//                        if (payIndex == 0) {
//                            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
//                            this->onHttpRequest_GetOrderAndSign(payCount / 10.0);
//                        }
//                        else {
//                            NoteTip::show("精彩功能敬请期待");
//                        }
//                    }
                    
                    showBuyInfo();
                }
                    break;
                    
                case 11:{
                    //查询用户
                    if (strlen(userIDBox->getText()) == 0) {
                        NoteTip::show("请输入查询的用户账号ID");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
                        onHttpRequest_SearchUser(userIDBox->getText());
                    }
                }
                    break;
                    
                case 12:{
                    //赠送
                    if (strlen(giveCountBox->getText()) == 0) {
                        NoteTip::show("请正确输入赠送的数目");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_DonateUserGold(userIDBox->getText(), giveCountBox->getText());
                    }
                }
                    break;
                    
                case 13:{
                    //赠送
                    if (strlen(giveCountBox->getText()) == 0) {
                        NoteTip::show("请输入赠送的数目");
                    }
                    else {
                        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                        onHttpRequest_DonateUserDiamond(userIDBox->getText(), giveCountBox->getText());
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

void ShopScene::showBuyInfo(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PopAlertDialog* popup = PopAlertDialog::create("images/store2_buy_background.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
    popup->setTitle("请选择支付方式\n(银行卡支付请选择支付宝)", 18, Color3B::Color3B(220, 20, 60));
    popup->m_touchCancel = true;
    popup->m_buttonListed = true;
    popup->setCallBackFunc(this,callfuncN_selector(ShopScene::popButtonCallback));
    
    BuyItem* item = buyList.at(payIndex);
    char m_string_pay_normal[20] = {0};
    char m_string_pay_apple[20] = {0};
    
    sprintf(m_string_pay_normal, "¥ %d", item->price_normal);
    sprintf(m_string_pay_apple, "¥ %d", item->price_apple);
    
    
    popup->addButton("images/store2_buy_channel_select_background.png", "images/store2_buy_channel_select_background.png", "支付宝",m_string_pay_normal,0);
    popup->addButton("images/store2_buy_channel_select_background.png", "images/store2_buy_channel_select_background.png", "微信",m_string_pay_apple,1);
    popup->addButton("images/store2_buy_channel_select_background.png", "images/store2_buy_channel_select_background.png", "App Store",m_string_pay_apple,2);
    
    this->addChild(popup, INT_MAX);
}

void ShopScene::showMessageManager(bool isShow){
    if (isShow) {
        m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
    }
    else {
        if (m_pMessage != NULL) {
            m_pMessage->hidden();
            m_pMessage = NULL;
        }
    }
}

void getProductCallback(IOSProduct *product, int code) // 获取商品的回调
{
    ShopScene* spLayer =(ShopScene* ) Director::getInstance()->getRunningScene()->getChildByTag(
                                                                                                  getShopTag);
    spLayer->showMessageManager(false);
    if (product != NULL) {
        log( "My: product->productIdentifier => %s", product->productIdentifier.c_str() );
        log( "My: product->localizedPrice => %s", product->localizedPrice.c_str() );
    }
    else {
        NoteTip::show("购买失败");
    }
}

void ShopScene::popButtonCallback(Node* pNode){
    switch (pNode->getTag()) {
        case 0:{
            BuyItem* item = buyList.at(payIndex);
            
            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
            this->onHttpRequest_GetOrderAndSign(item->price_normal);
        }
            break;
            
        case 1:{
            NoteTip::show("精彩功能，敬请期待");
//            BuyItem* item = buyList.at(payIndex);
//            
//            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
//            this->onHttpRequest_GetOrderAndSign(item->price_normal);
        }
            break;
            
        case 2:{
            BuyItem* item = buyList.at(payIndex);
            string str_s = item->ID;
            
            this->showMessageManager(true);
            GamePayment *gamePayment = GamePayment::getInstance();
            gamePayment->req_iap(str_s, getProductCallback);
        }
            break;
            
        default:
            break;
    }
}



#pragma edixBox
//开始编辑
void ShopScene::editBoxEditingDidBegin(ui::EditBox* editBox){
}

//结束编辑
void ShopScene::editBoxEditingDidEnd(ui::EditBox* editBox){
}

//编辑框内容改变
void ShopScene::editBoxTextChanged(ui::EditBox* editBox, const std::string& text){
    if (editBox == buyCountBox) {
        payListTableView->reloadData();
    }
}

//触发return返回
void ShopScene::editBoxReturn(ui::EditBox* editBox){
    
}

#pragma tableview
Size ShopScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 30);
    }
    else if (table == payListTableView) {
        return Size(recordListCellWidth, 50);
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
            
            Label* titleLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 12);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(recordListCellWidth / 2, 15);
            titleLabel->setDimensions(recordListCellWidth, 30);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::TOP);
            cell->addChild(titleLabel, 0 , 1);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
        
        RechargeItem* item = rechargeItems.at(rechargeItems.size() - 1 - idx);
        char content[100] = {0};
        sprintf(content, "%s\t+%d金币\t%s", item->date, item->amount, (item->rechargeWay == 2) ? "VIP奖励" : "充值");
        
        label->setString(content);
        
        
        return cell;
    }
    else if (table == payListTableView) {
        BaseCell* cell = (BaseCell* )table->dequeueCell();
        
        float height = 50;
        if(!cell)
        {
            cell = new BaseCell();
            cell->autorelease();
            
            auto head = Sprite::create();
            cell->addChild(head);
            cell->head = head;
            
            auto titleLabel = Label::create();
            titleLabel->setSystemFontSize(14.0);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.6 * recordListCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.7 * recordListCellWidth, height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->addChild(titleLabel);
            cell->titleLabel = titleLabel;
            
            auto line = Sprite::create("images/tableview_line.png");
            line->setScaleX(recordListCellWidth / line->getContentSize().width);
            line->setPosition(0.5 * recordListCellWidth, 0.5 * line->getContentSize().height);
            cell->addChild(line);
            
            auto selectImage = Sprite::create("images/pay_selected.png");
            selectImage->setScale(0.5 * height / selectImage->getContentSize().height);
            selectImage->setPosition(0.95 * recordListCellWidth - 0.5 * selectImage->getBoundingBox().size.width, 0.5 * height);
            selectImage->setVisible(false);
            cell->addChild(selectImage);
            cell->selectImage = selectImage;
        }

        BuyItem* item = buyList.at(buyList.size() - 1 - idx);
        
        
        char m_string[200] = {0};
        sprintf(m_string, "购买%d金币\t\t¥%d", item->goldCount, item->price_normal);
//        
//        char m_image[30] = {0};
//        sprintf(m_image, "images/gold_buy_%d.png", item->goldCount);
        cell->head->setTexture("jetton/jetton_bg.png");
        
        cell->titleLabel->setString(m_string);
        
        cell->head->setScale((0.9 * height) / cell->head->getContentSize().height);
        cell->head->setPosition(0.8 * cell->head->getBoundingBox().size.width, 0.5 * height);
        cell->selectImage->setVisible(payIndex == (buyList.size() - 1 - idx));
        
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t ShopScene::numberOfCellsInTableView(TableView* table)
{
    if (table == recordListTableView) {
        return rechargeItems.size();
    }
    else if (table == payListTableView) {
        return buyList.size();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        //iOS代码
//        return 3;
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//        //Android代码
//        return 2;
//#endif
    }
    
    return 0;
}

void ShopScene::tableCellTouched(TableView* table, TableViewCell* cell){
    if (table == recordListTableView) {
        
    }
    else if (table == payListTableView) {
        BaseCell* old_cell = (BaseCell *)table->cellAtIndex(buyList.size() - 1 - payIndex);
        if (old_cell != NULL) {
            old_cell->selectImage->setVisible(false);
        }
        
        payIndex = (int)buyList.size() - 1 - (int)cell->getIdx();
        
        BaseCell* new_cell = (BaseCell *)cell;
        new_cell->selectImage->setVisible(true);
    }
}

#pragma http
// 发送HTTP请求
void ShopScene::onHttpRequest_RechargeRecords(){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/gamebit/rechargeRecords");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "account=%s", Global::getInstance()->user_data.account);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("rechargeRecords");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}
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

void ShopScene::onHttpRequest_DonateUserGold(const char* account, const char* count){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/gamebit/donateGoldBit");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s&account=%s&count=%s", Global::getInstance()->user_data.ID,account, count);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("donateGold");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

void ShopScene::onHttpRequest_DonateUserDiamond(const char* account, const char* count){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/gamebit/donateDiamondBit");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s&account=%s&count=%s", Global::getInstance()->user_data.ID,account, count);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("donateDiamond");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

void ShopScene::onHttpRequest_GetOrderAndSign(float totalFee){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/alipay/getOrderAndSign");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "totalFee=%.2f&partner=2088521530118846&subject=recharge&userId=%s&account=%s", totalFee, Global::getInstance()->user_data.ID, Global::getInstance()->user_data.account);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("getOrderAndSign");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}


void ShopScene::onHttpRequest_RechargeForApple(float totalGold){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/applepay/rechargebit");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "money=%f&userId=%s&userAccount=%s", totalGold, Global::getInstance()->user_data.ID, Global::getInstance()->user_data.account);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ShopScene::onHttpResponse, this));
    request->setTag("rechargeForApple");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

// HTTP响应请求函数
void ShopScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
    this->showMessageManager(false);
    
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
                            Global::getInstance()->parseUserData(val_content, &user_data);
                            
                            char msg[200] = {0};
                            sprintf(msg, "ID：%s\n昵称：%s", user_data.account, user_data.nikename);
                            MessageBox(msg, "用户信息");
                        }
                        
                    }
                    else if (tag == "donateGold") {
                        MessageBox("赠送成功", "赠送金币");
                        rapidjson::Value& val_content = document["content"];
                        if (val_content.IsObject()) {
                            int amount = val_content["amount"].GetInt();
                            Global::getInstance()->user_data.gold = amount;
                            
                            MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_RefreshUserInfo, NULL);
                        }
                    }
                    else if (tag == "donateDiamond") {
                        MessageBox("赠送成功", "赠送钻石");
                        rapidjson::Value& val_content = document["content"];
                        if (val_content.IsObject()) {
                            int amount = val_content["amount"].GetInt();
                            Global::getInstance()->user_data.diamond = amount;
                            
                            MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_RefreshUserInfo, NULL);
                        }
                    }
                    else if (tag == "rechargeRecords") {
                        hasGetRecordList = true;
                        rechargeItems.clear();
                        
                        const rapidjson::Value& val_content = document["content"];
                        if (val_content.IsArray()) {
                            for (int i = 0; i < val_content.Size(); ++i) {
                                const rapidjson::Value& val_record = val_content[i];
                                assert(val_record.IsObject());
                                
                                if (val_record.HasMember("rechargeWay") && val_record.HasMember("amount") && val_record.HasMember("date")) {
                                    RechargeItem* item = new RechargeItem();
                                    item->autorelease();
                                    
                                    item->rechargeWay = atoi(val_record["rechargeWay"].GetString());
                                    item->amount = val_record["amount"].GetInt();
                                    sprintf(item->date, "%s", val_record["date"].GetString());
                                    
                                    rechargeItems.pushBack(item);
                                }
                                
                                
                            }
                        }
                        
                        if (rechargeItems.size() == 0) {
                            NoteTip::show(this, "没有充值记录");
                        }
                        
                        recordListTableView->reloadData();
                    }
                    else if (tag == "getOrderAndSign") {
                        const rapidjson::Value& val_content = document["content"];
                        char order_string[1024] = {0};
                        strcpy(order_string, val_content.GetString());
                        CppToFunction::getInstance()->doAlipayAction(order_string);
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
void ShopScene::onNotification_Pay(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    switch (post->cmd) {
        case PayStyle_alipay:{
            if (post->sub_cmd == 9000) {
                NoteTip::show("充值成功");
            }
            else {
                NoteTip::show("充值失败");
            }
        }
            break;
            
        default:
            break;
    }
}
