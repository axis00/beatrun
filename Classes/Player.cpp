//
// Created by POTATO on 5/27/2017.
//

#include "Player.h"
#include "GameManager.h"
#include "Definitions.h"

USING_NS_CC;

Player::Player(){
    this->size = Size(DEF_PLAYER_SIZE_X,DEF_PLAYER_SIZE_Y);
    vel_x = DEF_PLAYER_VEL_X;
    vel_y = DEF_PLAYER_VEL_Y;
    m_CameraMask = (unsigned short)CameraFlag::USER1;
    m_InContact = false;
}

Player* Player::createWithPhysics() {

    Player* player = new Player();
    if(player && player->initWithFile("player_def.png")){
        player->autorelease();
        player->setSize(player->size);

        player->m_PhysicsBody = PhysicsBody::createBox(player->getContentSize(),PhysicsMaterial(0,0,0));
        player->m_PhysicsBody->setDynamic(true);
        player->m_PhysicsBody->setCollisionBitmask(CollisionMask::playerMask);
        player->m_PhysicsBody->setContactTestBitmask(true);

        player->setPhysicsBody(player->m_PhysicsBody);
        player->setCameraMask(player->m_CameraMask,true);
        return player;
    }

    CC_SAFE_DELETE(player);
    return nullptr;

}

/*
 * Scales the player sprite in terms of pixels based on the design resolution
 */
void Player::setSize(const Size &s){
    size = s;
    SCALE_NODE(this,s);
}

void Player::update(float dt) {
}

bool Player::isInContact(){
    return m_InContact;
}
void Player::setInContact(bool c){
    m_InContact = c;
}

void Player::reset(){
    vel_x = DEF_PLAYER_VEL_X;
    vel_y = DEF_PLAYER_VEL_Y;
    m_CameraMask = (unsigned short)CameraFlag::USER1;
    m_InContact = false;
    _physicsBody->setVelocity(Vec2(0,0));
    _physicsBody->setAngularVelocity(DEF_PLAYER_ANG_VEL);
    setRotation(0);
}

void Player::drop(){
    getPhysicsBody()->setVelocity(Vec2(vel_x,PLAYER_DROP_VEL));
    getPhysicsBody()->setAngularVelocity(0);
}

/**
 * platform : platform to slide on
 */
void Player::slide(cocos2d::Node* platform){
    setInContact(true);

    getPhysicsBody()->setVelocity(Vec2(SLIDE_PLAYER_VEL_X,0));
    float pos_Y = platform->getBoundingBox().size.height/2 + platform->getPosition().y +
                  getBoundingBox().size.height/2;
    float pos_X = getPhysicsBody()->getPosition().x;
    getPhysicsBody()->setPosition(pos_X,pos_Y);
    getPhysicsBody()->setAngularVelocity(0);
    float rotation = (float)((int)getRotation() % 360);
    rotation = round((rotation / 360.0f) * 4) * 90;
    getPhysicsBody()->setRotation(rotation);
}


/**
* animates the player into a 'normal' rotation (flat side facing down)
*/
void Player::normalizeRotation(){
    float rotation = (float)((int)getRotation() % 360);
    rotation = std::ceil((rotation / 360.0f) * 4) * 90;
    auto rotAct = RotateTo::create(.07,rotation);
    runAction(rotAct);
}