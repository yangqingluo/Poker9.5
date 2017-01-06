//
//  OCPublic.m
//  MyCppGame
//
//  Created by yangqingluo on 2017/1/6.
//
//

#import "OCPublic.h"

__strong static OCPublic  *_singleManger = nil;

@interface OCPublic ()

@end

@implementation OCPublic

+ (OCPublic *)getInstance{
    static dispatch_once_t pred = 0;
    dispatch_once(&pred, ^{
        _singleManger = [[OCPublic alloc] init];
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
- (void)doCopyString:(NSString *)string{
    [UIPasteboard generalPasteboard].string = string;
    
    
}


@end
