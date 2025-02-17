//
//  PopAlertDialog.cpp
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#include "PopAlertDialog.h"

PopAlertDialog::PopAlertDialog():
m__pMenu(NULL),m_contentPadding(0),m_contentPaddingTop(0),m_callbackListener(NULL),m_callback(NULL),m__sfBackGround(NULL),m__s9BackGround(NULL),m__ltContentText(NULL),m__ltTitle(NULL),m_touchCancel(false), m_buttonListed(false){
    
    
    
}

PopAlertDialog::~PopAlertDialog(){
    CC_SAFE_RELEASE(m__pMenu);
    CC_SAFE_RELEASE(m__sfBackGround);
    CC_SAFE_RELEASE(m__s9BackGround);
    CC_SAFE_RELEASE(m__ltContentText);
    CC_SAFE_RELEASE(m__ltTitle);
}

bool PopAlertDialog::init(){
    if (!LayerColor::init()) {
        return false;
    }
    
    Menu* menu = Menu::create();
    menu->setPosition(Point::ZERO);
    setMenuButton(menu);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PopAlertDialog::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(PopAlertDialog::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PopAlertDialog::onTouchEnded, this);
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //设置弹出层的颜色，指定为淡灰色
    setColor(Color3B::GRAY);
    setOpacity(128);
    
    return true;
}

bool PopAlertDialog::onTouchBegan(Touch* touch,Event* event){
    return true;
}

void PopAlertDialog::onTouchMoved(Touch* touch,Event* event){
    
}

void PopAlertDialog::onTouchEnded(Touch* touch,Event* event){
    auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标
    
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Rect rect = getSpriteBackGround()->getBoundingBox();
    if (!rect.containsPoint(locationInNode) && m_touchCancel){
         this->removeFromParentAndCleanup(true);
    }
}

PopAlertDialog* PopAlertDialog::create(const char* backgoundImage,Size dialogSize){
    //创建弹出对话框，指定背景图和大小。
    PopAlertDialog* layer = PopAlertDialog::create();
    
    layer->setSpriteBackGround(Sprite::create(backgoundImage));
//    layer->setSprite9BackGround(Scale9Sprite::create(backgoundImage));
    layer->m_dialogContentSize = dialogSize;
    
    return layer;
}

void PopAlertDialog::setTitle(const char* title,int fontsize /*=20*/, Color3B color /*= Color3B::WHITE*/){
    Label* label = Label::create();
    label->setSystemFontSize(fontsize);
    label->setString(title);
    label->setColor(color);
    setLabelTitle(label);
}

void PopAlertDialog::setContentText(const char* text,int fontsize,int padding,int paddingTop){
    Label* ltf = Label::createWithTTF(text, "fonts/STKaiti.ttf", fontsize);
    ltf->setColor(Color3B::WHITE);
    setLabelContentText(ltf);
    m_contentPadding = padding;
    m_contentPaddingTop = paddingTop;
}

void PopAlertDialog::setCallBackFunc(Ref*target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;
}

