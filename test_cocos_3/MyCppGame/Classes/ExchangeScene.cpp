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
    
    auto btn_BuyItem = MenuItemImage::create(
                                             "images/shop_btn_exchange.png",
                                             "images/shop_btn_select.png",
                                             CC_CALLBACK_1(ExchangeScene::buttonCallback, this, 1));
    
    btn_BuyItem->setScale((160.0 / 960.0) * visibleSize.width / btn_BuyItem->getContentSize().width);
    btn_BuyItem->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.2 * visibleSize.height + btn_BuyItem->getBoundingBox().size.height / 2));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, btn_BuyItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto label = Label::createWithTTF("兑换记录", "fonts/STKaiti.ttf", 14);
    label->setTextColor(Color4B::BLACK);
    label->setPosition(origin.x + (560.0 / 960.0) * visibleSize.width, origin.y + (610.0 / 640.0) * visibleSize.height);
    this->addChild(label);
    
    auto recordListSprite = Sprite::create();
    recordListSprite->setContentSize(Size((680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height));
    recordListSprite->setPosition(origin.x + 0.95 * visibleSize.width - recordListSprite->getContentSize().width / 2, origin.y + (40.0 / 640.0 ) * visibleSize.height + recordListSprite->getContentSize().height / 2);
    this->addChild(recordListSprite, 1);
    
    recordListCellWidth = recordListSprite->getContentSize().width;
    recordListTableView = TableView::create(this, Size(recordListCellWidth,  recordListSprite->getContentSize().height));
    recordListTableView->setPosition(0 , 0);
    recordListTableView->setDirection(TableView::Direction::VERTICAL);
    recordListTableView->setDelegate(this);
    recordListSprite->addChild(recordListTableView);
    
    recordListTableView->reloadData();
    
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


#pragma tableview
Size ExchangeScene::tableCellSizeForIndex(TableView* table, ssize_t idx)
{
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 30);
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
            sprintf(content, "2016-10-%d\t-1000金币\t兑换AAAA 订单号:201610%02d%06d",(int)idx,(int)idx,(int)idx);
        }
        else{
            sprintf(content, "2016-10-%d\t-2000金币\t兑换BBBB 订单号:201610%02d%06d",(int)idx,(int)idx,(int)idx);
        }
        
        label->setString(content);
        
        
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
    
    return 0;
}

void ExchangeScene::tableCellTouched(TableView* table, TableViewCell* cell){
    
}
