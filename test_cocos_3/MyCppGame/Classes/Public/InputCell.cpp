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
    
    return true;
}

