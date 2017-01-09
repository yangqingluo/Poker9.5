//
//  CppToOCFunction.cpp
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "OCFunction.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#endif

#include "CppToOCFunction.h"

static CppToOCFunction* share = nullptr;

CppToOCFunction::~CppToOCFunction(void){
    
}

CppToOCFunction* CppToOCFunction::getInstance(){
    if(!share){
        share = new (std::nothrow) CppToOCFunction();
        
    }
    
    return share;
}


#pragma public
void CppToOCFunction::doCopyAction(const char* m_string){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS代码
    nsCopy(m_string);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    //Android代码
    
#endif
}
