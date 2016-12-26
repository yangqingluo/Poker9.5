#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cocos2d.h"
USING_NS_CC;
#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

#include "PublicHeader.h"
#include "ODSocket.h"
#include "tcpcommand.h"

enum RoomType
{
    RoomType_Silver = 0,
    RoomType_Gold,
    RoomType_VIP,
    RoomType_Diamond,
};

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
    int playEffect_add_gold(bool loop);
    int playEffect_select(bool loop);
    int playEffect_button(bool loop);
    
    int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen);
    int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
    int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
    
    void saveLoginData(const rapidjson::Value& val_content);
    void logout();
private:
    ODSocket socket;
    void connectServer();
    void disconnectServer();
    void receiveData();
    
    void socketdidConnect();
    void socketDidDisconnect();
    
    bool endianBig;//大端判断
protected:
    ~Global();
};


#endif
