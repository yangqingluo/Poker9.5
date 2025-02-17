//
//  CppToFunction.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "OCFunction.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "JavaToC.h"
#include "JavaToCPay.h"
#endif

#include "CppToFunction.h"
#include "Global.h"

void callbackAliPay(int code){
    PostRef* post = new PostRef();
    post->cmd = PayStyle_alipay;
    post->sub_cmd = code;
    
    MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_Pay, post);
    if (code == 9000) {
        Global::getInstance()->user_data.gold += Global::getInstance()->goldToRecharge;
        MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_RefreshUserInfo, NULL);
    }
}

void callbackWechatPay(int code){
    PostRef* post = new PostRef();
    post->cmd = PayStyle_wx;
    post->sub_cmd = code;
    
    MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_Pay, post);
    if (code == 0) {
        Global::getInstance()->user_data.gold += Global::getInstance()->goldToRecharge;
        MTNotificationQueue::sharedNotificationQueue()->postNotification(kNotification_RefreshUserInfo, NULL);
    }
}

static CppToFunction* share = nullptr;

CppToFunction::~CppToFunction(void){
    
}

CppToFunction* CppToFunction::getInstance(){
    if(!share){
        share = new (std::nothrow) CppToFunction();
        
    }
    
    return share;
}


#pragma public
void CppToFunction::doCopyAction(char* m_string){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS代码
    nsCopy(m_string);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //Android代码
    JavaToC* java2c = new JavaToC();
    java2c->getJavaCopyStr(m_string);
#endif
}

void CppToFunction::doAlipayAction(char* m_string){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS代码
    nsDoAlipay(m_string);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //Android代码
    JavaToCPay* java2cPay = new JavaToCPay();
    java2cPay->getJavaPayStr(m_string);
#endif
}

void CppToFunction::doWechatpayAction(char* m_string){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS代码
    nsDoWechatpay(m_string);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //Android代码
    JavaToCPay* java2cPay = new JavaToCPay();
    java2cPay->getJavaPayStrForWechat(m_string);
#endif
}
