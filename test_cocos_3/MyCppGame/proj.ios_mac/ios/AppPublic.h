//
//  AppPublic.h
//  SafetyOfMAS
//
//  Created by yangqingluo on 16/9/9.
//  Copyright © 2016年 yangqingluo. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface AppPublic : NSObject


+ (AppPublic *)shareInstance;

/*!
 @brief 检查版本是否第一次使用
 */
BOOL isFirstUsing();

- (void)doAlipayPay:(NSString *)orderString;
- (void)alipayResult:(NSDictionary *)resultDic;

- (void)doWechatPay:(NSString *)orderString;

@end
