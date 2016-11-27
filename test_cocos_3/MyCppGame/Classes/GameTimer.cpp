#include "GameTimer.h"

GameTimer::GameTimer()
{
    
}

GameTimer::~GameTimer()
{
    
}

bool GameTimer::init(float time)
{
    pTime = time;
    
    label = LabelTTF::create();
    label->setPosition(0,0);
    
    this->addChild(label);
    
    schedule(schedule_selector(GameTimer::update));
    
    return true;
}
void GameTimer::update(float delta)
{
    pTime -= delta;
    char* mtime = new char[10];
    //此处只是显示分钟数和秒数  自己可以定义输出时间格式
    sprintf(mtime,"%d : %d",(int)pTime / 60,(int)pTime % 60);
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
