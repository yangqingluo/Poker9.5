#ifndef __RegistInputView__
#define __RegistInputView__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class RegistInputView : public LayerColor, EditBoxDelegate
{
public:
    RegistInputView();
    ~RegistInputView();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    CREATE_FUNC(RegistInputView);
    
    
private:
    Sprite* bgSprite;
    
    virtual void editBoxEditingDidBegin(EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text); //编辑框文字改变
    virtual void editBoxReturn(EditBox* editBox); //触发return后的回调函数
};

#endif /* defined(__RegistInputView__) */
