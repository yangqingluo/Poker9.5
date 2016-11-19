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

//	test1(); //测试图片滚动
	test2(); //测试只能纵向滚动
	//test3(); //测试背包翻页

	return true;
}


//测试图片滚动
void HelloWorld2::test1()
{
	Size visableSize = Size(480, 320); //屏幕大小
	Size mysize = Size(960,600); //容器大小

	
//创建容器、设置大小
	LayerColor* scrollLayer = LayerColor::create( Color4B(255,255,255,255) );
	scrollLayer->setContentSize(mysize);
	
//容器中的东西
	Sprite* bg = Sprite::create("war.png");
	bg->setPosition( Size(960/2.0, 600/2.0) );
	scrollLayer->addChild(bg);


//创建滚动视图ScrollView
	ScrollView* scrollView = ScrollView::create();
	this->addChild(scrollView, 0, 1);
	
	//属性设置
	scrollView->setContainer(scrollLayer); //设置容器
	scrollView->setViewSize( visableSize ); //可视区域大小
	scrollView->setBounceable(true); //是否具有弹性
	
	//委托代理
	scrollView->setDelegate(this);
}


//测试只能纵向滚动
void HelloWorld2::test2()
{
	Size visableSize = Size(480, 320); //屏幕大小
	Size mysize = Size(150,220); //容器大小

//创建容器、设置大小
	LayerColor* scrollLayer = LayerColor::create( Color4B(255,255,255,255) );
	scrollLayer->setContentSize(mysize);

//容器中添加四个按钮
	for(int i = 1; i <= 4; i++) 
	{
		char file[20];
		sprintf(file, "btn%d.png", i);
		Sprite* btn = Sprite::create(file);
		btn->setPosition( Size(mysize.width/2, 220 - 50*i) );
		scrollLayer->addChild(btn);
	}

//创建滚动视图ScrollView
	//可视区域大小150*100 、 容器为scrollLayer
	ScrollView* scrollView = ScrollView::create(Size(150, 100), scrollLayer);
	scrollView->setPosition( visableSize/2 - Size(150/2.0, 100/2.0) );
	this->addChild(scrollView, 0, 2);
	
	//设置为只能纵向滚动
    scrollView->setDirection(ScrollView::Direction::VERTICAL);

	//委托代理
	scrollView->setDelegate(this);

}

//测试背包翻页
void HelloWorld2::test3()
{
	Size visableSize = Size(480, 320); //屏幕大小
	Size mysize = Size(300,80); //容器大小

//创建容器、设置大小
	LayerColor* scrollLayer = LayerColor::create( Color4B(255,255,255,255) );
	scrollLayer->setContentSize(mysize);

//容器中添加三个图片
	for(int i = 1; i <= 3; i++) 
	{
		char file[20];
		sprintf(file, "sp%d.png", i);
		Sprite* sp = Sprite::create(file);
		sp->setPosition( Size(100*i - 50, 40) );
		scrollLayer->addChild(sp);
	}

//创建滚动视图ScrollView
	//可视区域大小100*80 、 容器为scrollLayer
	ScrollView* scrollView = ScrollView::create(Size(100, 80), scrollLayer);
	scrollView->setPosition( visableSize/2 - Size(100/2.0, 0) );
	this->addChild(scrollView, 0, 3);

	//属性设置
	scrollView->setTouchEnabled(false); //关闭触碰事件，无法触摸滚动

	//委托代理
	scrollView->setDelegate(this);


//创建背包翻页按钮
	//前翻pBack、后翻pFront
	pBack = MenuItemImage::create("b1.png", "b2.png", "b3.png", this, menu_selector(HelloWorld2::scrollImage) );
	pFront = MenuItemImage::create("f1.png", "f2.png", "f3.png", this, menu_selector(HelloWorld2::scrollImage) );
	pBack->setPosition( Size(visableSize.width/2 - 100, 60) );
	pFront->setPosition( Size(visableSize.width/2 + 100, 60) );
	Menu* pMenu = Menu::create(pBack, pFront, NULL);
    pMenu->setPosition(Point::ZERO);
	this->addChild(pMenu, 0, 100);

	pBack->setEnabled(false);
	pageNumber = 0; //第0页

}

//实现翻页效果
void HelloWorld2::scrollImage(Object* sender)
{
	ScrollView* scrollView = (ScrollView*)this->getChildByTag(3);

	if(sender == pBack && pBack->isEnabled() )
	{
		pageNumber = fmax(0, pageNumber-1); //前翻
	}
	else if( pFront->isEnabled() )
	{
		pageNumber = fmin(2, pageNumber+1); //后翻
	}

	//设置容器相对滚动视图的偏移量
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


//关闭程序
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
