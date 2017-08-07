//
//  CppToFunction.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef CppToFunction_h
#define CppToFunction_h

#include "cocos2d.h"
USING_NS_CC;

#ifdef __cplusplus
extern "C"
{
#endif
extern void callbackAliPay(int code);
#ifdef __cplusplus
}
#endif


class CppToFunction : public Ref{
public:
    static CppToFunction* getInstance();
    
    void doCopyAction(char* m_string);
    void doAlipayAction(char* m_string);
    void doWechatpayAction(char* m_string);
private:
    
protected:
    ~CppToFunction();
};

#endif /* CppToFunction_h */
