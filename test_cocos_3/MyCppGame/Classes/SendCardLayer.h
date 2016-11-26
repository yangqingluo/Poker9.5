//
//  SendCardLayer.h
//  HelloWorldDemo
//
//  Created by xiaohua on 15/4/10.
//
//

#ifndef __HelloWorldDemo__SendCardLayer__
#define __HelloWorldDemo__SendCardLayer__

#include "cocos2d.h"

USING_NS_CC;

class SendCardLayer : public cocos2d::Layer
{
public:
    SendCardLayer();
    ~SendCardLayer();
    
    virtual bool init();
    CREATE_FUNC(SendCardLayer)
    
protected:
    void runSendAmt(Ref * obj);
    
private:
    Size m_winSize;
    Vector<cocos2d::Sprite *> m_cardVec;
    Vector<cocos2d::Sprite *> m_cardBg;
};

#endif /* defined(__HelloWorldDemo__SendCardLayer__) */
