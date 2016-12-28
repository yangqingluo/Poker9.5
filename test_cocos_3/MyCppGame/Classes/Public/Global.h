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
#include "MTNotificationQueue.h"

#define reversebytes_uint32t(value) ((value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24)//int 大小端转换

#define kNotification_Socket "notification_socket"

#define cmd_handle                     1000//握手
#define cmd_enterRoom                  3000//加入普通金币房间
#define cmd_leaveRoom                  3001//退出房间


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
    TableData table_data;
    
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
    
    void sendHandle();
    void sendEnterRoom(const char* roomTypeId, int capital);
    void sendLeaveRoom();
private:
    ODSocket socket;
    void connectServer();
    void disconnectServer();
    void receiveData();
    void sendData(const char* value);
    
    void socketdidConnect();
    void socketDidDisconnect();
    
    bool endianBig;//大端判断
    
    void postNotification(int cmd);
    void parseData(char* pbuf);
protected:
    ~Global();
};

class PostRef: public Ref{
public:
    PostRef();
    ~PostRef();
    
    int cmd;
    
};


#endif
