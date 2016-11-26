//
//  SendCardLayer.cpp
//  HelloWorldDemo
//
//  Created by xiaohua on 15/4/10.
//
//

#include "SendCardLayer.h"

float runTime = 0.15;

SendCardLayer::SendCardLayer()
{
    
}

SendCardLayer::~SendCardLayer()
{
    while (!m_cardBg.empty()) {
        m_cardBg.popBack();
    }
    
    while (!m_cardVec.empty()) {
        m_cardVec.popBack();
    }
}

bool SendCardLayer::init()
{
    if (! Layer::init()) {
        return false;
    }
    m_winSize = Director::getInstance()->getWinSize();
//    Director::getInstance()->setProjection(cocos2d::DisplayLinkDirector::Projection::_2D);
    for (int i = 1; i <= 16; i++)
    {
        auto bg_card = Sprite::create("card/card_bg.png");
        bg_card->setPosition(Vec2(m_winSize.width / 2,m_winSize.height * 0.8));
        this->addChild(bg_card);
        m_cardBg.pushBack(bg_card);
    }
    
    for (int i = 1; i <= 16; i++)
    {
        auto strN = __String::createWithFormat("card/%d.jpg",i);
        auto card = Sprite::create(strN->getCString());
        card->setPosition(Vec2(m_winSize.width * ( i / 20.0 + 0.08) ,m_winSize.height * 0.2));
        card->setVisible(false);
        this->addChild(card);
        m_cardVec.pushBack(card);
    }
    
    auto sendBtn = MenuItemFont::create("发牌", CC_CALLBACK_1(SendCardLayer::runSendAmt,this));
    sendBtn->setPosition(Vec2(m_winSize.width * 0.2,m_winSize.height * 0.85));
    
    auto closeBtn = MenuItemFont::create("关闭", [&](Ref * obj)
                                         {
                                             this->removeFromParentAndCleanup(true);
                                             Director::getInstance()->setProjection(cocos2d::DisplayLinkDirector::Projection::DEFAULT);
                                         });
    closeBtn->setPosition(Vec2(m_winSize.width * 0.2,m_winSize.height * 0.75));
    
    auto fastBtn = MenuItemFont::create("增加动画速度", [&](Ref * obj)
                                         {
                                             runTime += 0.05;
                                             log("-----------curTime--%f",runTime);
                                         });
    fastBtn->setPosition(Vec2(m_winSize.width * 0.8,m_winSize.height * 0.7));
    
    auto slowBtn = MenuItemFont::create("降低动画速度", [&](Ref * obj)
                                       {
                                           if (runTime <= 0.05) {
                                               runTime = 0.05;
                                               return ;
                                           }
                                           runTime -= 0.05;
                                           log("-----------curTime--%f",runTime);
                                       });
    slowBtn->setPosition(Vec2(m_winSize.width * 0.8,m_winSize.height * 0.6));
    
    auto pMenu = Menu::create(sendBtn,closeBtn,fastBtn,slowBtn,NULL);
    pMenu->setPosition(Vec2::ZERO);
    this->addChild(pMenu);
 
    return true;
}

void SendCardLayer::runSendAmt(Ref * obj)
{
    //发送中动画
    for (int i = 1; i <= 16; i++)
    {
        m_cardBg.at(i - 1)->runAction(Spawn::create(RotateBy::create(runTime + i / 16.0, 720),
            MoveTo::create(runTime + i / 16.0, Vec2(m_winSize.width * ( i / 20.0 + 0.08) ,m_winSize.height * 0.2)), NULL));
    }
    
    //翻牌动画一
    for (int j = 1; j <= 16; j++) {
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
    
//    //翻牌动画二
//    for (int j = 1; j <= 16; j++) {
//        m_cardBg.at(j - 1)->runAction(Sequence::create(DelayTime::create(1)
//                                                       ,ScaleTo::create(0.05 + j / 20.0, -1,1),
//                                                       Hide::create()
//                                                       ,CallFunc::create([=]
//                                                                         {
//                                                                             m_cardVec.at(j - 1)->runAction(Sequence::create(
//                                                                                                                             Show::create(),
//                                                                                                                             ScaleTo::create(0.05 + j / 20.0, -1,1), NULL));
//                                                                         }),NULL));
//        
//        m_cardVec.at(j - 1)->setFlippedX(true);
//    }
}

