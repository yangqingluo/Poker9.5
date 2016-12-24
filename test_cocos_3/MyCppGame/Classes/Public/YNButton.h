#ifndef _YNButton_H_
#define _YNButton_H_

#include "cocos2d.h"
USING_NS_CC;
using namespace ui;

#include "cocos-ext.h"
USING_NS_CC_EXT;

class YNButton: public Button{
public:
    YNButton();
    ~YNButton();
    virtual bool init();
    CREATE_FUNC(YNButton);
    
    void configImage(const std::string &normalImageName, const std::string &highlightedImageName, const std::string &selectedImageName);
    
    void setSelected(bool value);
    bool isSelected();
private:
    bool m_selected;
    void showImage();
    std::string m_normalImageName;
    std::string m_highlightedImageName;
    std::string m_selectedImageName;
};


#endif
