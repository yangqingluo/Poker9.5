//
//  PokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "PokerDeskScene.h"
#include "PopAlertDialog.h"
#include "SendCardLayer.h"

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
    
    btn_PrepareItem = MenuItemImage::create(
                                              "images/btn_prepare.png",
                                              "images/btn_prepare.png",
                                              CC_CALLBACK_1(PokerDesk::buttonCallback, this, 1));
    
    btn_PrepareItem->setScale(this->getScaleX(), this->getScaleY());
    btn_PrepareItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 0.8 * btn_PrepareItem->getContentSize().width, origin.y + 168.0 / 640.0 * visibleSize.height));
    
    btn_AnotherdeskItem = MenuItemImage::create(
                                                 "images/btn_anotherdesk.png",
                                                 "images/btn_anotherdesk.png",
                                                 CC_CALLBACK_1(PokerDesk::buttonCallback, this, 1));
    
    btn_AnotherdeskItem->setScale(this->getScaleX(), this->getScaleY());
    btn_AnotherdeskItem->setPosition(Vec2(origin.x + visibleSize.width / 2 - 0.8 * btn_AnotherdeskItem->getContentSize().width, btn_PrepareItem->getPositionY()));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, btn_PrepareItem, btn_AnotherdeskItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 0);
    
    auto lotus = Sprite::create("images/lotus.png");
    lotus->setScale(150.0 / 640.0 * visibleSize.height / lotus->getTextureRect().getMaxY());
    lotus->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - lotus->getBoundingBox().size.height / 2);
    this->addChild(lotus);
    
    upright_sprite = Sprite::create();
    upright_sprite->setContentSize(Size(0.15 * visibleSize.width, 0.15 * visibleSize.width));
    upright_sprite->setPosition(origin.x + visibleSize.width - upright_sprite->getContentSize().width * 0.6, origin.y + visibleSize.height - upright_sprite->getContentSize().height * 0.6);
    this->addChild(upright_sprite);
    
    auto uprightBG = Sprite::create("images/window_upright_bg.png");
    uprightBG->setScale(upright_sprite->getContentSize().width / uprightBG->getContentSize().width);
    uprightBG->setPosition(0.5 * upright_sprite->getContentSize().width, 0.5 * upright_sprite->getContentSize().width);
    upright_sprite->addChild(uprightBG);
    
    countLabel = Label::createWithTTF("桌子人数：1\n当前状态：未准备", "fonts/STKaiti.ttf", 8);
    countLabel->setColor(Color3B::BLACK);
//    countLabel->setHorizontalAlignment(TextHAlignment::LEFT);
//    countLabel->setVerticalAlignment(TextVAlignment::CENTER);
    countLabel->setPosition(0.5 * upright_sprite->getContentSize().width, 0.2 * upright_sprite->getContentSize().height);
    upright_sprite->addChild(countLabel);
    
    
    message_sprite = Sprite::create();
    message_sprite->setContentSize(Size(928.0 / 104.0 * 0.05 * visibleSize.height, 0.05 * visibleSize.height));
    message_sprite->setPosition(origin.x + message_sprite->getContentSize().width * 0.52, origin.y + message_sprite->getContentSize().height * 0.55);
    this->addChild(message_sprite);
    
    auto messageBG = Sprite::create("images/message_bg.png");
    messageBG->setScale(message_sprite->getContentSize().height / messageBG->getContentSize().height);
    messageBG->setPosition(0.5 * message_sprite->getContentSize().width, 0.5 * message_sprite->getContentSize().height);
    message_sprite->addChild(messageBG);
    
    messageLabel = Label::createWithTTF("正在等待玩家加入...", "fonts/STKaiti.ttf", 8);
    messageLabel->setColor(Color3B::BLACK);
    messageLabel->setHorizontalAlignment(TextHAlignment::LEFT);
    messageLabel->setVerticalAlignment(TextVAlignment::CENTER);
    messageLabel->setDimensions(message_sprite->getContentSize().width, message_sprite->getContentSize().height);
    messageLabel->setPosition(0.5 * message_sprite->getContentSize().width, 0.5 * message_sprite->getContentSize().height);
    message_sprite->addChild(messageLabel);
    
    
    showTimer = GameTimer::createTimer(0);
    showTimer->showTag = 0;
    showTimer->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    showTimer->setCallBackFunc(this,callfuncN_selector(PokerDesk::showTimerDoneCallback));
    this->addChild(showTimer, 2, 5);
    
    chair_tian = PokerChair::PokerChair::create("images/pk_table_tian@2x.png",Size(100,100));
    chair_tian->setContentSize(Size(0.1 * visibleSize.width, 0.1 * visibleSize.width));
    chair_tian->setPosition(origin.x + visibleSize.width / 2 - chair_tian->getContentSize().width / 2, origin.y + 0.7 * visibleSize.height  - chair_tian->getContentSize().height / 2);
    this->addChild(chair_tian);
    
    chair_di = PokerChair::PokerChair::create("images/pk_table_di@2x.png",Size(100,100));
    chair_di->setContentSize(Size(0.1 * visibleSize.width, 0.1 * visibleSize.width));
    chair_di->setPosition(origin.x + 0.3 * visibleSize.width - chair_di->getContentSize().width / 2, origin.y + 0.55 * visibleSize.height  - chair_di->getContentSize().height / 2);
    this->addChild(chair_di);
    
    chair_xuan = PokerChair::PokerChair::create("images/pk_table_xuan@2x.png",Size(100,100));
    chair_xuan->setContentSize(Size(0.1 * visibleSize.width, 0.1 * visibleSize.width));
    chair_xuan->setPosition(origin.x + 0.5 * visibleSize.width - chair_xuan->getContentSize().width / 2, origin.y + 0.4 * visibleSize.height  - chair_xuan->getContentSize().height / 2);
    this->addChild(chair_xuan);
    
    chair_huang = PokerChair::PokerChair::create("images/pk_table_huang@2x.png",Size(100,100));
    chair_huang->setContentSize(Size(0.1 * visibleSize.width, 0.1 * visibleSize.width));
    chair_huang->setPosition(origin.x + 0.7 * visibleSize.width - chair_huang->getContentSize().width / 2, origin.y + 0.55 * visibleSize.height  - chair_huang->getContentSize().height / 2);
    this->addChild(chair_huang);
    
    chair_pokerStack = PokerChair::PokerChair::create();
    chair_pokerStack->setContentSize(Size(0.1 * visibleSize.width, 0.1 * visibleSize.width));
    chair_pokerStack->setPosition(origin.x + 0.5 * visibleSize.width - chair_pokerStack->getContentSize().width / 2, origin.y + 0.6 * visibleSize.height  - chair_pokerStack->getContentSize().height / 2);
    chair_pokerStack->setColor(Color3B::WHITE);
    chair_pokerStack->setOpacity(0);
    chair_pokerStack->setVisible(false);
    this->addChild(chair_pokerStack);
    
    return true;
}


