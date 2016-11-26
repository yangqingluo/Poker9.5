//
//  SendCardTest.h
//  HelloWorldDemo
//
//  Created by xiaohua on 15/4/9.
//
//

#ifndef __HelloWorldDemo__SendCardTest__
#define __HelloWorldDemo__SendCardTest__

#include "cocos2d.h"

USING_NS_CC;

class SendCardTest : public cocos2d::Layer
{
public:
    SendCardTest();
    ~SendCardTest();
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(SendCardTest)

};

#endif /* defined(__HelloWorldDemo__SendCardTest__) */
