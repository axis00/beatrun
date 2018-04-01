//
// Created by POTATO on 5/31/2017.
//

#include "Platform.h"
#include "Definitions.h"
#include "GameManager.h"
#include "MyBodyParser.h"

USING_NS_CC;

Platform::Platform(){
    m_width = 100;
    m_height = 115;
    m_PhysicsCollisionMask = CollisionMask::basicPlatformMask;
    m_CameraMask = (unsigned short)CameraFlag::USER1;
}

Platform* Platform::createWithPhysics(int t){

    Platform* plat = new Platform();

    char filePath[30] = "";
    char physicsObj[30] = "";

    switch (t){
        case PlatformType::triangle:
            sprintf(filePath,"platform_basic.png");
            sprintf(physicsObj,"plat_triangle");
            break;
        case PlatformType::flat:
            sprintf(filePath,"platform_flat.png");
            sprintf(physicsObj,"plat_flat");
            plat->m_width = 400;//def value 150
            plat->m_height = 45;
            break;
        case PlatformType ::flat_short:
            sprintf(filePath,"platform_flat_short.png");
            sprintf(physicsObj,"plat_flat");
            plat->m_width = 150;//def value 150
            plat->m_height = 45;
            break;
    }
    if(plat && plat->initWithFile(filePath)){
        plat->autorelease();
        SCALE_NODE(plat,Size(plat->m_width,plat->m_height));
        plat->m_PhysicsBody = MyBodyParser::getInstance()->bodyFormJson(plat,physicsObj,PhysicsMaterial(0,0,0));
        plat->m_PhysicsBody->setDynamic(false);
        plat->m_PhysicsBody->setCollisionBitmask(plat->m_PhysicsCollisionMask);
        plat->m_PhysicsBody->setContactTestBitmask(true);
        plat->setPhysicsBody(plat->m_PhysicsBody);
        plat->setCameraMask(plat->m_CameraMask,true);
        return plat;
    }

    CC_SAFE_DELETE(plat);
    return nullptr;

}

void Platform::update(float dt) {

}
