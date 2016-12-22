#include "NoteTip.h"
USING_NS_CC;


NoteTip::NoteTip()
{
}
NoteTip::~NoteTip()
{
}


bool NoteTip::init()
{
    if (!Layer::init())
    {
        return false;
    }
    return true;
}
void NoteTip::show(const char *content)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    NoteTip *noteTip = NoteTip::create();
    Label *label = Label::create(content, "", 16);
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto sp = ui::Scale9Sprite::create("images/bg_notice.png");
    float fontSize = label->getSystemFontSize();
    float spWidth = label->getStringLength() * (fontSize / 2);
    sp->setContentSize(Size(MAX(spWidth, 0.45 * visibleSize.width), fontSize + 10));
    sp->setCapInsets(Rect(6, 6, 1, 1));
    sp->setScale9Enabled(true);
    sp->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2.5f);
    sp->setTag(100);
    
    
    label->setPosition(sp->getContentSize() / 2);
    sp->addChild(label);
    noteTip->addChild(sp);
    
    
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(noteTip);
}
void NoteTip::onEnter()
{
    Layer::onEnter();
    ui::Scale9Sprite *sp = (ui::Scale9Sprite*)this->getChildByTag(100);
    if (sp)
    {
        auto seq = Sequence::create(
                                    MoveBy::create(1.0f, Vec2(0, 180)),
                                    DelayTime::create(0.2f),
                                    FadeOut::create(0.2f),
                                    CallFunc::create([&](){this->removeFromParent();}),
                                    nullptr
                                    );
        sp->runAction(seq);
    }
    
    
}
void NoteTip::onExit()
{
    Layer::onExit();
    
    
}
