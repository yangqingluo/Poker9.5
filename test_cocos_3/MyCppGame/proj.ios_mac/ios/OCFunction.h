//
//  OCFunction.h
//  LocationFingerPrint
//
//  Created by yangqingluo on 16/6/16.
//  Copyright © 2016年 yangqingluo. All rights reserved.
//

#ifdef __cplusplus
extern "C"
{
#endif
    void nsCopy(const char *string);
    void nsDoAlipay(const char *string);
    void nsDoWechatpay(const char *string);
    void appAlipayCallback(int code);
    void appWechatPayCallback(int code);
#ifdef __cplusplus
}
#endif
