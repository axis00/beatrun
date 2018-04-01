//
// Created by POTATO on 6/12/2017.
//

#include "Hazard.h"
#include "GameManager.h"
#include "MyBodyParser.h"
#include "Definitions.h"

USING_NS_CC;

Hazard::Hazard(){
    m_height = 66;
    m_width = 75;

    m_PhysicsCollisionMask = CollisionMask::hazardMask;

    m_AngularVel = 5;

    m_CameraMask = (unsigned short)CameraFlag::USER1;
}

Hazard* Hazard::createWithPhysics(int h){
    Hazard* haz = new Hazard();

    char filePath[30] = "";
    char physicsObj[30] = "";

    switch (h){
        case HazardType::tri:
            sprintf(filePath,"haz_di.png");
            sprintf(physicsObj,"haz_di");
            break;
    }

    if(haz && haz->initWithFile(filePath)){
        haz->autorelease();
        SCALE_NODE(haz,Size(haz->m_width,haz->m_height));
        haz->m_PhysicsBody = MyBodyParser::getInstance()->bodyFormJson(haz,physicsObj,PhysicsMaterial(0,0,0));
        haz->m_PhysicsBody->setDynamic(false);
        haz->m_PhysicsBody->setCollisionBitmask(haz->m_PhysicsCollisionMask);
        haz->m_PhysicsBody->setContactTestBitmask(true);
        haz->m_PhysicsBody->setAngularVelocity(haz->m_AngularVel);
        haz->setPhysicsBody(haz->m_PhysicsBody);
        haz->setCameraMask(haz->m_CameraMask,true);
        return haz;
    }
    CC_SAFE_DELETE(haz);
    return nullptr;

}

void Hazard::update(float dt){

}

void Hazard::setAngularVel(float vel){
    this->m_AngularVel = vel;
    _physicsBody->setAngularVelocity(vel);
}
float Hazard::getAngularVel(){
    return _physicsBody->getAngularVelocity();
}


