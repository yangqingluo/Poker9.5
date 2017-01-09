//
//  CppToOCFunction.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef CppToOCFunction_h
#define CppToOCFunction_h

#include "cocos2d.h"
USING_NS_CC;

class CppToOCFunction : public Ref{
public:
    static CppToOCFunction* getInstance();
    
    void doCopyAction(const char* m_string);
private:
    
protected:
    ~CppToOCFunction();
};

#endif /* CppToOCFunction_h */
