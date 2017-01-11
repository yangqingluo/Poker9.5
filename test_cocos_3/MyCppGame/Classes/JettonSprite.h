#pragma once

#include "cocos2d.h"
USING_NS_CC;

class JettonSprite : public Sprite
{
public:
    JettonSprite();
    ~JettonSprite();
    
    bool isPlayer;//是否当前玩家下注
    static JettonSprite* create(int value, Size size);
    static JettonSprite* createWithEmpty(int value, Size size);
    bool isValidValue(int value);
    int validValueInEmpty(int value);
    void stringFromJettonValue(char* mString, int value);
private:
    CC_SYNTHESIZE_READONLY(int,m_value,JettonValue);//筹码值
    Sprite* valueSprite;
    Label* valueLabel;
};
