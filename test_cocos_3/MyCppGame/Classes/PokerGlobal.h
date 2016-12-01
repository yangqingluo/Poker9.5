#pragma once

const int pkWidth = 71;//牌的宽
const int pkHeight = 96;//牌的高
const int pkJianJu = 20;//牌与牌之间的间距
//花色
enum PokerColor
{
    PokerColor_Joker,
    PokerColor_Spade,
    PokerColor_Heart,
    PokerColor_Club,
    PokerColor_Diamond,
};
//牌点
enum PokerPoint
{
    PokerPoint_Ace = 1,
    PokerPoint_2,PokerPoint_3,PokerPoint_4,PokerPoint_5,PokerPoint_6,PokerPoint_7,PokerPoint_8,PokerPoint_9,PokerPoint_10,PokerPoint_Jack,PokerPoint_Queen,PokerPoint_King,
    PokerPoint_JokerJunior = 101,
    PokerPoint_JokerSenior = 102,
};

#define getRandomNumberNotEqualRight(from, to) ((int)(from + (arc4random() % (to - from))))//result is [from, to)
#define getRandomNumber(from, to) ((int)(from + (arc4random() % (to - from + 1))))//result is [from to]