void PokerDesk::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            this->goBackAction();
        }
            break;
            
        case 1:{
            this->preparedAction();
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
    if (pNode->getTag() == 0) {
        this->waitForPrepareprepareAction();
    }
    else if(pNode->getTag() == 1) {
        this->goBackAction();
    }
    pNode->removeFromParent();
}

void PokerDesk::onEnter(){
    Layer::onEnter();
    
    log("%d",deskType);
    this->showSettingChip();
}

void PokerDesk::showSettingChip(){
    PopAlertDialog* popup=PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
    popup->setTitle("");
    popup->setContentText("请设置带入的金币数目",12,50,150);
    popup->setCallBackFunc(this,callfuncN_selector(PokerDesk::popButtonCallback));
    popup->addButton("images/btn_sure.png", "images/btn_sure_highlighted.png", "",0);
    popup->addButton("images/btn_cancel.png", "images/btn_cancel_highlighted.png", "",1);
    
    this->addChild(popup);
    
    ControlSlider* myslider = ControlSlider::create("images/slider_bg.png","images/slider_jd.png","images/slider_hk.png");
    myslider->setPosition(popup->getContentSize().width / 2, popup->getContentSize().height * 0.45);
    myslider->setMaximumValue(100);
    myslider->setMinimumValue(0);
    popup->addChild(myslider);
}

void PokerDesk::goBackAction(){
    Director::getInstance()->popScene();
}

void PokerDesk::waitForPrepareprepareAction(){
    btn_PrepareItem->setVisible(true);
    btn_AnotherdeskItem->setVisible(true);
    
    sprintf(showTimer->prefixString,"等待准备…");
    showTimer->showTag = 1;
    showTimer->start(3);
}

void PokerDesk::preparedAction(){
    char* string = new char[100];
    sprintf(string,"桌子人数：1\n当前状态：已准备");
    countLabel->setString(string);
    
    btn_PrepareItem->setVisible(false);
    btn_AnotherdeskItem->setVisible(false);
    
    sprintf(showTimer->prefixString,"等待开始…");
    showTimer->showTag = 2;
}

void PokerDesk::showJudgeAction(){
    for (int j = 0; j <= 0; j++) {
        m_cardBg.at(j)->runAction(Sequence::create(DelayTime::create(1)
                                                       ,OrbitCamera::create(0.05 + j / 20.0, 1, 0, 0, 90, 0, 0)
                                                       ,Hide::create()
                                                       ,CallFunc::create([=]
                                                                         {
                                                                             m_cardVec.at(j)->runAction(Sequence::create(
                                                                                                                             Show::create(),
                                                                                                                             OrbitCamera::create(0.05 + j / 20.0, 1, 0, 90, -90, 0, 0), NULL));
                                                                         }),NULL));
        
        m_cardVec.at(j)->runAction(OrbitCamera::create(0.02, 1, 0, 0, 90, 0, 0));
        pokerIndex++;
    }
}

