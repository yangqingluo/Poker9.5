//
//  AppPublic.m
//  SafetyOfMAS
//
//  Created by yangqingluo on 16/9/9.
//  Copyright © 2016年 yangqingluo. All rights reserved.
//

#import "AppPublic.h"
#import "OCFunction.h"
#import "Order.h"
#import "APAuthV2Info.h"
#import "RSADataSigner.h"
#import <AlipaySDK/AlipaySDK.h>
#import "WXApiManager.h"

@implementation AppPublic

__strong static AppPublic  *_singleManger = nil;

+ (AppPublic *)shareInstance{
    static dispatch_once_t pred = 0;
    
    dispatch_once(&pred, ^{
        _singleManger = [[AppPublic alloc]init];
        
        
    });
    return _singleManger;
}

- (instancetype)init{
    if (_singleManger) {
        return _singleManger;
    }
    
    self = [super init];
    if (self) {
        
    }
    
    return self;
}



#pragma public
//检查该版本是否第一次使用
BOOL isFirstUsing(){
    //#if DEBUG
    //    NSString *key = @"CFBundleVersion";
    //#else
    NSString *key = @"CFBundleShortVersionString";
    //#endif
    
    // 1.当前版本号
    NSString *version = [NSBundle mainBundle].infoDictionary[key];
    
    // 2.从沙盒中取出上次存储的版本号
    NSString *saveVersion = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
    // 3.写入本次版本号
    [[NSUserDefaults standardUserDefaults] setObject:version forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
    
    return ![version isEqualToString:saveVersion];
}


- (NSString *)generateTradeNO{
    static int kNumber = 15;
    
    NSString *sourceStr = @"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    NSMutableString *resultStr = [[NSMutableString alloc] init];
    srand((unsigned)time(0));
    for (int i = 0; i < kNumber; i++)
    {
        unsigned index = rand() % [sourceStr length];
        NSString *oneStr = [sourceStr substringWithRange:NSMakeRange(index, 1)];
        [resultStr appendString:oneStr];
    }
    return resultStr;
}

//
//选中商品调用支付宝极简支付
//
- (void)doAlipayPay:(NSString *)orderString;{
    //应用注册scheme,在AliSDKDemo-Info.plist定义URL types
    NSString *appScheme = @"fulushouNineHalf";
    // NOTE: 调用支付结果开始支付
    [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
        NSLog(@"doAlipayPay reslut = %@",resultDic);
        if (resultDic[@"memo"]) {
            NSString *memo = [NSString stringWithFormat:@"%@",resultDic[@"memo"]];
            if (memo.length) {
                NSLog(@"支付结果:%@",memo);
            }
            
        }
        
        //            9000 	订单支付成功
        //            8000 	正在处理中
        //            4000 	订单支付失败
        //            6001 	用户中途取消
        //            6002 	网络连接出错
        
        if ([resultDic[@"resultStatus"] intValue] == 9000) {
            
        }
    }];
}

//微信支付
- (void)doWechatPay:(NSString *)orderString{
    NSDictionary *prepay = [self dictionaryWithJsonString:orderString];
    //调起微信支付
    PayReq* req             = [[PayReq alloc] init];
    
    req.partnerId           = @"1459543802";
    req.prepayId            = prepay[@"prepay_id"];
    req.nonceStr            = prepay[@"noncestr"];
    req.timeStamp           = [prepay[@"timestamp"] intValue];
    req.package             = @"Sign=WXPay";
    req.sign                = prepay[@"sign"];
    [WXApi sendReq:req];
}

- (void)alipayResult:(NSDictionary *)resultDic{
    appAlipayCallback([resultDic[@"resultStatus"] intValue]);
}

- (void)wechatPayResult:(int)code{
    appWechatPayCallback(code);
}

- (NSDictionary *)dictionaryWithJsonString:(NSString *)jsonString{
    if (jsonString == nil) {
        return nil;
    }
    
    NSData *jsonData = [jsonString dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData
                                                        options:kNilOptions
                                                          error:nil];
    return dic;
}

@end
