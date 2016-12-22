#ifndef __MyFighter__MessageManager__
#define __MyFighter__MessageManager__

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

enum
{
    MESSAGETYPE_LOADING,
    MESSAGETYPE_WARNING,
};


class MessageManager : public LayerColor
{
public:
    MessageManager();
    ~MessageManager();
    CREATE_FUNC(MessageManager);
    bool init();
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onEnter();
    virtual void onExit();
    
    static MessageManager* show(Node* nodeParent, int type,const char *msg);
    void hidden();
    void onWarningHidden(Node *node);
private:
    int m_iType;
};

#endif /* defined(__MyFighter__MessageManager__) */
