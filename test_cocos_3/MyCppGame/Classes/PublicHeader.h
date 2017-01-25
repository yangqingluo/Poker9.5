#ifndef __PUBLIC_HEADER_H__
#define __PUBLIC_HEADER_H__

#define Max_ID_Length 40
#define Max_String_Length 1024
#define Min_String_Length 100
#define MAX_PLAYER_NUM             7
#define length_room_password       8
#define length_room_capital        6
#define length_invite_code         8
#define Max_Message_Length 100

struct UserData {
    char ID[Max_ID_Length];
    char nikename[50];
    char account[Max_ID_Length];
//    "mobile": "18811610036",
//    "birthday": "1990-08-15",
//    "email": null,
//    "headImgUrl": null,
    char inviteCode[20];
    char winningPercent[20];
    int introCount;
    int gameTimes;
    int diamond;
    int gold;
    int silver;
//    "token": null,
};

struct PlayerData {
    int amount;
    int assassin;
    int bet;
    int capital;
    int joinTime;
    int ready;
    int remainCap;
    int role;
    char room[Max_ID_Length];
    int seat;
    UserData user;
};

struct PokerData {
    int color;
    int num;
    double count;
};

struct PokerPair {
    PokerData poker[2];
    double point;
    char pointDes[Min_String_Length];
};

struct SettleData {
    int count;
//    int result;
    int times;
};

struct RoundData {
    char stabberId[Max_ID_Length];//刺ID
    int stabberIndex;//刺序
    char roundId[Max_ID_Length];//牌把ID
    int roundIndex;
    
    PokerData pokerJudgement;//判定牌(决定发牌顺序的牌)
    PokerPair pokerSendedList[4];
    SettleData settleList[4];
    int betList[4];
};

struct BureauData {
    char bureauId[Max_ID_Length];//牌局ID
    char bureauOwnerId[Max_ID_Length];//庄家ID
};


struct TableData {
    int minPerStack;//最小下注额
    int minStack;//房间金额
    char roomType[Min_String_Length];//房间类型
    char roomId[Max_ID_Length];
    
    int code;
    char tableId[Max_ID_Length];
    char description[Max_String_Length];
    
    BureauData bureau;
    RoundData round;
};




#endif /* #define __PUBLIC_HEADER_H__ */
