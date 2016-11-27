//
//  PokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "PokerDeskScene.h"
#include "PopAlertDialog.h"

Scene* PokerDesk::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = PokerDesk::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool PokerDesk::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("images/pokerDesk_bg_medium.jpg");
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
                                              CC_CALLBACK_1(PokerDesk::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 0);
    
    auto lotus = Sprite::create("images/lotus.png");
    lotus->setScale(150.0 / 640.0 * visibleSize.height / lotus->getTextureRect().getMaxY());
    lotus->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - lotus->getBoundingBox().size.height / 2);
    this->addChild(lotus);
    
    
    return true;
}


void PokerDesk::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
//            Director::getInstance()->popScene();
            PopAlertDialog* popup=PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
            popup->setTitle("");
            popup->setContentText("请设置带入的金币数目",12,50,150);
            popup->setCallBackFunc(this,callfuncN_selector(PokerDesk::popButtonCallback));
            popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "",0);
            popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "",1);
            
            this->addChild(popup);
            
            ControlSlider* myslider = ControlSlider::create("images/slider_bg.jpg","images/slider_jd.jpg","images/slider_hk.jpg");
            myslider->setPosition(popup->getContentSize().width / 2, popup->getContentSize().height / 2);
            myslider->setMaximumValue(100);
            myslider->setMinimumValue(0);
            popup->addChild(myslider,0,521); //这里3个参数表示：对象，层，标记
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

void PokerDesk::popButtonCallback(Node* pNode){
    log("[==========]button call back.tag %d",pNode->getTag());
    
    if (pNode->getTag()==0) {
        
    }
    
    if (pNode->getTag()==1) {
        //pNode->getParent()->removeFromParent();
        
    }
    
    pNode->removeFromParent();
}

void PokerDesk::onEnter()
{
    Layer::onEnter();
    
    log("%d",deskType);
}


