//
// Created by POTATO on 5/27/2017.
//

#ifndef PROJ_ANDROID_STUDIO_PLAYER_H
#define PROJ_ANDROID_STUDIO_PLAYER_H


#include <2d/CCSprite.h>
#include <2d/CCMotionStreak.h>

class Player : public cocos2d::Sprite{
public:
    Player();
    static Player* createWithPhysics();

    /*
     * sets size in pixels based on the
     */
    void setSize(const cocos2d::Size &size);
    bool isInContact();
    void setInContact(bool c);
    void drop();
    void slide(cocos2d::Node* platform);
    void normalizeRotation();

    void update(float dt);

    void reset();

private:
    float vel_x,vel_y;
    cocos2d::Size size;
    cocos2d::PhysicsBody* m_PhysicsBody;
    cocos2d::MotionStreak* m_Streak;
    bool m_InContact;
    unsigned short m_CameraMask;
protected:

};


#endif //PROJ_ANDROID_STUDIO_PLAYER_H
