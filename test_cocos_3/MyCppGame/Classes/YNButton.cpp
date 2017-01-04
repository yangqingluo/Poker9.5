#include "YNButton.h"

YNButton::YNButton(){
    
}
YNButton::~YNButton(){
    
}


bool YNButton::init(){
    if (!Button::init()){
        return false;
    }
    
    
    
    
    return true;
}

void YNButton::configImage(const std::string &normalImageName, const std::string &highlightedImageName, const std::string &selectedImageName){
    m_normalImageName = normalImageName;
    m_highlightedImageName = highlightedImageName;
    m_selectedImageName = selectedImageName;
    
    showImage();
}

void YNButton::setSelected(bool value){
    if (m_selected != value) {
        m_selected = value;
        showImage();
    }
}

bool YNButton::isSelected(){
    return m_selected;
}

void YNButton::showImage(){
    if (strlen(m_highlightedImageName.c_str()) == 0) {
        this->loadTextureNormal(isSelected() ? m_selectedImageName : m_normalImageName);
    }
    else {
        this->loadTextures(isSelected() ? m_selectedImageName : m_normalImageName, m_highlightedImageName);
    }
}
