#ifndef __HelloWorld2_SCENE_H__
#define __HelloWorld2_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class HelloWorld2 : public Layer,public ScrollViewDelegate
{
public:
	virtual bool init();  
	static Scene* scene();
	void menuCloseCallback(Object* pSender);
	CREATE_FUNC(HelloWorld2);

	void test1(); //测试图片滚动
	void test2(); //测试只能纵向滚动
	void test3(); //测试背包翻页

	int pageNumber; //背包第几页
	MenuItemImage* pBack; //往前翻页
	MenuItemImage* pFront; //往后翻页
	void scrollImage(Object* sender); //test3的背包翻页

	void scrollViewDidScroll(ScrollView* view); //滚动时响应的回调函数
	void scrollViewDidZoom(ScrollView* view); //放缩时响应的回调函数
};

#endif // __HelloWorld2_SCENE_H__
