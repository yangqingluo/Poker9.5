#include "Global.h"
#include "HallScene.h"

#include <iconv.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#define key_BackgroundMusic "background_music_game"
#define key_Effect          "effect_game"

#define EFFECT_sendcard     "music/sendcard.wav"
#define EFFECT_place        "music/place.wav"
#define EFFECT_timer        "music/timer.wav"
#define EFFECT_warning      "music/warning.wav"
#define EFFECT_add_gold     "music/add_gold.wav"
#define EFFECT_select       "music/select.wav"
#define EFFECT_button       "music/button.wav"

//音效文件
//Android平台只支持OGG的音效格式
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define EFFECT_FILE        "music/effect2.ogg"
#elif( CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
#define EFFECT_FILE        "music/effect1.raw"
#else
#define EFFECT_FILE        "music/effect1.wav"
#endif

//音乐文件
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define MUSIC_FILE        "music/music.mid"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_BLACKBERRY || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX )
#define MUSIC_FILE        "music/background.ogg"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
#define MUSIC_FILE        "music/background.wav"
#else
#define MUSIC_FILE        "music/background.mp3"
#endif

static Global* share = nullptr;

Global::~Global(void){
    user_data = {0};
}
Global* Global::getInstance(){
    if(!share){
        share = new (std::nothrow) Global();
        
        //预加载音乐音效
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FILE);
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(EFFECT_FILE);
        
        int nNum = 0x12345678;
        char chData = *(char*)(&nNum);
        share->endianBig = (chData == 0x12);
    }
    
    return share;
}

bool Global::isBackgroundMusic(){
    bool canPlay = UserDefault::getInstance()->getBoolForKey(key_BackgroundMusic);
    
    return canPlay;
}

void Global::setBackgroundMusic(bool yn){
    UserDefault::getInstance()->setBoolForKey(key_BackgroundMusic, yn);
    playBackgroundMusic(yn);
}

void Global::playBackgroundMusic(bool yn){
    if (yn) {
//        if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()) {
//            
//        }
        bool canPlay = isBackgroundMusic();
        if (canPlay) {
            SimpleAudioEngine::getInstance()->playBackgroundMusic(MUSIC_FILE, true);//播放音乐，循环
        }
    }
    else {
        SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }
}

bool Global::isEffect(){
    return UserDefault::getInstance()->getBoolForKey(key_Effect);
}
void Global::setEffect(bool yn){
    UserDefault::getInstance()->setBoolForKey(key_Effect, yn);
    if (!yn) {
        SimpleAudioEngine::getInstance()->stopAllEffects();
    }
}

void Global::stopEffect(int ID){
    SimpleAudioEngine::getInstance()->stopEffect(ID);
}

int Global::playEffect(const char* fileName , bool loop){
    if (isEffect()) {
        return SimpleAudioEngine::getInstance()->playEffect(fileName, loop);
    }
    
    return -1;
}
//发牌声
int Global::playEffect_sendcard(bool loop){
    return playEffect(EFFECT_sendcard, loop);
}

//请下注
int Global::playEffect_place(bool loop){
    return playEffect(EFFECT_place, loop);
}

//计时
int Global::playEffect_timer(bool loop){
    return playEffect(EFFECT_timer, loop);
}

//警告
int Global::playEffect_warning(bool loop){
    return playEffect(EFFECT_warning, loop);
}

int Global::playEffect_add_gold(bool loop){
    return playEffect(EFFECT_add_gold, loop);
}
int Global::playEffect_select(bool loop){
    return playEffect(EFFECT_select, loop);
}

int Global::playEffect_button(bool loop){
    return playEffect(EFFECT_button, loop);
}


