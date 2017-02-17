//
//  AppPublic.m
//  SafetyOfMAS
//
//  Created by yangqingluo on 16/9/9.
//  Copyright © 2016年 yangqingluo. All rights reserved.
//

#import "AppPublic.h"
#import "Order.h"
#import "APAuthV2Info.h"
#import "RSADataSigner.h"
#import <AlipaySDK/AlipaySDK.h>

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
    //重要说明
    //这里只是为了方便直接向商户展示支付宝的整个支付流程；所以Demo中加签过程直接放在客户端完成；
    //真实App里，privateKey等数据严禁放在客户端，加签过程务必要放在服务端完成；
    //防止商户私密数据泄露，造成不必要的资金损失，及面临各种安全风险；
    /*============================================================================*/
    /*=======================需要填写商户app申请的===================================*/
    /*============================================================================*/
    NSString *appID = @"2017012505408813";
    
    // 如下私钥，rsa2PrivateKey 或者 rsaPrivateKey 只需要填入一个
    // 如果商户两个都设置了，优先使用 rsa2PrivateKey
    // rsa2PrivateKey 可以保证商户交易在更加安全的环境下进行，建议使用 rsa2PrivateKey
    // 获取 rsa2PrivateKey，建议使用支付宝提供的公私钥生成工具生成，
    // 工具地址：https://doc.open.alipay.com/docs/doc.htm?treeId=291&articleId=106097&docType=1
    NSString *rsa2PrivateKey = @"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQC//4SxiIPljmh1drHNX8f1dD3hoA+3+8iVmI6/IxnU/yqQd9lvJIyQtYcbiwaV9sA+KoExNUwljxhVXdRpKEd73ju+SOdpPsSzBmuYd0T0W6aKSF6s8DxkDUo585LscmCRjnumb4mNubMWu3IhvdoLCjgD6KzNvAE5SG2NwkxqBhclqVfv3zZ+2VmqfWqUfl/84VnX79GrCJFK4MApu0+x6T51xzPyRfypUOr9fxVreMkKiJrD8uD2peXTfNdskXWP7BBAAholawOI5xDnWbk4Gx3h6r4FqIP0uA01/T2NkSNYZvKS0MIm0GAXSqbVy7nr3DVQzZmbxVbQGncCfDrtAgMBAAECggEAAt77SPxVnJUokrkXp46LGuyGEF2oXoPxV+tUFx+bP0ghhs+D/Nl7EJ7mSk8l5hAuh79y9x1o6hbdskkY6AmmKAAGvK2yaCx8HIE4EquV6GGGeALlB4kAHi7ksUEnLUn4j7SlvLNmRyxDtU9QQZjpLckVVVmhW+I30nd2y+vf4KX3/6X4nFYlYHwmIpOXBFHevzLOdY9UvQCh86tZhXMJ/0RvY0X+GVaaAmD9vOGZNLJgGnu7lAMOBDv7sJjle5CyG+KwhRmhaeIBvIkDwWTKfsvfu4HkHULVYo6bee1mHOSadn0euKn95K22CqUSYWfuCbyjArVHzvUEbReHG750sQKBgQD2jlyafwyL7ASfwqO2KRkjy9DYpZevOYkrH5nS1byObNodlQ+8/zN68R28xbnuyM8R3LgNnTbq1KLby4hsgxq8wEeXGN2G6yyf0QnZs4yf94knTA4AgyT4ofyoYIlTczrAvML5ONZiihWVnM2A+QqHp7dWg1/G9JxvDzSac8X+ZwKBgQDHWi5ezA3HJLfUXG2p/n6OdZ85AMQ0Hpe0dR/0bwQfwMQ/QYgV8uOoH+fq2vKUy4X63313RnAy7DMf0oMGey4vxxMgZDrn/cqBWzHGxOY0hwEbUYURUrkwDcVPy9Hx3QYYqEnL9G/HZ4sPWUiimlKVGJq/oGwe3lLcwOjxpUp/iwKBgBjfBYJkPIzE0zaWeNNAVBPjVH72PVXktOyPtIGrI+kuG9aOGddT2wqU1g4b2AS5ZwIDROHCsS6BIqiPYISpuQG/V7lrPbWFdavtd1Ppo87jNoerSWJN2GXoWUdW7aNoZU3Z/MUNMbsg+Mpt1eEjvicvCPSGLhwwwrMFUqYd9aQTAoGBAKQpTyYhQvdK9VabBnOGbxFBKuz2BJwm1SZ1jOrswKilRPYSMM5iRbadD44G3txI/PuqLteZ2asVdsQtxlf/9/jAJq/12cxQ4gHD5ymX2zpGMlMcvkeVp4o6zND5NtnE+NA1RQ//Kk06pvjsvTOpgXfl2wkSQfbpfbLr6OeVBdQhAoGBALgYPf7W7iodZPEQEzDMq5K95tpGA2P6G5h4nPD4otG4glayZjd3Thlj2RNgoGyAticKKq7zz8FUjA8SBwLha9AAK5fkQWjF1MF0rdPnW76cw9m6vibeuX1utF+KNSjMoVCUMpXNmYRtHiORlMv/AhbXJ4fJE7hGafASBF+n16t0";
    NSString *rsaPrivateKey = @"";
    /*============================================================================*/
    /*============================================================================*/
    /*============================================================================*/
    
    //partner和seller获取失败,提示
    if ([appID length] == 0 ||
        ([rsa2PrivateKey length] == 0 && [rsaPrivateKey length] == 0))
    {
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示"
                                                        message:@"缺少appId或者私钥。"
                                                       delegate:self
                                              cancelButtonTitle:@"确定"
                                              otherButtonTitles:nil];
        [alert show];
        return;
    }
    
    /*
     *生成订单信息及签名
     */
    //将商品信息赋予AlixPayOrder的成员变量
    Order* order = [Order new];
    
    // NOTE: app_id设置
    order.app_id = appID;
    
    // NOTE: 支付接口名称
    order.method = @"alipay.trade.app.pay";
    
    // NOTE: 参数编码格式
    order.charset = @"utf-8";
    
    // NOTE: 当前时间点
    NSDateFormatter* formatter = [NSDateFormatter new];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    order.timestamp = [formatter stringFromDate:[NSDate date]];
    
    // NOTE: 支付版本
    order.version = @"1.0";
    
    // NOTE: sign_type 根据商户设置的私钥来决定
    order.sign_type = (rsa2PrivateKey.length > 1)?@"RSA2":@"RSA";
    
    // NOTE: 商品数据
    order.biz_content = [BizContent new];
    order.biz_content.body = @"我是测试数据";
    order.biz_content.subject = @"1";
    order.biz_content.out_trade_no = [self generateTradeNO]; //订单ID（由商家自行制定）
    order.biz_content.timeout_express = @"30m"; //超时时间设置
    order.biz_content.total_amount = [NSString stringWithFormat:@"%.2f", 0.01]; //商品价格
    
    //将商品信息拼接成字符串
    NSString *orderInfo = [order orderInfoEncoded:NO];
    NSString *orderInfoEncoded = [order orderInfoEncoded:YES];
    NSLog(@"orderSpec = %@",orderInfo);
    
    // NOTE: 获取私钥并将商户信息签名，外部商户的加签过程请务必放在服务端，防止公私钥数据泄露；
    //       需要遵循RSA签名规范，并将签名字符串base64编码和UrlEncode
    NSString *signedString = nil;
    RSADataSigner* signer = [[RSADataSigner alloc] initWithPrivateKey:((rsa2PrivateKey.length > 1)?rsa2PrivateKey:rsaPrivateKey)];
    if ((rsa2PrivateKey.length > 1)) {
        signedString = [signer signString:orderInfo withRSA2:YES];
    } else {
        signedString = [signer signString:orderInfo withRSA2:NO];
    }
    
    // NOTE: 如果加签成功，则继续执行支付
    if (signedString != nil) {
        //应用注册scheme,在AliSDKDemo-Info.plist定义URL types
        NSString *appScheme = @"alisdkdemo";

        // NOTE: 将签名成功字符串格式化为订单字符串,请严格按照该格式
        NSString *orderString = [NSString stringWithFormat:@"%@&sign=%@",
                                 orderInfoEncoded, signedString];
        
        // NOTE: 调用支付结果开始支付
        [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
            NSLog(@"reslut = %@",resultDic);
        }];
    }
    