void PokerDesk::sendPokerAction(){
    float runTime = 0.15;
    for (int i = pokerIndex + 1; i <= pokerIndex + 8; i++){
        Vec2 position = Vec2::ZERO;
        switch ((i - pokerIndex - 1) % 4) {
            case 0:{
                position = Vec2(chair_tian->getPosition().x + chair_tian->getContentSize().width / 2, chair_tian->getPosition().y + chair_tian->getContentSize().height / 2);
            }
                break;
                
            case 1:{
                position = Vec2(chair_di->getPosition().x + chair_di->getContentSize().width / 2, chair_di->getPosition().y + chair_di->getContentSize().height / 2);
            }
                break;
                
            case 2:{
                position = Vec2(chair_xuan->getPosition().x + chair_xuan->getContentSize().width / 2, chair_xuan->getPosition().y + chair_xuan->getContentSize().height / 2);
            }
                break;
                
            case 3:{
                position = Vec2(chair_huang->getPosition().x + chair_huang->getContentSize().width / 2, chair_huang->getPosition().y + chair_huang->getContentSize().height / 2);
            }
                break;
                
            default:
                break;
        }
        m_cardBg.at(i - 1)->runAction(Spawn::create(RotateBy::create(runTime + (i - pokerIndex - 1) * 1.0, 720),
                                                    MoveTo::create(runTime + (i - pokerIndex - 1) * 1.0, Vec2(position.x ,position.y)), NULL));
        m_cardVec.at(i - 1)->setPosition(position);
    }
    
    //翻牌动画一
    for (int j = pokerIndex + 1; j <= 8; j++) {
        m_cardBg.at(j - 1)->runAction(Sequence::create(DelayTime::create(1)
                                                       ,OrbitCamera::create(0.05 + j / 20.0, 1, 0, 0, 90, 0, 0)
                                                       ,Hide::create()
                                                       ,CallFunc::create([=]
                                                                         {
                                                                             m_cardVec.at(j - 1)->runAction(Sequence::create(
                                                                                                                             Show::create(),
                                                                                                                             OrbitCamera::create(0.05 + j / 20.0, 1, 0, 90, -90, 0, 0), NULL));
                                                                         }),NULL));
        
        m_cardVec.at(j - 1)->runAction(OrbitCamera::create(0.02, 1, 0, 0, 90, 0, 0));
    }
    
    pokerIndex+=8;
}

void PokerDesk::showTimerDoneCallback(Node* pNode){
    switch (showTimer->showTag) {
        case 0:{
//            sprintf(showTimer->prefixString,"");
//            showTimer->showPrefix();
        }
            break;
        case 1:{
            this->goBackAction();
        }
            break;
            
        case 2:{
            message_sprite->setVisible(false);
            
            sprintf(showTimer->prefixString,"开始!");
            showTimer->showTag = 0;
            showTimer->showPrefix();
            
            chair_pokerStack->setVisible(true);
            
            auto visibleSize = Director::getInstance()->getVisibleSize();
            Vec2 origin = Director::getInstance()->getVisibleOrigin();
            for (int i = 1; i <= 54; i++)
            {
                auto bg_card = Sprite::create("card/card_bg.png");
                bg_card->setPosition(Vec2(origin.x + 0.5 * visibleSize.width, origin.y + 0.58 * visibleSize.height));
                this->addChild(bg_card);
                m_cardBg.pushBack(bg_card);
            }
            
            for (int i = 1; i <= 54; i++)
            {
                auto strN = __String::createWithFormat("card/%d.jpg",i);
                auto card = Sprite::create(strN->getCString());
                card->setPosition(Vec2(origin.x + 0.5 * visibleSize.width,origin.y + 0.58 * visibleSize.height));
                card->setVisible(false);
                this->addChild(card);
                m_cardVec.pushBack(card);
            }
            
            sprintf(showTimer->prefixString,"发牌顺序");
            showTimer->showTag = 3;
            showTimer->start(1);
        }
            break;
            
        case 3:{
            this->showJudgeAction();
            
            sprintf(showTimer->prefixString,"准备发牌");
            showTimer->showTag = 4;
            showTimer->start(2);
            
        }
            break;
            
        case 4:{
            this->sendPokerAction();
            
            sprintf(showTimer->prefixString,"发牌");
            showTimer->showTag = 5;
            showTimer->start(2);
            
        }
            break;
            
        case 5:{
            sprintf(showTimer->prefixString,"下注");
            showTimer->showTag = 6;
            showTimer->start(30);
            
        }
            break;
            
        default:
            break;
    }
    
}

