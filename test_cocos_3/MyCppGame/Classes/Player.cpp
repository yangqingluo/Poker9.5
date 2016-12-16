#include "Player.h"

Player::Player():m_isCall(false),m_isDiZhu(false)
{
    
}

Player::~Player()
{
    
}


void Player::infoConfig(const char* nick, const char* head, int jetton){
    sprintf(nickName, "%s",nick);
    sprintf(headImage, "%s", head);
    this->setJettonCount(jetton);
    m_jettonInitial = jetton;
}
