#ifndef __PUBLIC_HEADER_H__
#define __PUBLIC_HEADER_H__

#define Max_ID_Length 40
#define Max_String_Length 1024

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
//    "introCount": 0,
    int gameTimes;
    int diamond;
    int gold;
    int silver;
//    "token": null,
};

struct TableData {
    int code;
    char tableId[Max_ID_Length];
    char roomId[Max_ID_Length];
    char bureauId[Max_ID_Length];//牌局ID
    char bureauOwnerId[Max_ID_Length];//庄家ID
    char description[Max_String_Length];
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


#endif /* #define __PUBLIC_HEADER_H__ */
