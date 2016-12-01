//
//  PokerDeskScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/23.
//
//

#include "PokerDeskScene.h"
#include "PopAlertDialog.h"

PokerDesk::PokerDesk():m_deskState(0),m_IndexSend(0),m_IndexStart(0),m_isSendSingle(true){
    
}

PokerDesk::~PokerDesk(){
    
}

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
    btn_PrepareItem->setPosition(Vec2(origin.x + visibleSize.width / 2 + 0.8 * btn_PrepareItem->getContentSize().width, origin.y + 0.1 * visibleSize.height));
    
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
    showTimer->setPosition(origin.x + visibleSize.width / 2, origin.y + 0.74 * visibleSize.height);
    showTimer->setCallBackFunc(this,callfuncN_selector(PokerDesk::showTimerDoneCallback));
    this->addChild(showTimer, 2, 5);
    
    char imageName[4][100] = {"images/pk_table_tian@2x.png","images/pk_table_di@2x.png","images/pk_table_xuan@2x.png","images/pk_table_huang@2x.png"};
    float scaleArray[4][2] = {{0.5,0.65},{0.3,0.4},{0.5,0.4},{0.7,0.4}};
    for (int i = 0; i < 4; i++) {
        PokerChair* chair = this->createChair(imageName[i], scaleArray[i][0], scaleArray[i][1], i);
        m_arrChairs.pushBack(chair);
        this->addChild(chair);
    }
    
    bool isRet = false;
    do{
        srand((unsigned)time(NULL));//初始化随机种子
        CC_BREAK_IF(!createPokers());
        CC_BREAK_IF(!reindexPoker());
        
        isRet = true;
    } while (0);
    
    return isRet;
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
    this->showSettingChip();
}

void PokerDesk::showSettingChip(){
    PopAlertDialog* popup = PopAlertDialog::create("images/set_chip_bg.png",Size(312,190));
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
    
}

