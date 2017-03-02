//
//  PopAlertDialog.h
//  HelloWorldDemo
//
//  Created by yangqingluo on 16/11/9.
//
//

#ifndef __HelloWorldDemo__PopAlertDialog__
#define __HelloWorldDemo__PopAlertDialog__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class PopAlertDialog:public LayerColor{  //继承LayerColor类，方便更改layer的颜色和透明度
public:
    PopAlertDialog();
    ~PopAlertDialog();
    virtual bool init();
    CREATE_FUNC(PopAlertDialog);
    
    static PopAlertDialog* create(const char* backgroudImage,Size dialogSize);
    bool onTouchBegan(Touch* touch,Event* event);
    void onTouchMoved(Touch* touch,Event* event);
    void onTouchEnded(Touch* touch,Event* event);
    void setTitle(const char* title,int fontsize=20);
    void setContentTextShowed(const char* text);
    void setContentText(const char* text,int fontsize=20,int padding=50,int paddingTop=50);
    void setCallBackFunc(Ref* target,SEL_CallFuncN callfun);
    bool addButton(const char* normalImage,const char* selectedImage,const char* title,int tag=0);
    
    virtual void onEnter();
    virtual void onExit();
    void backgroundFinish();
    
    Size m_dialogContentSize; //对话框大小
    bool m_touchCancel;//触摸对话框以外区域是否消失
private:
    void buttonCallBack(Ref* pSender);
    int m_contentPadding;
    int m_contentPaddingTop;
    
    Ref* m_callbackListener;
    SEL_CallFuncN m_callback;
    
    CC_SYNTHESIZE_RETAIN(Menu*, m__pMenu, MenuButton);
    CC_SYNTHESIZE_RETAIN(Sprite*, m__sfBackGround, SpriteBackGround);
    CC_SYNTHESIZE_RETAIN(Sprite*, m__s9BackGround, Sprite9BackGround);
    CC_SYNTHESIZE_RETAIN(Label*, m__ltTitle, LabelTitle);
    CC_SYNTHESIZE_RETAIN(Label*, m__ltContentText, LabelContentText);
};

#endif /* defined(__HelloWorldDemo__PopAlertDialog__) */
