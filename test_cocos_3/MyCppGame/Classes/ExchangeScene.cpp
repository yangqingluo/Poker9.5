//
//  ExchangeScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "ExchangeScene.h"

USING_NS_CC;

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
        float inputHeight = MAX(32, layer->getContentSize().height / 8);
        switch (i) {
            case 0:{
                exchangeItems.clear();
                char description[8][30] = {"iPhone7 32GB", "iPhone7 128GB", "iPhone7 256GB", "iPhone7 Plus 32GB", "iPhone7 Plus 128GB", "iPhone7 Plus 256GB", "iPadPro 9.7英寸", "iPadPro 12.9英寸"};
                int price[8] = {5388, 6188, 6988, 6388, 7188, 7988, 4388, 5388};
                for (int i = 7; i >= 0; i--) {
                    ExchangeItem* item = new ExchangeItem();
                    item->autorelease();
                    
                    switch (i) {
                        case 0:
                        case 1:
                        case 2:{
                            sprintf(item->imagePath, "images/iphone7.png");
                        }
                            break;
                            
                        case 3:
                        case 4:
                        case 5:{
                            sprintf(item->imagePath, "images/iphone7_plus.png");
                        }
                            break;
                            
                        case 6:{
                            sprintf(item->imagePath, "images/ipad_pro_9in.png");
                        }
                            break;
                            
                        case 7:{
                            sprintf(item->imagePath, "images/ipad_pro_12in.png");
                        }
                            break;
                            
                        default:
                            break;
                    }
                    
                    sprintf(item->description, "%s", description[i]);
                    item->price_gold = price[i];
                    
                    exchangeItems.pushBack(item);
                }
                
                
//                auto label = Label::createWithTTF("兑换列表", "fonts/STKaiti.ttf", 14);
//                label->setTextColor(Color4B::BLACK);
//                label->setPosition(0.5 * layer->getContentSize().width, (610.0 / 640.0) * layer->getContentSize().height);
//                layer->addChild(label);
                
                exchangeListTableView = TableView::create(this, Size(recordListCellWidth,  (520.0 / 640.0) * visibleSize.height));
                exchangeListTableView->setPosition(0 , (40.0 / 640.0 ) * visibleSize.height);
                exchangeListTableView->setDirection(TableView::Direction::VERTICAL);
                exchangeListTableView->setDelegate(this);
                layer->addChild(exchangeListTableView);
                
                exchangeListTableView->reloadData();
            }
                break;
                
            case 1:{
                auto label = Label::createWithTTF("兑换记录", "fonts/STKaiti.ttf", 14);
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
//                    if (button->getTag() == 1) {
//                        if (!hasGetRecordList) {
//                            m_pMessage = MessageManager::show(this, MESSAGETYPE_LOADING, NULL);//显示
//                            this->onHttpRequest_RechargeRecords();
//                        }
//                    }
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

#pragma tableview
Size ExchangeScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 40);
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
            
            Label* titleLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 10);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(recordListCellWidth / 2, 15);
            titleLabel->setDimensions(recordListCellWidth, 40);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::TOP);
            cell->addChild(titleLabel, 0 , 1);
        }
        
        Label* label = (Label* )cell->getChildByTag(1);
        char content[200] = {0};
        if (idx % 2 == 0) {
            sprintf(content, "2016-10-%d\t-1000金币\t兑换AAAA 订单号:201610%02d%06d 快递单号:201610%02d%06d 快递公司:顺丰快递",(int)idx,(int)idx,(int)idx,(int)idx,(int)idx);
        }
        else{
            sprintf(content, "2016-10-%d\t-2000金币\t兑换BBBB 订单号:201610%02d%06d 未发货",(int)idx,(int)idx,(int)idx);
        }
        label->setString(content);
        
        
        return cell;
    }
    else if (table == exchangeListTableView) {
        TableViewCell* cell = table->dequeueCell();
        
        float height = 50;
        if(!cell){
            cell = new TableViewCell();
            cell->autorelease();
            
            auto head = Sprite::create("images/iphone7_plus.png");
            head->setScale(height / head->getContentSize().height);
            head->setPosition(0.5 * head->getBoundingBox().size.width, 0.5 * height);
            cell->addChild(head, 0 , 2);
            
            Label* titleLabel = Label::create("", "", 8);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.6 * recordListCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.7 * recordListCellWidth, height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            titleLabel->setTag(1);
            cell->addChild(titleLabel);
        }
        
        ExchangeItem* item = exchangeItems.at(idx);
        
        Label* label = (Label* )cell->getChildByTag(1);
        char content[200];
        sprintf(content, "%s\n价格：%d金币", item->description, item->price_gold);
        label->setString(content);
        
        Sprite* head = (Sprite* )cell->getChildByTag(2);
        head->setTexture(item->imagePath);
        head->setScale((0.9 * height) / head->getContentSize().height);
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t ExchangeScene::numberOfCellsInTableView(TableView* table)
{
    if (table == recordListTableView) {
        return 20;
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
