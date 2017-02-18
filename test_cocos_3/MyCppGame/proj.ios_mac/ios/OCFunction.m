//
//  OCFunction.m
//  LocationFingerPrint
//
//  Created by yangqingluo on 16/6/16.
//  Copyright © 2016年 yangqingluo. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AppPublic.h"

extern void callbackAliPay(int code);

void nsCopy(const char *string){
    [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:string];
}

void nsDoAlipay(const char *string){
    [[AppPublic shareInstance] doAlipayPay:[NSString stringWithUTF8String:string]];
}

void appCallback(int code){
    callbackAliPay(code);
}
