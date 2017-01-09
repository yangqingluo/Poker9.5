//
//  CppToFunction.h
//  MyCppGame
//
//  Created by yangqingluo on 2016/11/20.
//
//

#ifndef CppToFunction_h
#define CppToFunction_h

#include "cocos2d.h"
USING_NS_CC;

class CppToFunction : public Ref{
public:
    static CppToFunction* getInstance();
    
    void doCopyAction(const char* m_string);
private:
    
protected:
    ~CppToFunction();
};

#endif /* CppToFunction_h */
