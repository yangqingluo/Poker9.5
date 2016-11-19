#include "HelloWorld2Scene.h"

USING_NS_CC;

Scene* HelloWorld2::scene()
{
    Scene *scene = Scene::create();
    HelloWorld2 *layer = HelloWorld2::create();
    scene->addChild(layer);
    return scene;
}


bool HelloWorld2::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

//	test1(); //����ͼƬ����
	test2(); //����ֻ���������
	//test3(); //���Ա�����ҳ

	return true;
}


//����ͼƬ����
void HelloWorld2::test1()
{
	Size visableSize = Size(480, 320); //��Ļ��С
	Size mysize = Size(960,600); //������С

	
//�������������ô�С
	LayerColor* scrollLayer = LayerColor::create( Color4B(255,255,255,255) );
	scrollLayer->setContentSize(mysize);
	
//�����еĶ���
	Sprite* bg = Sprite::create("war.png");
	bg->setPosition( Size(960/2.0, 600/2.0) );
	scrollLayer->addChild(bg);


//����������ͼScrollView
	ScrollView* scrollView = ScrollView::create();
	this->addChild(scrollView, 0, 1);
	
	//��������
	scrollView->setContainer(scrollLayer); //��������
	scrollView->setViewSize( visableSize ); //���������С
	scrollView->setBounceable(true); //�Ƿ���е���
	
	//ί�д���
	scrollView->setDelegate(this);
}


//����ֻ���������
void HelloWorld2::test2()
{
	Size visableSize = Size(480, 320); //��Ļ��С
	Size mysize = Size(150,220); //������С

//�������������ô�С
	LayerColor* scrollLayer = LayerColor::create( Color4B(255,255,255,255) );
	scrollLayer->setContentSize(mysize);

//����������ĸ���ť
	for(int i = 1; i <= 4; i++) 
	{
		char file[20];
		sprintf(file, "btn%d.png", i);
		Sprite* btn = Sprite::create(file);
		btn->setPosition( Size(mysize.width/2, 220 - 50*i) );
		scrollLayer->addChild(btn);
	}

//����������ͼScrollView
	//���������С150*100 �� ����ΪscrollLayer
	ScrollView* scrollView = ScrollView::create(Size(150, 100), scrollLayer);
	scrollView->setPosition( visableSize/2 - Size(150/2.0, 100/2.0) );
	this->addChild(scrollView, 0, 2);
	
	//����Ϊֻ���������
    scrollView->setDirection(ScrollView::Direction::VERTICAL);

	//ί�д���
	scrollView->setDelegate(this);

}

//���Ա�����ҳ
void HelloWorld2::test3()
{
	Size visableSize = Size(480, 320); //��Ļ��С
	Size mysize = Size(300,80); //������С

//�������������ô�С
	LayerColor* scrollLayer = LayerColor::create( Color4B(255,255,255,255) );
	scrollLayer->setContentSize(mysize);

//�������������ͼƬ
	for(int i = 1; i <= 3; i++) 
	{
		char file[20];
		sprintf(file, "sp%d.png", i);
		Sprite* sp = Sprite::create(file);
		sp->setPosition( Size(100*i - 50, 40) );
		scrollLayer->addChild(sp);
	}

//����������ͼScrollView
	//���������С100*80 �� ����ΪscrollLayer
	ScrollView* scrollView = ScrollView::create(Size(100, 80), scrollLayer);
	scrollView->setPosition( visableSize/2 - Size(100/2.0, 0) );
	this->addChild(scrollView, 0, 3);

	//��������
	scrollView->setTouchEnabled(false); //�رմ����¼����޷���������

	//ί�д���
	scrollView->setDelegate(this);


//����������ҳ��ť
	//ǰ��pBack����pFront
	pBack = MenuItemImage::create("b1.png", "b2.png", "b3.png", this, menu_selector(HelloWorld2::scrollImage) );
	pFront = MenuItemImage::create("f1.png", "f2.png", "f3.png", this, menu_selector(HelloWorld2::scrollImage) );
	pBack->setPosition( Size(visableSize.width/2 - 100, 60) );
	pFront->setPosition( Size(visableSize.width/2 + 100, 60) );
	Menu* pMenu = Menu::create(pBack, pFront, NULL);
    pMenu->setPosition(Point::ZERO);
	this->addChild(pMenu, 0, 100);

	pBack->setEnabled(false);
	pageNumber = 0; //��0ҳ

}

//ʵ�ַ�ҳЧ��
void HelloWorld2::scrollImage(Object* sender)
{
	ScrollView* scrollView = (ScrollView*)this->getChildByTag(3);

	if(sender == pBack && pBack->isEnabled() )
	{
		pageNumber = fmax(0, pageNumber-1); //ǰ��
	}
	else if( pFront->isEnabled() )
	{
		pageNumber = fmin(2, pageNumber+1); //��
	}

	//����������Թ�����ͼ��ƫ����
	scrollView->setContentOffset(Size(-100 * pageNumber, 0), true);

	pBack->setEnabled( pageNumber != 0);
	pFront->setEnabled( pageNumber != 2);
}


void HelloWorld2::scrollViewDidScroll(ScrollView* view) 
{
	CCLOG("ScrollView Moved!");
}
void HelloWorld2::scrollViewDidZoom(ScrollView* view) 
{
	CCLOG("ScrollView Scaled");
}


//�رճ���
void HelloWorld2::menuCloseCallback(Object* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
