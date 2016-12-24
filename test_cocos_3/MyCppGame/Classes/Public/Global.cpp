#include "Global.h"

#define key_BackgroundMusic "background_music_game"
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
