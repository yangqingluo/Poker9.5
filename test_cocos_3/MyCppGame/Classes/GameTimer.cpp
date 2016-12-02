#include "GameTimer.h"

GameTimer::GameTimer():m_callbackListener(NULL),m_callback(NULL)
{
    
}

GameTimer::~GameTimer()
{
    
}

bool GameTimer::init(float time){
    label = Label::create();
    label->setPosition(0,0);
    
    this->addChild(label);
    this->start(time);
    
    return true;
}

void GameTimer::start(float time){
    pTime = time;
    schedule(schedule_selector(GameTimer::update));
}


void GameTimer::update(float delta)
{
    pTime -= delta;
    char* mtime = new char[100];
    if (strcmp(prefixString, "") != 0) {
        sprintf(mtime,"%s%d",prefixString,(int)pTime % 60);
    }
    else{
        sprintf(mtime," ");
    }
    
    label->setString(mtime);
    
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
    }
    else{
        sprintf(mtime," ");
    }
    
    label->setString(mtime);
}

GameTimer* GameTimer::createTimer(float time)
{
    GameTimer* gametimer = new GameTimer;
    if(gametimer && gametimer->init(time))
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
    m_callbackListener=target;
    m_callback=callfun;
}
