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

#define MAX_NET_DATA_LEN (10 * 1024)
#define MAX_PLAYER_NUM       7

#define reversebytes_uint32t(value) ((value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24)//int 大小端转换

#define kNotification_Socket           "notification_socket"
#define kNotification_RefreshUserInfo  "notification_refreshUserInfo"

#define cmd_handle                     1000//握手

#define cmd_removePlayer               1000//踢出房间通知
#define cmd_beginCountDownBeforeBureau 1001//牌局开始前倒计时通知
#define cmd_synPlayerList              1002//同步玩家列表通知
#define cmd_bureauOpen                 1003//开始牌局通知
#define cmd_countDownApplyBureauOwner  1004//抢庄通知
#define cmd_selectedBureauOwner        1005//选中庄家通知
#define cmd_countDownApplyStabber      1006//抢刺通知

#define cmd_countDownBetStake          1008//下注通知
#define cmd_countDownSendCard          1009//发牌通知
#define cmd_sysBetStake                1010//同步下注数据通知
#define cmd_settle                     1011//结算通知


#define cmd_enterRoom                  3000//加入普通金币房间
#define cmd_leaveRoom                  3001//退出房间

#define cmd_playerReady                3003//玩家准备
#define cmd_applyOwner                 3004//申请上庄
#define cmd_betStake                   3005//下注
#define cmd_supplyBit                  3006//补充本金

#define cmd_applyStabber               3040//抢刺

#define cmd_enterRoomByPassword        4000//加入需要密码的指定房间

//牌局状态
enum DeskState
{
    DeskState_Default = 0,
    DeskState_Waiting = 1,//等待准备
    DeskState_Prepared,//已准备
    DeskState_Start,//开始
    DeskState_ChooseDealer,//抢庄
    DeskState_Bet,//下注
    DeskState_SendPoker,//发牌
    DeskState_ChooseStabber,//抢刺
    DeskState_Settle,//结算
};

//房间类型
enum RoomType
{
    RoomType_Silver = 0,
    RoomType_Gold,
    RoomType_VIP,
    RoomType_Diamond,
};


class PostRef: public Ref{
public:
    PostRef();
    ~PostRef();
    
    int cmd;
    char description[1024];
};

class Global: public Ref{
public:
    static Global* getInstance();
    
    UserData user_data;//用户数据
    TableData table_data;//牌桌数据
    int countDownInSecond;//倒计时读秒数
    bool isDealer;//是否是庄家
    
    int playerListCount = 0;
    PlayerData playerList[MAX_PLAYER_NUM];
    PokerData pokerJudgement;//判定牌(决定发牌顺序的牌)
    PokerPair pokerSendedList[4];
    int settleList[4];
    int betList[4];
    
    void clearPlayerList();
    void clearPokerSendedList();
    
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
    
    int getInt(char *buffer, int offset);
    
    void saveLoginData(const rapidjson::Value& val_content);
    void logout();
    
    void sendHandle();
    void sendEnterRoom(const char* roomTypeId, int capital);
    void sendPlayerReady();
    void sendApplyOwner();
    void sendLeaveRoom();
    void sendBetStake(int jetton, int gateType);
    
    void parseUserData(const rapidjson::Value& val_user, UserData* data_user);
    void parsePlayerData(const rapidjson::Value& val_player, PlayerData* data_player);
    
private:
    char m_ucRecvBuffer[MAX_NET_DATA_LEN] = {0};//缓冲区
    unsigned int m_nRecvLen = 0;
    unsigned int m_nRecvFrameLen = 0;
    
    ODSocket socket;
    void connectServer();
    void disconnectServer();
    void receiveData();
    void onReceiveData(char *buffer, int len);
    void sendData(const char* value);
    
    void socketdidConnect();
    void socketDidDisconnect();
    
    bool endianBig;//大端判断
    
    void postNotification(int cmd);
    void postNotification(PostRef* post);
    void parseData(char* pbuf, int len);
    
protected:
    ~Global();
};




#endif
