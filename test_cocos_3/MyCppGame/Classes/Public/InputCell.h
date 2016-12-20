#ifndef __InputCell__
#define __InputCell__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class InputCell : public TableViewCell
{
public:
    InputCell();
    ~InputCell();
    virtual bool init();
    CREATE_FUNC(InputCell);
    
    Label* titleLabel;
private:
    Sprite* bgSprite;
};

#endif /* defined(__InputCell__) */
