#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Platform.h"
#include "TerrainQueue.h"
#include "Hazard.h"


class Player;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    //update func to be called every frame
    void update(float dt);

    //touch event
    bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
    bool onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);

private:
    Player* m_Player;
    cocos2d::MotionStreak* playerStreak;
    cocos2d::Sprite* bgSprite;
    bool m_GameOver;
    bool m_GameStarted;
    bool m_Restarting;
    bool m_IsPressed;

    std::string levelStr;

    //node counter for the visible queue
    int m_NodeCounter;

    std::queue<cocos2d::Node*> visible_Queue;
    std::vector<cocos2d::Node*> complete_Vector;

    cocos2d::Size m_visibleSize;
    cocos2d::Vec2 m_origin;

    cocos2d::Camera* gameCam;

    bool onContactWithPlatform(cocos2d::PhysicsContact &contact);
    bool handleContactWithPlatform(cocos2d::Node* plat);

    bool checkGameOver();

    bool isNodeVisible(cocos2d::Node* n);

    void onGameOver();
    void onGameStart();

    void loadLevel(std::vector<cocos2d::Node*> level);
    void resetLevel();
    std::vector<cocos2d::Node*> parseLevel(std::string& l);
    Platform* createPlat(int t,float x, float y);
    Hazard* createHaz(int t,float x, float y,float rot,float angVel);
    void split(const std::string &s,const char* delim, std::vector<std::string> &tokens);

    void updateVisibleQueue();

    void onRestart(float dt);

    void populatePlatforms(int n);

    bool onContactBegin(cocos2d::PhysicsContact &contact);
    bool onContactSeparate(cocos2d::PhysicsContact &contact);

};

#endif // __GAME_SCENE_H__
