#include "GameTimer.h"

GameTimer::GameTimer():m_callbackListener(NULL),m_callback(NULL){
    
}

GameTimer::~GameTimer(){
    
}

bool GameTimer::init(){
    label = Label::createWithTTF("", "fonts/STKaiti.ttf", 12.0);
    label->setPosition(0,0);
    
    this->addChild(label);
    
    return true;
}

void GameTimer::start(float time){
    pTime = time;
    schedule(schedule_selector(GameTimer::update));
}


void GameTimer::update(float delta){
    pTime -= delta;
    char* mtime = new char[100];
    if (strcmp(prefixString, "") != 0) {
        sprintf(mtime,"%s%d",prefixString,(int)pTime % 60);
        label->setString(mtime);
    }
    
    if (pTime <= 1) {
        unschedule(schedule_selector(GameTimer::update));
        
        if (m_callback&&m_callbackListener) {
            (m_callbackListener->*m_callback)(this);
        }
    }
}

void GameTimer::showPrefix(){
    char* mtime = new char[100];
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

void GameTimer::setCallBackFunc(Ref*target, SEL_CallFuncN callfun){
    m_callbackListener = target;
    m_callback = callfun;
}