int Global::code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    iconv_t cd;
    const char *temp = inbuf;
    const char **pin = &temp;
    char **pout = &outbuf;
    memset(outbuf, 0, outlen);
    
    cd = iconv_open(to_charset, from_charset);
    if (cd == 0) {
        return -1;
    }
    if (iconv(cd, (char **)pin, &inlen, pout, &outlen) == -1) {
        return -1;
    }
    iconv_close(cd);
    return 0;
}

int Global::u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert("UTF-8", "GBK", inbuf, inlen, outbuf, outlen);
}

int Global::g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) {
    return code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen);
}


void Global::saveLoginData(const rapidjson::Value& val_content){
    user_data = {0};
    user_data.gameTimes = val_content["gameTimes"].GetInt();
    
    const char* nikename = val_content["nikename"].GetString();
    memcpy(user_data.nikename, nikename, strlen(nikename));
    
    const char* account = val_content["account"].GetString();
    memcpy(user_data.account, account, strlen(account));
    
    const char* winningPercent = val_content["winningPercent"].GetString();
    memcpy(user_data.winningPercent, winningPercent, strlen(winningPercent));
    
    const char* inviteCode = val_content["inviteCode"].GetString();
    memcpy(user_data.inviteCode, inviteCode, strlen(inviteCode));
    
    if (val_content.HasMember("diamondGameBit")) {
        const rapidjson::Value& val_diamondGameBit = val_content["diamondGameBit"];
        user_data.diamond = val_diamondGameBit["amount"].GetInt();
    }
    
    if (val_content.HasMember("silverGameBit")) {
        const rapidjson::Value& val_silverGameBit = val_content["silverGameBit"];
        user_data.silver = val_silverGameBit["amount"].GetInt();
    }
    
    if (val_content.HasMember("goldGameBit")) {
        const rapidjson::Value& val_goldGameBit = val_content["goldGameBit"];
        user_data.gold = val_goldGameBit["amount"].GetInt();
    }
    
    auto scene = Hall::createScene();
    Director::getInstance()->replaceScene(scene);
    
    this->connectServer();
}

void Global::logout(){
    Director::getInstance()->popToRootScene();
    this->playBackgroundMusic(false);
    
    this->disconnectServer();
}

#pragma Socket
void Global::disconnectServer(){
    // 关闭连接
    socket.Close();
    socket.Clean();
}
void Global::connectServer(){
    // 初始化
    // ODSocket socket;
    socket.Init();
    socket.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 设置服务器的IP地址，端口号
    // 并连接服务器 Connect
    const char* ip = "115.28.109.174";
    int port = 8888;
    bool result = socket.Connect(ip, port);
    
    //发送数据 Send
    SEND_PACKAGE package = {0};
    char handle[200];
    sprintf(handle, "{\"id\":1000,\"content\":{\"userId\":%s}}", user_data.account);
    
    int length = (int)strlen(handle);
    if (!endianBig) {
        package.valueLength = reversebytes_uint32t(length);
    }
    else {
        package.valueLength = length;
    }
    
    memcpy(package.value, handle, length);
    
    int result_send = socket.Send((const char *)&package, sizeof(int) + length);
    if (result_send > 0) {
        log("Socket::send->%s\n",package.value);
    }
    
    if (result) {
        CCLOG("Socket::connect to server success!");
        // 开启新线程，在子线程中，接收数据
        std::thread recvThread = std::thread(&Global::receiveData, this);
        recvThread.detach(); // 从主线程分离
    }
    else {
        log("Socket::can not connect to server");
        return;
    }
}

// 接收数据
void Global::receiveData(){
    // 因为是强联网
    // 所以可以一直检测服务端是否有数据传来
    while (true) {
        // 接收数据 Recv
        char data[512] = "";
        int result = socket.Recv(data, 512, 0);
        log("Socket::%d\n", result);
        // 与服务器的连接断开了
        if (result <= 0){
            log("Socket::disconnect");
            break;
        }
        
        log("Socket::receive->%s\n", data + 4);
    }
    log("&&&&&&");
    // 关闭连接
    socket.Close();
}
