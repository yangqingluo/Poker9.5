#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "cocos2d.h"
USING_NS_CC;

#include "PublicHeader.h"

class Global: public Ref{
public:
    static Global* getInstance();
    
    UserData user_data;
protected:
    ~Global();
};


#endif