//    _input_charset=\"utf-8\"&notify_url=\"http://115.28.109.174:8181/game/alipay/notify\"&out_trade_no=\"20170217180127652\"&partner=\"2088521530118846\"&payment_type=\"1\"&seller_id=\"779159405@qq.com\"&service=\"mobile.securitypay.pay\"&subject=\"充值\"&total_fee=\"10\"&sign=\"gqhJTeZDA9I%2BQhjqzTQycj3fiPEQQKLM9%2BIoibROiDZpq%2FvX86brbZNkyrm8xLLS6tvHEtXDqp1pID0EbE178oXAtX9gbLdEyJfT%2FVwzEbx2%2BlyRPSleVLMKY2chImTXWC0KPuAJQK2dEvy%2BtcbQiju68LrxMsnh3vYW1ORLiAfH%2FxWWIe%2FTEVPbWpOXoiGjFftyy4uClfBf8LEriYIHMMHI6V8PM1JoJrMZH6R8PZRZ5RdrXNRtOn1PWQC2gVYja1nk6xEi%2Fh6cfRiSkZ1sy3RKin7xtkg4zieRv%2FQzwbOVG6mQ2ksgTcMKy1%2BqW%2FEAb2Pv%2BZ4sd1fTzM0i5jjkPw%3D%3D\"&sign_type=\"RSA\"
    
