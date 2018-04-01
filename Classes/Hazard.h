//
// Created by POTATO on 6/12/2017.
//

#ifndef PROJ_ANDROID_STUDIO_HAZARD_H
#define PROJ_ANDROID_STUDIO_HAZARD_H


#include <2d/CCSprite.h>

enum HazardType{
    tri = 10
};

class Hazard : public cocos2d::Sprite{
public:
    Hazard();
    static Hazard* createWithPhysics(int h);
    void update(float dt);
    void setAngularVel(float vel);
    float getAngularVel();
private:
    cocos2d::PhysicsBody* m_PhysicsBody;
    float m_height,m_width;
    int m_PhysicsCollisionMask;
    float m_AngularVel;

    unsigned short m_CameraMask;

};


#endif //PROJ_ANDROID_STUDIO_HAZARD_H
