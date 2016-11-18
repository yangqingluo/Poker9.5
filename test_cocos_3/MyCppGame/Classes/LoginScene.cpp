//
//  LoginScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/18.
//
//

#include "LoginScene.h"

USING_NS_CC;

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
    
    return true;
}
