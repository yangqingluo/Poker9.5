//
//  InviteScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "InviteScene.h"
#include "QLImageSprite.h"
#include "Global.h"

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
    
    auto inviteBG = QLImageSprite::create("images/share_bg_share.png", Size((720.0 / 609.0) * inviteSprite->getContentSize().height, inviteSprite->getContentSize().height));
    inviteBG->setPosition(Vec2(inviteSprite->getBoundingBox().size.width / 2, inviteSprite->getBoundingBox().size.height / 2));
    inviteSprite->addChild(inviteBG);
    
    auto btn_copy = Button::create("images/share_btn_s_copy.png", "images/share_btn_s_copy_hover.png");
//    btn_copy->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_copy->setScale(0.2 * inviteBG->getContentSize().width / btn_copy->getContentSize().width);
//    btn_copy->setContentSize(Size(0.2 * inviteBG->getContentSize().width, (68.0 / 149.0) * 0.2 * inviteBG->getContentSize().width));
    btn_copy->setPosition(Vec2((690.0 / 720.0) * inviteBG->getContentSize().width - 0.5 * btn_copy->getContentSize().width, (450.0 / 609.0) * inviteBG->getContentSize().height));
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
    
    auto btn_code = Button::create("images/bg_yellow_square.png");
    btn_code->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_code->setTitleText(Global::getInstance()->user_data.inviteCode);
    btn_code->setTitleFontSize(12);
    btn_code->setTitleColor(Color3B::BLACK);
    btn_code->setContentSize(btn_copy->getBoundingBox().size);
    btn_code->setPosition(Vec2(btn_copy->getBoundingBox().getMinX() - 0.6 * btn_code->getContentSize().width, btn_copy->getPositionY()));
    btn_code->setEnabled(false);
    inviteBG->addChild(btn_code);
    
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
