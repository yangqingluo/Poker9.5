#ifndef __H_NOTETIP_H__
#define __H_NOTETIP_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#define showNoteTipTag "NotiTip_Game"

class NoteTip : public Layer
{
public:
    NoteTip();
    ~NoteTip();
    CREATE_FUNC(NoteTip);
    static void show(const char *content);
    static void show(Node* pParent, const char *content);
    
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
};


#endif
