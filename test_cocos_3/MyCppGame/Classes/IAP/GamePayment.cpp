#include "GamePayment.h"

//==========================================================================================
// 初始化
//==========================================================================================

GamePayment *GamePayment::getInstance()
{
	static GamePayment _instance;
	return &_instance;
}

GamePayment::GamePayment()
{
    
}

//==========================================================================================
// IAP内购（IOS）
//==========================================================================================

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

// 初始化（获取商品信息）
void GamePayment::req_iap(std::string &identifier, iapProductCallback callback)
{
    _iap.requestProducts(identifier, callback);
}

// 付款
void GamePayment::pay_iap(int quantity, iapPaymentCallback callback)
{
    _iap.requestPayment(quantity, callback);
}

// 恢复购买
void GamePayment::restore_iap(iapRestoreCallback restoreCallback, iapRestoreFinishCallback finishCallback)
{
    _iap.requestRestore(restoreCallback, finishCallback);
}

#endif


//==========================================================================================
// 支付宝（IOS／Android）
//==========================================================================================

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

// 初始化
void GamePayment::init_alipay(const aliPaymentCallback& callback)
{
	_alipay.init(callback);
}

void GamePayment::pay_alipay(float price, const std::string& order, const std::string& subject, const std::string& body)
{
	_alipay.payment(price, order, subject, body);
}

#endif

//==========================================================================================
//
//==========================================================================================

