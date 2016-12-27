#include "Player.h"

Player::Player():m_isCall(false),m_isDiZhu(false)
{
    
}

Player::~Player()
{
    
}


void Player::infoConfig(const char* nick, const char* head, int jetton){
    strcpy(nickName, nick);
    strcpy(headImage, head);
    this->setJettonCount(jetton);
    m_jettonInitial = jetton;
}
