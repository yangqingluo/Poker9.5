#include "InputCell.h"

InputCell::InputCell(){
    
}

InputCell::~InputCell(){
    
}

bool InputCell::init(){
    if ( !TableViewCell::init() )
    {
        return false;
    }
    
//    titleLabel = Label::createWithTTF("", "fonts/STKaiti.ttf", 12);
//    titleLabel->setTextColor(Color4B::BLACK);
//    titleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
//    titleLabel->setVerticalAlignment(TextVAlignment::CENTER);
//    this->addChild(titleLabel, 0 , 1);
    
    textField = TextFieldTTF::textFieldWithPlaceHolder("", "fonts/STKaiti.ttf", 12);
    textField->setHorizontalAlignment(TextHAlignment::LEFT);
    textField->setVerticalAlignment(TextVAlignment::CENTER);
    this->addChild(textField, 0, 2);
    textField->setDelegate(this);
    
    return true;
}

void InputCell::onEnter(){
    TableViewCell::onEnter();
    
//    titleLabel->setPosition(0.15 * this->getContentSize().width, 0.5 * this->getContentSize().height);
//    titleLabel->setDimensions(0.3 * this->getContentSize().width, this->getContentSize().height);
    
//    textField->setContentSize(Size(0.5 * this->getContentSize().width, 0.8 * this->getContentSize().height));
    textField->setPosition(0.5 * this->getContentSize().width, 0.5 * this->getContentSize().height);
    textField->setDimensions(0.5 * this->getContentSize().width, 0.8 * this->getContentSize().height);
}
void InputCell::onExit(){
    TableViewCell::onExit();
    
}

#pragma textField
//当用户启动虚拟键盘的时候的回调函数
bool InputCell::onTextFieldAttachWithIME(TextFieldTTF* sender)
{
    //事件处理
//    sender->setFontSize(30); //字体放大为30
//    sender->setColor(ccYELLOW); //内容颜色: 黄色
//    sender->setColorSpaceHolder(ccWHITE); //默认内容颜色: 白色
    
    return false; //启用键盘。若不启用键盘return true;
}

//当用户关闭虚拟键盘的时候的回调函数
bool InputCell::onTextFieldDetachWithIME(TextFieldTTF* sender)
{
    //事件处理
//    sender->setFontSize(24); //字体大小还原为24
//    sender->setColor(ccORANGE); //内容颜色: 橘黄
//    sender->setColorSpaceHolder(ccGRAY); //默认内容颜色: 灰色
    
    return false; //关闭键盘。若不关闭键盘return true;
}

//当用户输入的时候的回调函数
bool InputCell::onTextFieldInsertText(TextFieldTTF* sender, const char* text, int nLen)
{
    //事件处理
    CCLOG("CharCount: %d", sender->getCharCount());
    
    return false; //输入字符。若不允许输入字符return true;
}

//当用户删除文字的时候的回调函数
bool InputCell::onTextFieldDeleteBackward(TextFieldTTF* sender, const char* delText, int nLen)
{
    return false; //删除字符。若不允许删除字符return true;
}

#pragma public
Size InputCell::tableCellSizeForIndex(TableView* table, ssize_t idx){
    return Size(table->getContentSize().width, 30);
}