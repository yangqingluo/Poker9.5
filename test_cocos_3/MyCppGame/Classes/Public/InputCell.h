#ifndef __InputCell__
#define __InputCell__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class InputCell : public TableViewCell, TextFieldDelegate, EditBoxDelegate
{
public:
    InputCell();
    ~InputCell();
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    //开启触控
    virtual bool onTouchBegan(Touch* touch, Event* event);
    virtual void onTouchMoved(Touch* touch, Event* event);
    virtual void onTouchEnded(Touch* touch, Event* event);
    CREATE_FUNC(InputCell);
    
    Label* titleLabel;
    TextFieldTTF* textField;
    EditBox* inputBox;
    
    static Size tableCellSizeForIndex(TableView* table, ssize_t idx);
private:
    Sprite* bgSprite;
    
    virtual bool onTextFieldAttachWithIME(TextFieldTTF* sender); //当用户启动虚拟键盘的时候的回调函数
    virtual bool onTextFieldDetachWithIME(TextFieldTTF* sender); //当用户关闭虚拟键盘的时候的回调函数
    virtual bool onTextFieldInsertText(TextFieldTTF* sender, const char* text, int nLen); //当用户输入的时候的回调函数
    virtual bool onTextFieldDeleteBackward(TextFieldTTF* sender, const char* delText, int nLen); //当用户删除文字的时候的回调函数
    
    virtual void editBoxEditingDidBegin(EditBox* editBox); //开始编辑
    virtual void editBoxEditingDidEnd(EditBox* editBox); //结束编辑
    virtual void editBoxTextChanged(EditBox* editBox, const std::string& text); //编辑框文字改变
    virtual void editBoxReturn(EditBox* editBox); //触发return后的回调函数
};

#endif /* defined(__InputCell__) */