void PokerDesk::sendPokerAction(){
    
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
            
            scheduleUpdate();
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


void PokerDesk::update(float delta){
    switch (m_deskState) {
        case 0:{
            sendPoker();
        }
            break;
            
        case DeskState_Bet:{
            
        }
            break;
            
        default:
            break;
    }
}

#pragma chair
void PokerDesk::touchedChairCallback(Node* pSender){
    if (m_deskState == DeskState_Bet) {
        int arr[12] = {10,20,50,100,200,500,1000,2000,5000,10000,20000,50000};
        
        JettonSprite* sp = this->createjetton(arr[getRandomNumberNotEqualRight(0, 12)]);
        sp->setPosition(0.1 * getRandomNumber(0, 10) * (pSender->getContentSize().width - sp->getContentSize().width) + 0.5 * sp->getContentSize().width, 0.1 * getRandomNumber(0, 10) * (pSender->getContentSize().height - sp->getContentSize().height) + 0.5 * sp->getContentSize().height);
        pSender->addChild(sp, 0, 99);
    }
}

PokerChair* PokerDesk::createChair(const char* backgroudImage, float xScale, float yScale, int index){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    PokerChair* chair = PokerChair::PokerChair::create(backgroudImage, Size::ZERO);
    chair->setContentSize(Size(0.3 * visibleSize.height, (index == 0 ? 0.125 : 0.3) * visibleSize.height));
    chair->setPosition(origin.x + xScale * visibleSize.width - chair->getContentSize().width / 2, origin.y + yScale * visibleSize.height  - chair->getContentSize().height / 2);
    chair->setPoint(Vec2(chair->getPosition().x + chair->getContentSize().width / 2, chair->getPosition().y + 0.5 * chair->getContentSize().height));
    if (index == 0) {
        chair->setIsBanker(true);
    }
    else{
        chair->setTouchCallBackFunc(this, callfuncN_selector(PokerDesk::touchedChairCallback));
    }
    
    return chair;
}

#pragma jetton
JettonSprite* PokerDesk::createjetton(int value){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    JettonSprite* sp = JettonSprite::create(value, Size(0.08 * visibleSize.height, 0.08 * visibleSize.height));
    
    return sp;
}

void PokerDesk::addJetton(PokerChair* chair,JettonSprite* jetton){
    
}

#pragma poker
PokerSprite* PokerDesk::createPoker(PokerColor color,PokerPoint point){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    PokerSprite* pk = PokerSprite::create(color, point, Size(0.09375 * visibleSize.height, 0.125 * visibleSize.height));
    
    return pk;
}
bool PokerDesk::createPokers(){
    bool isRet = false;
    do{
        //创建52个牌
        for (int i = PokerColor_Spade; i <= PokerColor_Diamond; ++i){
            for (int j = PokerPoint_Ace; j <= PokerPoint_King; ++j){
                PokerSprite* pk = createPoker((PokerColor)i, (PokerPoint)j);
                m_arrPokers.pushBack(pk);
            }
        }
        //创建小鬼
        PokerSprite* joker_junior = createPoker(PokerColor_Joker, PokerPoint_JokerJunior);
        m_arrPokers.pushBack(joker_junior);
        
        //创建大鬼
        PokerSprite* joker_senior = createPoker(PokerColor_Joker, PokerPoint_JokerSenior);
        m_arrPokers.pushBack(joker_senior);
        
        isRet = true;
    } while (0);
    
    return isRet;
}

bool PokerDesk::reindexPoker(){
    bool isRet = false;
    do{
        for(int i = 0; i < m_arrPokers.size(); ++i){
            PokerSprite* pk1 = m_arrPokers.getRandomObject();
            PokerSprite* pk2 = m_arrPokers.getRandomObject();
            m_arrPokers.swap(pk1, pk2);
        }
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 position = Vec2(origin.x + 0.4 * visibleSize.width, origin.y + 0.8 * visibleSize.height);
        for (size_t i = m_arrPokers.size(); i > 0; --i) {
            PokerSprite* pk = m_arrPokers.at(i - 1);
            pk->setPosition(position.x, position.y - (i - 1) * 0.005 * pk->getContentSize().height);
            pk->setCallBackFunc(this, callfuncN_selector(PokerDesk::turnedSinglePokerCallback));
            this->addChild(pk);
        }
        
        isRet = true;
    } while (0);
    
    return isRet;
}

void PokerDesk::turnTopPoker(){
    
}

void PokerDesk::sendPoker(){
    if (m_IndexSend >= m_arrPokers.size()) {
        unscheduleUpdate();
        return;
    }
    
    int index = m_IndexSend % 9;
    if (index == 0 && m_isSendSingle) {
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        this->reorderChild(pk, 0);
        pk->showPokerAnimated(true, true);
        
        ++m_IndexSend;
        m_isSendSingle = false;
        
        if (pk->getPoker_point() == PokerPoint_JokerJunior || pk->getPoker_point() == PokerPoint_JokerSenior) {
            m_IndexStart = 0;
        }
        else{
            m_IndexStart = pk->getPoker_point() - 1;
        }
    }
    else if(index > 0 && index <= 8 && m_isSendSingle){
        PokerSprite *pk = m_arrPokers.at(m_IndexSend);
        this->reorderChild(pk, 0);
        PokerChair* chair = m_arrChairs.at(((index - 1) % m_arrChairs.size() + m_IndexStart) % m_arrChairs.size());
        movePoker(chair, pk);
        
        m_isSendSingle = false;
        ++m_IndexSend;
        if (m_IndexSend % 9 == 0) {
            m_deskState = DeskState_Bet;
            unscheduleUpdate();
        }
    }
}

void PokerDesk::movePoker(PokerChair* chair,PokerSprite* poker){
    float time = 0.5;
    chair->pokerArray.pushBack(poker);
    MoveTo* move = MoveTo::create(time, chair->getPoint());
    RotateBy* rotate = RotateBy::create(time, 360);
    CallFuncN* func = CallFuncN::create(CC_CALLBACK_1(PokerDesk::sendedSinglePoker, this, chair));
    Sequence* sequence = Sequence::create(Spawn::create(rotate,move, NULL),func,NULL);
    poker->runAction(sequence);
}

void PokerDesk::sendedSinglePoker(Node* pSender, void* pData){
    PokerChair* chair = (PokerChair* )pData;
    chair->updatePokerPosition();
    m_isSendSingle = true;
}

void PokerDesk::turnedSinglePokerCallback(Node* pSender){
    PokerSprite* poker = (PokerSprite* )pSender;
    
    if (m_arrPokers.getIndex(poker) % 9 == 0) {
        for (int i = 0; i < m_arrChairs.size(); i++) {
            PokerChair* chair = m_arrChairs.at(i);
            chair->setHighlighted(i == (m_IndexStart % m_arrChairs.size()));
        }
        poker->removeFromParent();
        
        m_isSendSingle = true;
    }
}
