#pragma once

#define pkWidth (106.0 / Director::getInstance()->getContentScaleFactor())//牌的宽
#define pkHeight (144.0 / Director::getInstance()->getContentScaleFactor())//牌的高
//花色
enum PokerColor
{
    PokerColor_JokerJunior = 10,
    PokerColor_JokerSenior = 20,
    PokerColor_Spade = 1,
    PokerColor_Heart,
    PokerColor_Club,
    PokerColor_Diamond,
};
//牌点
enum PokerPoint
{
    PokerPoint_Joker = 0,
    PokerPoint_Ace = 1,
    PokerPoint_2,PokerPoint_3,PokerPoint_4,PokerPoint_5,PokerPoint_6,PokerPoint_7,PokerPoint_8,PokerPoint_9,PokerPoint_10,PokerPoint_Jack,PokerPoint_Queen,PokerPoint_King,
};

//牌型
enum PokerType
{
    PokerType_Default = 0,
    PokerType_9_Half,     //9点半
    PokerType_Pair,		      //对子
    PokerType_9,
    PokerType_8_Half,
    PokerType_8,
    PokerType_7_Half,
    PokerType_7,
    PokerType_6_Half,
    PokerType_6,
    PokerType_5_Half,
    PokerType_5,
    PokerType_4_Half,
    PokerType_4,
    PokerType_3_Half,
    PokerType_3,
    PokerType_2_Half,
    PokerType_2,
    PokerType_1_Half,
    PokerType_1,
    PokerType_0_Half,
    PokerType_0,
};

#define getRandomNumberNotEqualRight(from, to) ((int)(from + (arc4random() % (to - from))))//result is [from, to)
#define getRandomNumber(from, to) ((int)(from + (arc4random() % (to - from + 1))))//result is [from to]
