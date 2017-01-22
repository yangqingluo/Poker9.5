#ifndef __ExchangeCell_H__
#define __ExchangeCell_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class ExchangeCell : public cocos2d::extension::TableViewCell
{
public:
	virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
    Sprite* head;
    Label* titleLabel;
};

#endif /* __ExchangeCell_H__ */

