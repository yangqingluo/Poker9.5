//
//  ExchangeScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "ExchangeScene.h"
#include "ExchangeCell.h"
#include "PopAlertDialog.h"

#define dialogTag         9527
#define addressBoxTag     9528
#define nameBoxTag        9529
#define mobileBoxTag      9530
#define noteBoxTag      9531


Scene* ExchangeScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ExchangeScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool ExchangeScene::init()
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
                                              CC_CALLBACK_1(ExchangeScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    char showTitle[2][20] = {"兑换","兑换记录"};
    for (int i = 0; i < 2; i++) {
        auto btn_list = YNButton::create();
        btn_list->configImage("images/shop_btn_def.png", "", "images/shop_btn_select.png");
        btn_list->setScale9Enabled(true);
        btn_list->setContentSize(Size((160.0 / 960.0) * visibleSize.width, (65.0 / 173.0) * (160.0 / 960.0) * visibleSize.width));
        btn_list->setTitleColor(Color3B::WHITE);
        btn_list->setTitleText(showTitle[i]);
        btn_list->setTitleFontSize(12);
        btn_list->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.5 * visibleSize.height - (i * 1.2) * btn_list->getBoundingBox().size.height));
        btn_list->addTouchEventListener(CC_CALLBACK_2(ExchangeScene::touchEvent, this));
        btn_list->setTag(i);
        this->addChild(btn_list);
        
        listButtons.pushBack(btn_list);
        
        auto layer = LayerColor::create(Color4B(0, 0, 0, 0), (680.0 / 960.0) * visibleSize.width, visibleSize.height);
        layer->setPosition(origin.x + 0.95 * visibleSize.width - layer->getContentSize().width, origin.y);
        this->addChild(layer);
        listLayers.pushBack(layer);
        
        recordListCellWidth = layer->getContentSize().width;
        switch (i) {
            case 0:{
                exchangeItems.clear();
                char description[12][50] = {"iPhone7 32GB", "iPhone7 128GB", "iPhone7 256GB", "iPhone7 Plus 32GB", "iPhone7 Plus 128GB", "iPhone7 Plus 256GB", "iPadPro 9.7英寸 WLAN 32GB", "iPadPro 9.7英寸 WLAN 128GB", "iPadPro 9.7英寸 WLAN 256GB", "iPadPro 12.9英寸 WLAN 32GB", "iPadPro 12.9英寸 WLAN 128GB", "iPadPro 12.9英寸 WLAN 256GB"};
                int price[12] = {5388, 6188, 6988, 6388, 7188, 7988, 4388, 5188, 5888, 5888, 6688, 7488};
                for (int i = 11; i >= 0; i--) {
                    ExchangeItem* item = new ExchangeItem();
                    item->autorelease();
                    
                    switch (i) {
                        case 0:
                        case 1:
                        case 2:{
                            sprintf(item->imagePath, "images/iphone7.png");
                            
                            item->colors.push_back(Value(ItemColorType_Black));
                            item->colors.push_back(Value(ItemColorType_Silver));
                            item->colors.push_back(Value(ItemColorType_Golden));
                            item->colors.push_back(Value(ItemColorType_RoseGolden));
                            if (i != 0) {
                                item->colors.push_back(Value(ItemColorType_BlackLighted));
                            }
                        }
                            break;
                            
                        case 3:
                        case 4:
                        case 5:{
                            sprintf(item->imagePath, "images/iphone7_plus.png");
                            
                            item->colors.push_back(Value(ItemColorType_Black));
                            item->colors.push_back(Value(ItemColorType_Silver));
                            item->colors.push_back(Value(ItemColorType_Golden));
                            item->colors.push_back(Value(ItemColorType_RoseGolden));
                            if (i != 3) {
                                item->colors.push_back(Value(ItemColorType_BlackLighted));
                            }
                        }
                            break;
                            
                        case 6:
                        case 7:
                        case 8:{
                            sprintf(item->imagePath, "images/ipad_pro_9in.png");
                            item->colors.push_back(Value(ItemColorType_Silver));
                            item->colors.push_back(Value(ItemColorType_Golden));
                            item->colors.push_back(Value(ItemColorType_DeepSkyGray));
                            item->colors.push_back(Value(ItemColorType_RoseGolden));
                        }
                            break;
                            
                        case 9:
                        case 10:
                        case 11:{
                            sprintf(item->imagePath, "images/ipad_pro_12in.png");
                            item->colors.push_back(Value(ItemColorType_Silver));
                            item->colors.push_back(Value(ItemColorType_Golden));
                            item->colors.push_back(Value(ItemColorType_DeepSkyGray));
                        }
                            break;
                            
                        default:
                            break;
                    }
                    
                    sprintf(item->description, "%s", description[i]);
                    item->price = price[i];
                    
                    exchangeItems.pushBack(item);
                }
                
                auto label = Label::create();
                label->setSystemFontSize(14.0);
                label->setString("选择物品及颜色");
                label->setTextColor(Color4B::BLACK);
                label->setPosition(0.5 * layer->getContentSize().width, (610.0 / 640.0) * layer->getContentSize().height);
                layer->addChild(label);
                
                exchangeListTableView = TableView::create(this, Size(recordListCellWidth,  (520.0 / 640.0) * visibleSize.height));
                exchangeListTableView->setPosition(0 , (40.0 / 640.0 ) * visibleSize.height);
                exchangeListTableView->setDirection(TableView::Direction::VERTICAL);
                exchangeListTableView->setDelegate(this);
                layer->addChild(exchangeListTableView);
                
                exchangeListTableView->reloadData();
            }
                break;
                
            case 1:{
                auto label = Label::create();
                label->setSystemFontSize(14.0);
                label->setString("兑换记录");
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
    
//    
//    
//    auto btn_BuyItem = MenuItemImage::create(
//                                             "images/shop_btn_exchange.png",
//                                             "images/shop_btn_select.png",
//                                             CC_CALLBACK_1(ExchangeScene::buttonCallback, this, 1));
//    
//    btn_BuyItem->setScale((160.0 / 960.0) * visibleSize.width / btn_BuyItem->getContentSize().width);
//    btn_BuyItem->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.2 * visibleSize.height + btn_BuyItem->getBoundingBox().size.height / 2));
//    
//    auto label = Label::createWithTTF("兑换记录", "fonts/STKaiti.ttf", 14);
//    label->setTextColor(Color4B::BLACK);
//    label->setPosition(origin.x + (560.0 / 960.0) * visibleSize.width, origin.y + (610.0 / 640.0) * visibleSize.height);
//    this->addChild(label);
//    
//    auto recordListSprite = Sprite::create();
//    recordListSprite->setContentSize(Size((680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height));
//    recordListSprite->setPosition(origin.x + 0.95 * visibleSize.width - recordListSprite->getContentSize().width / 2, origin.y + (40.0 / 640.0 ) * visibleSize.height + recordListSprite->getContentSize().height / 2);
//    this->addChild(recordListSprite, 1);
//    
//    recordListCellWidth = recordListSprite->getContentSize().width;
//    recordListTableView = TableView::create(this, Size(recordListCellWidth,  recordListSprite->getContentSize().height));
//    recordListTableView->setPosition(0 , 0);
//    recordListTableView->setDirection(TableView::Direction::VERTICAL);
//    recordListTableView->setDelegate(this);
//    recordListSprite->addChild(recordListTableView);
//    
//    recordListTableView->reloadData();
    
    
    
    return true;
}


void ExchangeScene::buttonCallback(cocos2d::Ref* pSender, int index){
    if (index >= 100) {
        //兑换物品颜色选择
        int itemIndex = index / 100 - 1;
        if (itemIndex >= 0 && itemIndex <= exchangeItems.size()) {
            int colorIndex = index % 100;
            this->showExchangeInput(itemIndex, colorIndex);
        }
        else {
            MessageBox("未知兑换物品", "出错了");
        }
        return;
    }
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

void ExchangeScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 0:
                case 1:{
                    showSettingWithIndex(button->getTag());
                    if (button->getTag() == 1) {
                        if (!hasGetRecordList) {
                            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
                            this->onHttpRequest_ExchangeRecords();
                        }
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

void ExchangeScene::showSettingWithIndex(int index){
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

void ExchangeScene::showExchangeInput(size_t itemIndex, size_t colorIndex){
    m_itemIndex = itemIndex;
    m_colorIndex = colorIndex;
    
    ExchangeItem* item = exchangeItems.at(itemIndex);
    if (colorIndex < item->colors.size()) {
        int colorType = item->colors.at(colorIndex).asInt();
        char colorString[20] = {0};
        Global::getInstance()->getStringWithItemColor(colorString, (ItemColorType)colorType);
        
        char showTitle[200] = {0};
        sprintf(showTitle, "兑换%s%s\n将花费%d金币", colorString, item->description, item->price * 10);
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        PopAlertDialog* popup = PopAlertDialog::create("images/bg_dialog_empty.png", Size(0.7 * visibleSize.width, 0.7 * visibleSize.height));
        popup->setTitle(showTitle, 14);
        
        popup->setTag(dialogTag);
        popup->setCallBackFunc(this,callfuncN_selector(ExchangeScene::popButtonCallback));
        
        popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "","",1);
        popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "","",0);
        
        this->addChild(popup, 2);
        
        char boxPlaceHolder[3][50] = {"收货人姓名","收货地址","收货人联系方式"};
        float boxHeight = MIN(0.15 * popup->m_dialogContentSize.height, 32);
        for (int j = 0; j < 3; j++) {
            auto inputBox = ui::EditBox::create(Size(0.4 * popup->m_dialogContentSize.width, boxHeight), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
            inputBox->setPosition(Vec2(popup->getContentSize().width / 2, 0.6 * popup->getContentSize().height - j * boxHeight));
            popup->addChild(inputBox);
            
            //属性设置
            //    inputBox->setFontName("fonts/STKaiti.ttf");
            inputBox->setFontSize(12);
            inputBox->setFontColor(Color4B::BLACK);
            //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
            inputBox->setPlaceholderFontSize(12);
            inputBox->setPlaceholderFontColor(Color4B::GRAY);
            
            inputBox->setPlaceHolder(boxPlaceHolder[j]);
            switch (j) {
                case 0:{
                    inputBox->setTag(nameBoxTag);
                    inputBox->setMaxLength(Max_Name_Length);
                }
                    break;
                    
                case 1:{
                    inputBox->setTag(addressBoxTag);
                    inputBox->setMaxLength(Max_Address_Length);
                }
                    break;
                    
                case 2:{
                    inputBox->setTag(mobileBoxTag);
                    inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::PHONE_NUMBER);
                    inputBox->setMaxLength(length_mobile);
                }
                    break;
                    
                default:
                    break;
            }
        }
        
    }
    else {
        MessageBox("未知颜色", "出错了");
    }
}

void ExchangeScene::popButtonCallback(Node* pNode){
    if (pNode->getTag() == 0) {
        PopAlertDialog* popup = (PopAlertDialog *)this->getChildByTag(dialogTag);
        if (popup == NULL) {
            return;
        }
        cocos2d::ui::EditBox* nameBox = (cocos2d::ui::EditBox* )popup->getChildByTag(nameBoxTag);
        if (nameBox == NULL) {
            return;
        }
        else {
            if (strlen(nameBox->getText()) == 0) {
                NoteTip::show("请输入正确的联系人姓名");
                return;
            }
        }
        
        cocos2d::ui::EditBox* addressBox = (cocos2d::ui::EditBox* )popup->getChildByTag(addressBoxTag);
        if (addressBox == NULL) {
            return;
        }
        else {
            if (strlen(addressBox->getText()) == 0) {
                NoteTip::show("请输入正确的联系人地址");
                return;
            }
        }
        
        cocos2d::ui::EditBox* mobileBox = (cocos2d::ui::EditBox* )popup->getChildByTag(mobileBoxTag);
        if (mobileBox == NULL) {
            return;
        }
        else {
            if (strlen(mobileBox->getText()) != length_mobile) {
                NoteTip::show("请输入正确的联系人电话");
                return;
            }
        }
        
        ExchangeItem* item = exchangeItems.at(m_itemIndex);
        if (m_colorIndex < item->colors.size()) {
            int colorType = item->colors.at(m_colorIndex).asInt();
            char colorString[20] = {0};
            Global::getInstance()->getStringWithItemColor(colorString, (ItemColorType)colorType);
            
            char remarks[200] = {0};
            sprintf(remarks, "%s%s", colorString, item->description);
            
            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);
            this->onHttpRequest_addAwardRecord(nameBox->getText(), addressBox->getText(), mobileBox->getText(), remarks, item->price * 10);
        }
    }
}

#pragma tableview
Size ExchangeScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 50);
    }
    else if (table == exchangeListTableView) {
        return Size(recordListCellWidth, 50);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* ExchangeScene::tableCellAtIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        TableViewCell* cell = table->dequeueCell();
        
        if(!cell)
        {
            cell = new TableViewCell();
            cell->autorelease();
            
            auto titleLabel = Label::create();
            titleLabel->setSystemFontSize(10.0);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(recordListCellWidth / 2, 15);
            titleLabel->setDimensions(recordListCellWidth, 50);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::TOP);
            cell->addChild(titleLabel, 0 , 1);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
        ExchangeRecordItem* item = recordItems.at(recordItems.size() - 1 - idx);
        char content[400] = {0};
        char status_string[100] = {0};
        switch (item->status) {
            case 0:{
                sprintf(status_string, "拒绝");
            }
                break;
                
            case 1:{
                sprintf(status_string, "待审核");
            }
                break;
                
            case 2:{
                sprintf(status_string, "审核通过");
            }
                break;
                
            case 3:{
                if (strlen(item->expressCode) > 0 && strlen(item->expressType) > 0) {
                    sprintf(status_string, "已发货\t快递单号:%s\t快递公司:%s", item->expressCode, item->expressType);
                }
                else {
                    sprintf(status_string, "已发货");
                }
            }
                break;
                
            case 4:{
                sprintf(status_string, "已完成");
            }
                break;
                
            default:
                break;
        }
        
        sprintf(content, "%s\t-%d金币\t兑换%s 订单号:%s 收货人:%s 地址:%s 电话:%s %s", item->date, item->gameBitAmount, item->remarks, item->ID, item->receiveName, item->address, item->tel,status_string);
        
        label->setString(content);
        
        return cell;
    }
    else if (table == exchangeListTableView) {
        ExchangeCell* cell = (ExchangeCell* )table->dequeueCell();
        
        float height = 50;
        if(!cell){
            cell = new ExchangeCell();
            cell->autorelease();
            
            auto head = Sprite::create();
            cell->addChild(head);
            cell->head = head;
            
            auto titleLabel = Label::create();
            titleLabel->setSystemFontSize(8.0);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.3 * recordListCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.3 * recordListCellWidth, height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->addChild(titleLabel);
            cell->titleLabel = titleLabel;
            
            auto line = Sprite::create("images/tableview_line.png");
            line->setScaleX(recordListCellWidth / line->getContentSize().width);
            line->setPosition(0.5 * recordListCellWidth, 0.5 * line->getContentSize().height);
            cell->addChild(line);
            
            auto layerColor = LayerColor::create(Color4B::WHITE, 0.55 * recordListCellWidth, 0.8 * height);
            layerColor->setPosition(0.45 * recordListCellWidth, 0.5 * height - 0.5 * layerColor->getContentSize().height);
            cell->addChild(layerColor);
            cell->buttonLayer = layerColor;
            
            cell->colorMenu = Menu::create();
            cell->colorMenu->setPosition(Vec2::ZERO);
            cell->buttonLayer->addChild(cell->colorMenu);
            cell->itemWidth = MIN(30, layerColor->getContentSize().width / 5.0);
        }
        
        ExchangeItem* item = exchangeItems.at(idx);
        
        char content[200];
        sprintf(content, "%s\n价格：%d金币", item->description, item->price * 10);
        cell->titleLabel->setString(content);
        
        cell->head->setTexture(item->imagePath);
        cell->head->setScale((0.9 * height) / cell->head->getContentSize().height);
        cell->head->setPosition(0.5 * cell->head->getBoundingBox().size.width, 0.5 * height);
        
        cell->colorMenu->removeAllChildren();
        for (int i = 0; i < item->colors.size(); i++) {
            int colorType = item->colors.at(i).asInt();
            
            char colorString[20] = {0};
            Global::getInstance()->getStringWithItemColor(colorString, (ItemColorType)colorType);
            
            auto label = Label::create();
            label->setSystemFontSize(8.0);
            label->setTextColor(Color4B::BLACK);
//            label->setPosition((i + 0.5) * cell->itemWidth, 0.5 * cell->buttonLayer->getContentSize().height);
            label->setDimensions(cell->itemWidth, cell->buttonLayer->getContentSize().height);
            label->setHorizontalAlignment(TextHAlignment::CENTER);
            label->setVerticalAlignment(TextVAlignment::CENTER);
            label->setString(colorString);
            
            auto menuItem = MenuItemLabel::create(label, CC_CALLBACK_1(ExchangeScene::buttonCallback, this, (idx + 1) * 100 + i));
//            MenuItemFont::create(colorString, CC_CALLBACK_1(ExchangeScene::buttonCallback, this, (idx + 1) * 100 + i));
//            menuItem->setFontSizeObj(8);
//            menuItem->setColor(Color3B::BLACK);
            menuItem->setPosition((i + 0.5) * cell->itemWidth, 0.5 * cell->buttonLayer->getContentSize().height);
            cell->colorMenu->addChild(menuItem);
        }
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t ExchangeScene::numberOfCellsInTableView(TableView* table)
{
    if (table == recordListTableView) {
        return recordItems.size();
    }
    else if (table == exchangeListTableView) {
        return exchangeItems.size();
    }
    
    return 0;
}

void ExchangeScene::tableCellTouched(TableView* table, TableViewCell* cell){
    if (table == recordListTableView) {
        
    }
    else if (table == exchangeListTableView) {
        
    }
}

#pragma http
// 发送HTTP请求
void ExchangeScene::onHttpRequest_ExchangeRecords(){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/awardRecords");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "userId=%s", Global::getInstance()->user_data.ID);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ExchangeScene::onHttpResponse, this));
    request->setTag("exchangeRecords");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

void ExchangeScene::onHttpRequest_addAwardRecord(const char* name, const char* address, const char* tel, const char* remarks, int gold){
    // 创建HTTP请求
    HttpRequest* request = new HttpRequest();
    
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl("http://115.28.109.174:8181/game/user/addAwardRecord");
    
    // 设置post发送请求的数据信息
    char param[200] = {0};
    sprintf(param, "user=%s&receiveName=%s&address=%s&tel=%s&remarks=%s&gameBitAmount=%d", Global::getInstance()->user_data.ID, name, address, tel, remarks, gold);
    request->setRequestData(param, strlen(param));
    
    // HTTP响应函数
    request->setResponseCallback(CC_CALLBACK_2(ExchangeScene::onHttpResponse, this));
    request->setTag("addAwardRecord");
    // 发送请求
    HttpClient::getInstance()->send(request);
    
    // 释放链接
    request->release();
}

// HTTP响应请求函数
void ExchangeScene::onHttpResponse(HttpClient* sender, HttpResponse* response){
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
                    if (tag == "addAwardRecord") {
                        rapidjson::Value& val_content = document["content"];
                        if (val_content.IsObject()) {
                            if (val_content.HasMember("code")) {
                                int code = val_content["code"].GetInt();
                                if (code == 1000) {
                                    //成功
                                    ExchangeItem* item = exchangeItems.at(m_itemIndex);
                                    Global::getInstance()->user_data.gold -= item->price * 10;
                                    MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_RefreshUserInfo, NULL);
                                }
                            }
                            
                            if (val_content.HasMember("description")) {
                                NoteTip::show(val_content["description"].GetString());
                            }
                        }
                    }
                    else if (tag == "exchangeRecords") {
                        hasGetRecordList = true;
                        recordItems.clear();
                        
                        const rapidjson::Value& val_content = document["content"];
                        if (val_content.IsArray()) {
                            for (int i = 0; i < val_content.Size(); ++i) {
                                const rapidjson::Value& val_record = val_content[i];
                                assert(val_record.IsObject());
                                
                                if (val_record.HasMember("id") && val_record.HasMember("receiveName") && val_record.HasMember("status") && val_record.HasMember("gameBitAmount") && val_record.HasMember("address") && val_record.HasMember("tel") && val_record.HasMember("remarks") && val_record.HasMember("date")) {
                                    ExchangeRecordItem* item = new ExchangeRecordItem();
                                    item->autorelease();
                                    
                                    item->status = atoi(val_record["status"].GetString());
                                    item->gameBitAmount = val_record["gameBitAmount"].GetInt();
                                    sprintf(item->ID, "%s", val_record["id"].GetString());
                                    sprintf(item->receiveName, "%s", val_record["receiveName"].GetString());
                                    sprintf(item->address, "%s", val_record["address"].GetString());
                                    sprintf(item->tel, "%s", val_record["tel"].GetString());
                                    sprintf(item->remarks, "%s", val_record["remarks"].GetString());
                                    sprintf(item->date, "%s", val_record["date"].GetString());
                                    
                                    if (val_record.HasMember("expressCode") && val_record.HasMember("expressType")) {
                                        if (!val_record["expressCode"].IsNull() && !val_record["expressType"].IsNull()) {
                                            sprintf(item->expressCode, "%s", val_record["expressCode"].GetString());
                                            sprintf(item->expressType, "%s", val_record["expressType"].GetString());
                                        }
                                    }
                                    
                                    recordItems.pushBack(item);
                                }
                            }
                        }
                        
                        if (recordItems.size() == 0) {
                            NoteTip::show(this, "没有兑换记录");
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