bool PopAlertDialog::addButton(const char *normalImage, const char *selectedImage,const char* title,const char* subTitle,int tag){
    Size winSize = Director::getInstance()->getWinSize();
    Point center_point = Point(winSize.width / 2,winSize.height / 2);
    
    auto menuImage = MenuItemImage::create(normalImage,
                                         selectedImage,
                                         CC_CALLBACK_1(PopAlertDialog::buttonCallBack,this));
    menuImage->setTag(tag);
    menuImage->setPosition(center_point);
    menuImage->setScale((m_buttonListed ? 0.45 : 0.2) * m_dialogContentSize.width / menuImage->getContentSize().width);
    
    if (strlen(title)) {
        Size menuSize = menuImage->getContentSize();
        Label* titleLabel = Label::create();
        titleLabel->setString(title);
        titleLabel->setSystemFontSize(15.0);
        titleLabel->setColor(Color3B(Color3B::WHITE));
        titleLabel->setPosition(Point(menuSize.width/2,menuSize.height/2));
        menuImage->addChild(titleLabel);
        if (strlen(subTitle)) {
            auto subLabel = Label::create();
            subLabel->setString(subTitle);
            subLabel->setSystemFontSize(15.0);
            subLabel->setTextColor(Color4B(220, 20, 60, 255));
            subLabel->setPosition(Point(menuSize.width/2,menuSize.height/2));
            subLabel->setDimensions(0.8 * menuSize.width, menuSize.height);
            subLabel->setHorizontalAlignment(TextHAlignment::RIGHT);
            subLabel->setVerticalAlignment(TextVAlignment::CENTER);
            menuImage->addChild(subLabel);
            
            titleLabel->setDimensions(0.8 * menuSize.width, menuSize.height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
        }
    }
    
    getMenuButton()->addChild(menuImage);
    
    return true;
}


void PopAlertDialog::buttonCallBack(Ref* pSender){
    Node* node = dynamic_cast<Node*>(pSender);
    if (m_callback && m_callbackListener) {
        (m_callbackListener->*m_callback)(node);
    }
    this->removeFromParentAndCleanup(true);
}


void  PopAlertDialog::onEnter(){
    LayerColor::onEnter();
    
//    Size winSize=Director::getInstance()->getWinSize();
//    Point center=Point(winSize.width/2,winSize.height/2);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite* background = getSpriteBackGround();
    
//    Scale9Sprite* background=getSprite9BackGround();
    
//    background->setContentSize(m_dialogContentSize);//指定对话框大小
    float bg_scale_x = m_dialogContentSize.width / background->getTextureRect().getMaxX();
    float bg_scale_y = m_dialogContentSize.height / background->getTextureRect().getMaxY();
    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    background->setScale(bg_scale_x, bg_scale_y);
    this->addChild(background,0,0);
    
    Action* popupActions = Sequence::create(ScaleTo::create(0.0, 0.0),
                                            ScaleTo::create(0.06, 1.05 * bg_scale_x, 1.05 * bg_scale_y),
                                            ScaleTo::create(0.08, 0.95 * bg_scale_x, 0.95 * bg_scale_y),
                                            ScaleTo::create(0.08, 1.00 * bg_scale_x, 1.00 * bg_scale_y),
                                            CallFunc::create(CC_CALLBACK_0(PopAlertDialog::backgroundFinish, this))
                                            , NULL);
    
    background->runAction(popupActions);
}

void PopAlertDialog::backgroundFinish(){
    auto winSize = Director::getInstance()->getWinSize();
    Point pCenter = Point(winSize.width / 2, winSize.height / 2);
    
    this->addChild(getMenuButton());
    
    float btnWidth = m_dialogContentSize.width / (getMenuButton()->getChildrenCount() + 1);
    
    Vector<Node*> vector = getMenuButton()->getChildren();
    
    int i = 0;
    int num_line = 2;
    for (Node* pObj : vector){
        Node* node = dynamic_cast<Node*>(pObj);
        if (m_buttonListed) {
            node->setPosition((i % num_line == 0) ? pCenter.x - m_dialogContentSize.width / 4 : pCenter.x + m_dialogContentSize.width / 4, pCenter.y - (i / num_line) * 1.1 * node->getContentSize().height);
        }
        else {
            node->setPosition(Point(pCenter.x - m_dialogContentSize.width / 2 + btnWidth * (i + 1), pCenter.y - m_dialogContentSize.height / 3));
        }
        
        i++;
    }
    
    if (getLabelTitle()) {
        getLabelTitle()->setDimensions(m_dialogContentSize.width, 50);
        getLabelTitle()->setHorizontalAlignment(TextHAlignment::CENTER);
        getLabelTitle()->setVerticalAlignment(TextVAlignment::CENTER);
        getLabelTitle()->setPosition(pCenter.x, pCenter.y + m_dialogContentSize.height / 2 - 25.0f);
        this->addChild(getLabelTitle());
    }
    
    if (getLabelContentText()) {
        Label* ltf = getLabelContentText();
        ltf->setPosition(pCenter);
        ltf->setDimensions(m_dialogContentSize.width - m_contentPadding * 2, m_dialogContentSize.height - m_contentPaddingTop);
        ltf->setHorizontalAlignment(TextHAlignment::LEFT);
        
        this->addChild(ltf);
    }
}

void PopAlertDialog::onExit(){
    LayerColor::onExit();
}

void PopAlertDialog::setContentTextShowed(const char* text){
    if (getLabelContentText()) {
        Label* ltf = getLabelContentText();
        ltf->setString(text);
    }
}