//    app_id=2017012505408813&biz_content=%7B%22timeout_express%22%3A%2230m%22%2C%22seller_id%22%3A%22%22%2C%22product_code%22%3A%22QUICK_MSECURITY_PAY%22%2C%22total_amount%22%3A%220.01%22%2C%22subject%22%3A%221%22%2C%22body%22%3A%22%E6%88%91%E6%98%AF%E6%B5%8B%E8%AF%95%E6%95%B0%E6%8D%AE%22%2C%22out_trade_no%22%3A%22Q9GG3S6NKUKIIQO%22%7D&charset=utf-8&method=alipay.trade.app.pay&sign_type=RSA2&timestamp=2017-02-17%2018%3A08%3A38&version=1.0&sign=NE1yNk5xDWZeswghjNThdKb%2FYIGhLdDKjs64s7V7%2FD7RYdqxYTooy2TvD1xx%2BL09P6jUul7C1XDMCq0II06%2FlqvoeGLDRgUHGelU9NrnQkBgRgPdFeunzrGYtvtLFdk6OPYWJSw6uYi%2FOG5mQjZXSVgxPwjHAk%2BHhZsBzEZHCbLW%2F0NvHR2vELv8fi5r4a0kaIOlJiBnyzpHgfV%2BM5xXYtGnjClJQHmq%2BKGSz5jPsTjD3tXz5uamsqVsY97L3hEscY3sPVf6PCUTIcQQbrzLedWSl1pVxpOrJVga0DfiN9r1ETURd9NNQDHQCy1J8uRTrKF4FogX6dpx0W4KWn6uUA%3D%3D
    
    //应用注册scheme,在AliSDKDemo-Info.plist定义URL types
    NSString *appScheme = @"fulushouNineHalf";
    // NOTE: 调用支付结果开始支付
    [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
        NSLog(@"reslut = %@",resultDic);
    }];
}


@end
