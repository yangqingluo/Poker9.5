#pragma once
#include "cocos2d.h"
USING_NS_CC;

class Player : public Ref
{
public:
    Player();
    ~Player();
    
    char nickName[100];
    char headImage[100];
    void infoConfig(const char* nick, const char* head, int jetton);
private:
    CC_SYNTHESIZE(bool,m_isDiZhu,IsDiZhu);//是否为地主
    CC_SYNTHESIZE(bool,m_isCall,Call);//是否已叫地主
    CC_SYNTHESIZE(int,m_iPlayerClass,PlayerClass);//玩家种类:0为电脑，1为玩家
    CC_SYNTHESIZE(int,m_jettonCount,JettonCount);//筹码数目
    CC_SYNTHESIZE_READONLY(int,m_jettonInitial,JettonInitial);//本金
};



