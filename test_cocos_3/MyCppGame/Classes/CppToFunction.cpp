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

#endif

#include "CppToFunction.h"

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
    
#endif
}

void CppToFunction::doAlipayAction(char* m_string){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS代码
    nsDoAlipay(m_string);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //Android代码
    
#endif
}
