#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cocos2d.h"
USING_NS_CC;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "PublicHeader.h"

class Global: public Ref{
public:
    static Global* getInstance();
    
    UserData user_data;
    
    bool isBackgroundMusic();
    void setBackgroundMusic(bool yn);
    void playBackgroundMusic(bool yn);
protected:
    ~Global();
};


#endif
