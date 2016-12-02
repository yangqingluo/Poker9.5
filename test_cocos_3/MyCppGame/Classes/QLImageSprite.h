#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "PokerGlobal.h"

class QLImageSprite : public Sprite
{
public:
    QLImageSprite();
    ~QLImageSprite();
    
    static QLImageSprite* create(const char* backgroudImage, Size size);
private:
    Sprite* bgSprite;
};
