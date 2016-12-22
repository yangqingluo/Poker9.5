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
class ShadowLayer:public Layer
{
public:
    CREATE_FUNC(ShadowLayer);
    bool init();
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onEnter();
    virtual void onExit();
};
class MessageManager : public Ref
{
public:
    CREATE_FUNC(MessageManager);
    bool init();
    void show(Node* nodeParent, int type,const char *msg);
    void hidden();
    void onWarningHidden(Node *node);
private:
    int         m_iType;
    ShadowLayer *m_pLayer;
};

#endif /* defined(__MyFighter__MessageManager__) */
