#ifndef __QLImageSprite__
#define __QLImageSprite__

#include "cocos2d.h"

class QLImageSprite : public cocos2d::Sprite
{
public:
    QLImageSprite();
    ~QLImageSprite();
    
    static QLImageSprite* create(const char* backgroudImage, cocos2d::Size size);
private:
    Sprite* bgSprite;
};

#endif /* defined(__QLImageSprite__) */
