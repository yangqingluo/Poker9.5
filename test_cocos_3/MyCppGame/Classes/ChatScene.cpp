//
//  ChatScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "ChatScene.h"
#include "Global.h"
#include "QLImageSprite.h"
#include "BaseCell.h"

ChatScene::ChatScene(){
    NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ChatScene::onNotification_Socket), kNotification_Socket, NULL);
}

ChatScene::~ChatScene(){
    NotificationCenter::getInstance()->removeAllObservers(this);
}

Scene* ChatScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ChatScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void ChatScene::onEnter(){
    Layer::onEnter();
    
}
void ChatScene::onExit(){
    Layer::onExit();
}

bool ChatScene::init()
{
    // super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sprite = Sprite::create("images/bg_yellow_square.png");
    
    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    
    float spx = sprite->getTextureRect().getMaxX();
    float spy = sprite->getTextureRect().getMaxY();
    
    sprite->setScaleX(visibleSize.width / spx); //设置精灵宽度缩放比例
    sprite->setScaleY(visibleSize.height / spy);
    
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    QLImageSprite* navHead = QLImageSprite::create("images/nav_bg.png", Size(visibleSize.width, MIN(0.1 * visibleSize.height, 40)));
    navHead->setPosition(origin.x + 0.5 * visibleSize.width, origin.y + visibleSize.height - 0.5 * navHead->getContentSize().height);
    this->addChild(navHead);
    
    auto btn_BackItem = MenuItemFont::create("返回", CC_CALLBACK_1(ChatScene::buttonCallback, this, 0));
    btn_BackItem->setFontSizeObj(16.0);
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(btn_BackItem->getContentSize().width, 0.5 * navHead->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    navHead->addChild(menu, 1);
    
    float width = navHead->getContentSize().width - btn_BackItem->getBoundingBox().getMaxX();
    auto inputBox = ui::EditBox::create(Size(0.8 * width, MIN(0.8 * navHead->getContentSize().height, 32)), ui::Scale9Sprite::create("images/bg_editbox_normal.png"));
    inputBox->setPosition(Vec2(btn_BackItem->getBoundingBox().getMaxX() + 5 + 0.5 * inputBox->getContentSize().width, 0.5 * navHead->getContentSize().height));
    navHead->addChild(inputBox);
    
    //属性设置
    //    inputBox->setFontName("fonts/STKaiti.ttf");
    inputBox->setFontSize(12);
    inputBox->setFontColor(Color4B::BLACK);
    //    inputBox->setPlaceholderFont("fonts/STKaiti.ttf", 10);
    inputBox->setPlaceholderFontSize(12);
    inputBox->setPlaceholderFontColor(Color4B::GRAY);
    
    //模式类型设置
    inputBox->setInputMode(cocos2d::ui::EditBox::InputMode::SINGLE_LINE);
    inputBox->setInputFlag(cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS);
    inputBox->setReturnType(cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT);
    
    inputBox->setPlaceHolder("请输入喇叭的内容");
    inputBox->setMaxLength(Max_Message_Length);
    msgBox = inputBox;
    
    auto btn_send = Button::create("images/chat_btn_send.png");
    btn_send->setScale9Enabled(true);//打开scale9 可以拉伸图片
    btn_send->setTitleText("发送");
    btn_send->setTitleFontSize(12);
    btn_send->setTitleColor(Color3B::BLACK);
    btn_send->setContentSize(Size(MIN(2 * inputBox->getContentSize().height, 0.15 * width), inputBox->getContentSize().height));
    btn_send->setPosition(Vec2(inputBox->getBoundingBox().getMaxX() + 0.6 * btn_send->getContentSize().width, inputBox->getPositionY()));
    btn_send->setTag(0);
    btn_send->addTouchEventListener(CC_CALLBACK_2(ChatScene::touchEvent, this));
    navHead->addChild(btn_send);
    
    recordListCellWidth = visibleSize.width;
    recordListTableView = TableView::create(this, Size(recordListCellWidth,  visibleSize.height - navHead->getContentSize().height));
    recordListTableView->setPosition(origin.x , origin.y);
    recordListTableView->setDirection(TableView::Direction::VERTICAL);
    recordListTableView->setDelegate(this);
    this->addChild(recordListTableView);
    
    recordListTableView->reloadData();
    
    return true;
}


void ChatScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
        default:
            break;
    }
}

void ChatScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            switch (button->getTag()) {
                case 0:{
                    if (strlen(msgBox->getText()) > 0) {
                        Global::getInstance()->sendMessageToAll(msgBox->getText());
                    }
                    else {
                        NoteTip::show("发送消息不能为空");
                    }
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

#pragma tableview
Size ChatScene::tableCellSizeForIndex(TableView* table, ssize_t idx){
    if (table == recordListTableView) {
        return Size(recordListCellWidth, 40);
    }
    
    return Size::ZERO;
}

//定制每个cell的内容
TableViewCell* ChatScene::tableCellAtIndex(TableView* table, ssize_t idx){
    if (table == recordListTableView) {
        BaseCell* cell = (BaseCell* )table->dequeueCell();
        
        float height = 40;
        if(!cell)
        {
            cell = new BaseCell();
            cell->autorelease();
            
            auto head = Sprite::create();
            cell->addChild(head);
            cell->head = head;
            
            auto titleLabel = Label::create();
            titleLabel->setSystemFontSize(12.0);
            titleLabel->setTextColor(Color4B::BLACK);
            titleLabel->setPosition(0.5 * recordListCellWidth, 0.5 * height);
            titleLabel->setDimensions(0.9 * recordListCellWidth, height);
            titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
            titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
            cell->addChild(titleLabel);
            cell->titleLabel = titleLabel;
            
            auto line = Sprite::create("images/tableview_line.png");
            line->setScaleX(0.9 * recordListCellWidth / line->getContentSize().width);
            line->setPosition(0.5 * recordListCellWidth, 0.5 * line->getContentSize().height);
            cell->addChild(line);
        }
        
        MessageRef* item = Global::getInstance()->messageItems.at(idx);
        
        char m_string[200] = {0};
        sprintf(m_string, "%s\t%s:\t%s", item->sendTime, item->fromUserNikeName, item->message);
        cell->titleLabel->setString(m_string);
        
        return cell;
    }
    
    return NULL;
}

//确定这个tableview的cell行数
ssize_t ChatScene::numberOfCellsInTableView(TableView* table){
    if (table == recordListTableView) {
        return Global::getInstance()->messageItems.size();
    }
    
    return 0;
}

void ChatScene::tableCellTouched(TableView* table, TableViewCell* cell){
    
}

#pragma notification
void ChatScene::onNotification_Socket(Ref* pSender){
    PostRef* post = (PostRef* )pSender;
    switch (post->cmd) {
        case cmd_sendMessageToAll:{
            if (strlen(post->description) > 0) {
                NoteTip::show(post->description);
            }
            
            if (post->sub_cmd == 1) {
                msgBox->setText("");
            }
        }
            break;
            
        case cmd_receiveAllMessage:{
            this->recordListTableView->reloadData();
        }
            break;
            
        default:
            break;
    }
}
