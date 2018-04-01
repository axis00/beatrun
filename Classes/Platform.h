//
// Created by POTATO on 5/31/2017.
//

#ifndef __BASICPLATFORM_H
#define __BASICPLATFORM_H

#include <2d/CCSprite.h>

enum PlatformType{
    triangle = 1,
    flat = 2,
    flat_short = 3,
    flat_long = 4
};

class Platform : public cocos2d::Sprite{

public:
    Platform();

    static Platform* createWithPhysics(int t);

    void update(float dt);

    void setContacts(int c);
    int getContacts(){return m_Contacts;}

private:
    cocos2d::PhysicsBody* m_PhysicsBody;

    float m_height,m_width;
    int m_PhysicsCollisionMask;
    int m_Contacts;

    unsigned short m_CameraMask;

};


#endif //__BASICPLATFORM_H