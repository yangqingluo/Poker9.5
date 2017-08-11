//
//  HelpScene.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#include "HelpScene.h"
#include "Global.h"
#include "QLImageSprite.h"

USING_NS_CC;

Scene* HelpScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelpScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void HelpScene::onEnter(){
    Layer::onEnter();
    
    showSettingWithIndex(100);
}
void HelpScene::onExit(){
    Layer::onExit();
}

bool HelpScene::init()
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
                                              CC_CALLBACK_1(HelpScene::buttonCallback, this, 0));
    
    btn_BackItem->setScale(this->getScaleX(), this->getScaleY());
    btn_BackItem->setPosition(Vec2(origin.x + btn_BackItem->getContentSize().width, origin.y + visibleSize.height - btn_BackItem->getContentSize().height));
    
    // create menu, it's an autorelease object
    auto menu = Menu::create(btn_BackItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    auto labelTitle = Label::createWithTTF("帮助", "fonts/STKaiti.ttf", 20);
    labelTitle->setTextColor(Color4B::BLACK);
    labelTitle->setPosition(origin.x + (560.0 / 960.0) * visibleSize.width, origin.y + (600.0 / 640.0) * visibleSize.height);
    this->addChild(labelTitle);
    
    auto listSprite = Sprite::create();
    listSprite->setContentSize(Size((680.0 / 960.0) * visibleSize.width, (520.0 / 640.0) * visibleSize.height));
    listSprite->setPosition(origin.x + 0.95 * visibleSize.width - listSprite->getContentSize().width / 2, origin.y + (40.0 / 640.0 ) * visibleSize.height + listSprite->getContentSize().height / 2);
    this->addChild(listSprite, 1);
    
    char showTitle[3][100] = {"简介","牌型","赔率"};
    char showContent[3][10000] = {"客服微信：WeChatService 关于游戏币充值、兑换等事宜，若有疑问，可联系客服解决。\n\n说明：本游戏使用一副牌，共54张。分为四门：庄，过门（庄下家），天门（庄对家），坎门（庄上家）。庄家一门。闲家若干，押注其他一家至三家闲门。\n\nCopyright ©2017 by 吉林市福禄寿科技有限公司","大王、小王是半点，J、Q、K分别是1点 2点 3点，A是1点。其他牌取自身数字为点数（如红桃6、黑桃6、方块10、梅花3分别为6点、6点、10点、3点），将所发得的两张牌的点数相加（如果相加大于等于10则减去10）所得的点数即为当前牌型。\n对子：由2张一样点数牌（相同花色）组成，红桃方片两个相同点数的牌一起是对子，黑桃草花两个相同点数的牌黑色是对子，红黑则不是。对子不分点数都一样大。对王：2张王组成，也是对子。\n九点半：由大王或小王配数字为9的牌（不论花色） 组成。\n九点：两张牌点数相加为九的牌型。如：黑桃4+方块5 或黑桃7+梅花2。\n八点半：两张牌点数相加为八点半的牌型。如：黑桃8+小王或红桃8+大王。\n八点：两张牌点数相加为八的牌型。如：红桃4+黑桃4或方块10+红桃8。\n……依此类推……\n一点：两张牌点数为一点的牌型，如：红桃10+红桃J或红桃10+黑桃A。\n半点：两张牌点数相加为半点的牌型，如：方块10+大王。\n没点：两张牌点数相加为0点的牌型。如：方块9+红桃J。\n\n大小比较\n九点半>对子（对王算对子，对子不分大小，庄闲都是对子，庄赢)>九点>八点半>八点>七点半>七点>六点半>六点>五点半>五点>四点半>四点>三点半>三点>两点半>两点>一点半>一点>半点>没点\n\n特殊情况判定\n1当庄家与闲家为相同牌型时，庄家赢。\n2三家以上同时为0点时重新开牌。\n3当庄家的本金低于初始本金的1/3是，任意一家闲家可以“刺”，即为单独跟庄家比大小（正常发牌，只比这两家，其余家不计）。闲家赢庄家赔付剩余的筹码，庄家赢闲家赔付庄家剩余筹码等值的筹码。","正常计分：\n闲家和庄家比牌，如果是输了，则输掉所下的注，如果是赢了则赢得等同数量或者成倍的金额。\n\n特殊计分：\n闲家以以下牌型获胜，则赢取所下筹码*对应牌型倍数，相应的庄家扣除闲家所下筹码*对应牌型倍数\n九点半：闲九点半，庄负赔付该闲家9倍下注数的筹码；庄九点半，闲负赔付庄家3倍下注数的筹码\n对子: 闲对子，庄负赔付6倍该闲家下注数；庄对子，闲负赔付3倍下注数\n9点:相互都为3倍\n8点半，8点:相互为2倍\n7点以下:赔付下注的筹码的1倍即本金"};
    for (int i = 0; i < 3; i++) {
        auto btn_list = YNButton::create();
        btn_list->configImage("images/bg_item_hide.png", "", "images/bg_item_show.png");
        btn_list->setScale9Enabled(true);
//        btn_list->setScale((160.0 / 960.0) * visibleSize.width / btn_list->getContentSize().width);
        btn_list->setContentSize(Size((160.0 / 960.0) * visibleSize.width, (65.0 / 173.0) * (160.0 / 960.0) * visibleSize.width));
        btn_list->setTitleColor(Color3B::WHITE);
        btn_list->setTitleText(showTitle[i]);
        btn_list->setTitleFontSize(12);
        btn_list->setPosition(Vec2(origin.x + (100.0 / 960.0) * visibleSize.width, origin.y + 0.5 * visibleSize.height - (i * 1.2) * btn_list->getBoundingBox().size.height));
        btn_list->addTouchEventListener(CC_CALLBACK_2(HelpScene::touchEvent, this));
        btn_list->setTag(100 + i);
        this->addChild(btn_list);
        
        listButtons.pushBack(btn_list);
        
        ScrollView* scrollView = ScrollView::create(listSprite->getContentSize());
        scrollView->setDirection(ScrollView::Direction::VERTICAL);
        scrollView->setPosition(Vec2(0, 0));
        listSprite->addChild(scrollView);
        
        listScrolls.pushBack(scrollView);

        
        auto textField = TextFieldTTF::textFieldWithPlaceHolder("", "fonts/STKaiti.ttf", 16);
        textField->setString(showContent[i]);
        textField->setTextColor(Color4B::BLACK);
        textField->setHorizontalAlignment(TextHAlignment::LEFT);
//        textField->setAlignment(TextHAlignment::LEFT);
        textField->setMaxLineWidth(listSprite->getContentSize().width);
        textField->setDimensions(listSprite->getContentSize().width, textField->getContentSize().height);
        
        float scrollHeight = MAX(textField->getContentSize().height, listSprite->getContentSize().height);
        scrollView->setContentSize(Size(listSprite->getContentSize().width, scrollHeight));
//        textField->setAnchorPoint(Vec2(0.5, 1));
        textField->setPosition(0.5 * listSprite->getContentSize().width, scrollHeight - 0.5 * textField->getContentSize().height);
        scrollView->addChild(textField);
        scrollView->setContentOffset(Vec2(0, -scrollHeight + listSprite->getContentSize().height));
    }
    
    return true;
}


void HelpScene::buttonCallback(cocos2d::Ref* pSender, int index){
    switch (index) {
        case 0:{
            Director::getInstance()->popScene();
        }
            break;
            
        case 1:
        case 2:{
            showSettingWithIndex(index);
        }
            break;
            
        default:
            break;
    }
}

void HelpScene::touchEvent(Ref *pSender, Widget::TouchEventType type){
    Button* button = (Button* )pSender;
    switch (type){
        case Widget::TouchEventType::BEGAN:
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            if (button->getTag() >= 100) {
                this->showSettingWithIndex(button->getTag());
            }
            switch (button->getTag()) {
                case 1:
                case 2:{
                    
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

void HelpScene::showSettingWithIndex(int index){
    if (index >= 100) {
        for (int i = 0; i < listButtons.size(); i++) {
            YNButton* button = listButtons.at(i);
            button->setSelected(button->getTag() == index);
        }
        
        for (int i = 0; i < listScrolls.size(); i++) {
            ScrollView* view = listScrolls.at(i);
            view->setVisible((i + 100) == index);
        }
    }
}

