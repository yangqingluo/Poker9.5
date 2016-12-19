#ifndef __MTNotificationQueue_h
#define __MTNotificationQueue_h

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

class MTNotificationQueue : public Ref
{
    static MTNotificationQueue * mInstance;
    
    class CGarbo
    {
    public:
        ~CGarbo()
        {
            if (MTNotificationQueue::mInstance)
                delete MTNotificationQueue::mInstance;
            mInstance = NULL;
        }
    };
    
    static CGarbo Garbo;
    
    typedef struct
    {
        string name;
        Ref* object;
    } NotificationArgs;
    
    vector<NotificationArgs> notifications;
    MTNotificationQueue(void);
    
public:
    static MTNotificationQueue* sharedNotificationQueue();
    void postNotifications(float dt);
    ~MTNotificationQueue(void);
    void postNotification(const char* name, Ref* object);
};  

#endif



