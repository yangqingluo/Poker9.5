#pragma once

#include "cocos2d.h"
USING_NS_CC;

#include "PokerGlobal.h"

class JettonSprite : public Sprite
{
public:
    JettonSprite();
    ~JettonSprite();
    
    static JettonSprite* create(int value, Size size);
    void setSelected(bool select);
    bool isValidValue(int value);
private:
    CC_SYNTHESIZE_READONLY(bool,m_isSelected,IsSelect);//是否已选
    CC_SYNTHESIZE_READONLY(int,m_value,JettonValue);//筹码值
    
    Sprite* bgSprite;
    Sprite* valueSprite;
};
