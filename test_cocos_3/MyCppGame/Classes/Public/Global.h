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
    
    bool isEffect();
    void setEffect(bool yn);
    
    void stopEffect(int ID);
    int playEffect(const char* fileName , bool loop);
    int playEffect_sendcard(bool loop);
    int playEffect_place(bool loop);
    int playEffect_timer(bool loop);
    int playEffect_warning(bool loop);
    
protected:
    ~Global();
};


#endif
