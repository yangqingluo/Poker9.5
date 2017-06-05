#include "MTNotificationQueue.h"



pthread_mutex_t sharedNotificationQueueLock;

class LifeManager_PThreadMutex
{
    pthread_mutex_t* mutex;
    
public:
    LifeManager_PThreadMutex(pthread_mutex_t* mut) : mutex(mut)
    {
        pthread_mutex_init(mutex, NULL);
    }
    
    ~LifeManager_PThreadMutex()
    {
        pthread_mutex_destroy(mutex);
    }
}__LifeManager_sharedNotificationQueueLock(&sharedNotificationQueueLock);

class LifeCircleMutexLocker
{
    pthread_mutex_t* mutex;
    
public:
    LifeCircleMutexLocker(pthread_mutex_t* aMutex) : mutex(aMutex)
    {
        pthread_mutex_lock(mutex);
    }
    ~LifeCircleMutexLocker(){
        pthread_mutex_unlock(mutex);
    }
};

#define LifeCircleMutexLock(mutex) LifeCircleMutexLocker __locker__(mutex)




MTNotificationQueue* MTNotificationQueue::mInstance = NULL;

MTNotificationQueue::MTNotificationQueue(void)
{
}

MTNotificationQueue::~MTNotificationQueue(void)
{
}

MTNotificationQueue* MTNotificationQueue::sharedNotificationQueue()
{
    if (!mInstance)
    {
        mInstance = new MTNotificationQueue();
    }
    
    return mInstance;
}

void MTNotificationQueue::postNotifications(float dt)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([&, this] {
        LifeCircleMutexLock(&sharedNotificationQueueLock);
        
        for(uint16_t i = 0; i < notifications.size(); i++) {
            NotificationArgs &arg = notifications[i];
            NotificationCenter::getInstance()->postNotification(arg.name.c_str(), arg.object);
        }
        notifications.clear();
    });
    
}

void MTNotificationQueue::postNotification(const char* name, Ref* object)
{
    LifeCircleMutexLock(&sharedNotificationQueueLock);
    
    NotificationArgs arg;
    arg.name = name;
    
    if(object != NULL)
        arg.object = object;   //object->copy();
    else
        arg.object = NULL;
    
    notifications.push_back(arg);
}
