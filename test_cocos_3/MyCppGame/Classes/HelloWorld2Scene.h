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

	void test1(); //����ͼƬ����
	void test2(); //����ֻ���������
	void test3(); //���Ա�����ҳ

	int pageNumber; //�����ڼ�ҳ
	MenuItemImage* pBack; //��ǰ��ҳ
	MenuItemImage* pFront; //����ҳ
	void scrollImage(Object* sender); //test3�ı�����ҳ

	void scrollViewDidScroll(ScrollView* view); //����ʱ��Ӧ�Ļص�����
	void scrollViewDidZoom(ScrollView* view); //����ʱ��Ӧ�Ļص�����
};

#endif // __HelloWorld2_SCENE_H__
