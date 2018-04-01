//
// Created by POTATO on 6/1/2017.
//

#include "MyPhysicsBody.h"

USING_NS_CC;

void MyPhysicsBody::setPosition(float positionX, float positionY){
    PhysicsBody::setPosition(positionX,positionY);
}

void MyPhysicsBody::setRotation(float rotation){
    PhysicsBody::setRotation(rotation);
}

void MyPhysicsBody::setScale(float scaleX, float scaleY){
    PhysicsBody::setScale(scaleX,scaleY);
}
