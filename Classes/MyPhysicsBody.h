//
// Created by POTATO on 6/1/2017.
//

#ifndef __MYPHYSICSBODY_H
#define __MYPHYSICSBODY_H


#include <cocos/physics/CCPhysicsBody.h>

class MyPhysicsBody: public cocos2d::PhysicsBody {

public:
    virtual void setPosition(float positionX, float positionY);

    virtual void setRotation(float rotation);

    virtual void setScale(float scaleX, float scaleY);

};


#endif //__MYPHYSICSBODY_H
