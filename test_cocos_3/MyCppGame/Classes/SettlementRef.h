//
//  SettlementRef.h
//
//  Created by yangqingluo on 16/11/9.
//
//

#ifndef __SettlementRef__
#define __SettlementRef__

#include "cocos2d.h"
USING_NS_CC;

class SettlementRef : public Ref{
public:
    SettlementRef();
    ~SettlementRef();
    int multiple;//结算倍数
    int accounts;//结算数目
    bool winned;
};

#endif /* defined(__SettlementRef__) */
