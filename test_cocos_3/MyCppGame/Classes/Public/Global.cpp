#include "Global.h"

#define key_BackgroundMusic "background_music_game"
#define key_Effect          "effect_game"

#define EFFECT_sendcard     "music/sendcard.wav"
#define EFFECT_place        "music/place.wav"
#define EFFECT_timer        "music/timer.wav"
#define EFFECT_warning      "music/warning.wav"

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


