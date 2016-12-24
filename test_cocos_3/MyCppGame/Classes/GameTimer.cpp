#include "GameTimer.h"
#include "Global.h"

GameTimer::GameTimer():m_callbackListener(NULL),m_callback(NULL),m_valid(false){
    
}

GameTimer::~GameTimer(){
    if (m_valid) {
        unschedule(schedule_selector(GameTimer::update));
//        Global::getInstance()->stopEffect(soundID_timer);
    }
}

bool GameTimer::init(){
    label = Label::createWithTTF("", "fonts/STKaiti.ttf", 10.0);
    label->setPosition(0,0);
    
    this->addChild(label);
    
    return true;
}

void GameTimer::start(float time){
    pTime = time;
    m_valid = true;
    
//    soundID_timer = Global::getInstance()->playEffect_timer(true);
    
    char mtime[100];
    if (strcmp(prefixString, "") != 0) {
        sprintf(mtime,"%s%d",prefixString,(int)pTime % 60);
        label->setString(mtime);
    }
    schedule(schedule_selector(GameTimer::update), 1.0);
}

void GameTimer::update(float delta){
    pTime -= delta;
    char mtime[100];
    if (strcmp(prefixString, "") != 0) {
        sprintf(mtime,"%s%d",prefixString,(int)pTime % 60);
        label->setString(mtime);
    }
    
    if (pTime <= 5) {
        Global::getInstance()->playEffect_warning(false);
    }
    
    if (pTime <= 0) {
        unschedule(schedule_selector(GameTimer::update));
//        Global::getInstance()->stopEffect(soundID_timer);
        m_valid = false;
        if (m_callback && m_callbackListener) {
            (m_callbackListener->*m_callback)(this);
        }
    }
}

void GameTimer::showPrefix(){
    char mtime[100];
    if (strcmp(prefixString, "") != 0) {
        sprintf(mtime,"%s",prefixString);
        label->setString(mtime);
    }
}

GameTimer* GameTimer::createTimer(){
    GameTimer* gametimer = new GameTimer;
    if(gametimer && gametimer->init())
    {
        gametimer->autorelease();
        return gametimer;
    }
    else
    {
        delete gametimer;
        gametimer = NULL;
    }
    return NULL;
}

void GameTimer::setCallBackFunc(Ref* target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;
}
