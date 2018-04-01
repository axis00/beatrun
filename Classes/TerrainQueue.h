//
// Created by POTATO on 6/3/2017.
//

#ifndef PROJ_ANDROID_STUDIO_TERRAINQUEUE_H
#define PROJ_ANDROID_STUDIO_TERRAINQUEUE_H

#include "cocos2d.h"
#include <queue>

class TerrainQueue {

public:
    TerrainQueue();
    ~TerrainQueue();
    static TerrainQueue* createWithSceneAndCamera(cocos2d::Scene* s, cocos2d::Camera* c);
    void setParentScene(cocos2d::Scene* p);
    void setCamera(cocos2d::Camera* cam);
    //void parseLevel(char* l);
    cocos2d::Scene* getParentScene();
    cocos2d::Camera* getCamera();
    std::queue<cocos2d::Node*>* getQueue();
    void update(float dt);
    void init();

    void testQueue();

private:
    std::queue<cocos2d::Node*>* visible_Queue;
    std::queue<cocos2d::Node*>* complete_Queue;
    cocos2d::Camera* m_Cam;
    cocos2d::Scene* m_ParentScene;

    cocos2d::Vec2 m_origin;
    cocos2d::Size m_visibleSize;

    bool isNodeVisible(cocos2d::Node* n);

};


#endif //PROJ_ANDROID_STUDIO_TERRAINQUEUE_H
