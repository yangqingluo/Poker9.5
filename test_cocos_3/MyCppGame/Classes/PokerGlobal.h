#pragma once
#include <vector>
const int pkWidth = 71;//牌的宽
const int pkHeight = 96;//牌的高
const int pkJianJu = 20;//牌与牌之间的间距
//花色
enum PokerColor
{
    PokerColor_Joker,
    PokerColor_Spade,
    PokerColor_Heart,
    PokerColor_Diamond,
    PokerColor_Club,
};
//牌点
enum PokerPoint
{
    PokerPoint_Ace = 1,
    PokerPoint_2,PokerPoint_3,PokerPoint_4,PokerPoint_5,PokerPoint_6,PokerPoint_7,PokerPoint_8,PokerPoint_9,PokerPoint_10,PokerPoint_Jack,PokerPoint_Queen,PokerPoint_King,
    PokerPoint_JokerJunior = 101,
    PokerPoint_JokerSenior = 102,
};