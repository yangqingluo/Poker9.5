//
//  SendCardTest.cpp
//  HelloWorldDemo
//
//  Created by xiaohua on 15/4/9.
//
//

#include "SendCardTest.h"
#include "SendCardLayer.h"

SendCardTest::SendCardTest()
{
    
}

SendCardTest::~SendCardTest()
{
}

cocos2d::Scene * SendCardTest::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SendCardTest::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool SendCardTest::init()
{
    if (! Layer::init()) {
        return false;
    }
    auto winSize = Director::getInstance()->getWinSize();
    //创建发牌层
    auto senfLayerBtn = MenuItemFont::create("创建发牌层", [&](Ref * obj)
                                         {
                                             auto sendLayer = SendCardLayer::create();
                                             this->addChild(sendLayer);
                                         });
    senfLayerBtn->setPosition(Vec2(winSize.width * 0.8,winSize.height * 0.8));
    
    auto pMenu = Menu::create(senfLayerBtn,NULL);
    pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu);
    
    return true;
}


