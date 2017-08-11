//==========================================================================================
// GamePayment.h
// Created by Dolphin Lee.
// http://blog.csdn.net/ldpjay/article/details/46459509
//==========================================================================================
#ifndef __GAME_PAYMENT_H__
#define __GAME_PAYMENT_H__

//==========================================================================================

#include "cocos2d.h"
USING_NS_CC;

//#include "PaymentDefine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "IOSiAP_Bridge.h"
#endif

//==========================================================================================

class GamePayment
{
    // 单例模式
    //======================================================================================
public:
    static GamePayment *getInstance();
    
protected:
    GamePayment();
    //======================================================================================
    
    // IAP内购（IOS）
    //======================================================================================
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
public:
    // 请求商品信息
    void req_iap(std::string &identifier, iapProductCallback callback);
    
    // 购买请求
    void pay_iap(int quantity, iapPaymentCallback callback);
    
    // 恢复购买
    void restore_iap(iapRestoreCallback restoreCallback, iapRestoreFinishCallback finishCallback);
    
private:
    IOSiAP_Bridge _iap;
#endif
    //======================================================================================
    
    // 支付宝（Android/IOS）
    //======================================================================================
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
public:
    // 初始化支付宝
    void init_alipay(const aliPaymentCallback& callback);

    // 购买请求
    void pay_alipay(float price, const std::string& order, const std::string& subject, const std::string& body);

private:
    Alipay_Bridge _alipay;
#endif
    //======================================================================================
    
};

//==========================================================================================

#endif

