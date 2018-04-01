//
// Created by POTATO on 5/28/2017.
//

enum CollisionMask{
    playerMask = 1,
    basicPlatformMask = 2,
    hazardMask = 3
};

#ifndef __DEFINITIONS_H
#define __DEFINITIONS_H

#define SPLASH_DISPLAY_TIME 1
#define TRAINSITION_TIME .5

#define DESIGN_RESOLUTION cocos2d::Size(1024, 768)

#define SCALE_NODE(n,t) GameManager::scaleNode(n,t)
#define SCALE_VAL(n,t) GameManager::scaleNodeVal(n,t)

#define DEF_PLAYER_SIZE_X 55
#define DEF_PLAYER_SIZE_Y 55

#define DEF_PLAYER_VEL_X 1200
#define DEF_PLAYER_VEL_Y 1000
#define SLIDE_PLAYER_VEL_X 1500
#define PLAYER_DROP_VEL -3000
#define PLAYER_SLOW_DROP_VEL -1500
#define DEF_PLAYER_ANG_VEL -2

#define GAME_GRAVITY -2000

#endif //__DEFINITIONS_H
