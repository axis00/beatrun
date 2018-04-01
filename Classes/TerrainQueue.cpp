//
// Created by POTATO on 6/3/2017.
//

#include <base/CCDirector.h>
#include "TerrainQueue.h"
#include "Platform.h"

USING_NS_CC;

TerrainQueue* TerrainQueue::createWithSceneAndCamera(cocos2d::Scene* s, cocos2d::Camera* c){


    TerrainQueue* res = new TerrainQueue();

    res->setParentScene(s);

    res->setCamera(c);

    res->testQueue();

    return res;

}

TerrainQueue::TerrainQueue(){

}

void TerrainQueue::init(){
    visible_Queue = new std::queue<Node*>;
    complete_Queue = new std::queue<Node*>;

    m_origin = Director::getInstance()->getVisibleOrigin();
    m_visibleSize = Director::getInstance()->getVisibleSize();

    testQueue();
}

void TerrainQueue::setParentScene(cocos2d::Scene* p){
    m_ParentScene = p;
}

cocos2d::Scene* TerrainQueue::getParentScene(){
    return m_ParentScene;
}

std::queue<Node*>* TerrainQueue::getQueue(){
    return visible_Queue;
}

void TerrainQueue::update(float dt) {

    CCLOG("Updating");

    CCLOG("complete: %d, visible: %d",complete_Queue->size(),visible_Queue->size());

    if (isNodeVisible(complete_Queue->front())) {
        visible_Queue->push(complete_Queue->front());
        complete_Queue->pop();

        if (!isNodeVisible(visible_Queue->front())) {
            visible_Queue->pop();
        }

    }


}

void TerrainQueue::testQueue(){

    for(int i = 0; i < 50; i++){
        Platform* plat = Platform::createWithPhysics(PlatformType::flat);
        if(complete_Queue->empty()){
            plat->setPosition(Vec2(m_origin.x + 400, m_origin.y + m_visibleSize.height/2 - 200));
        }else{
            plat->setPosition(Vec2(complete_Queue->back()->getPositionX() +
                                   complete_Queue->back()->getBoundingBox().size.width/2 + std::rand() %
                                                                                       1400,
                                   (std::rand() % ((int)m_visibleSize.height - 800)) + 300));
        }

        complete_Queue->push(plat);
    }

}

cocos2d::Camera* TerrainQueue::getCamera(){
    return m_Cam;
}

void TerrainQueue::setCamera(cocos2d::Camera* cam){
    m_Cam = cam;
}

TerrainQueue::~TerrainQueue(){
    delete visible_Queue;
    delete complete_Queue;
}

bool TerrainQueue::isNodeVisible(cocos2d::Node* n){

    float buffer = 100;

    if(n){
        CCLOG("not null");
    }

    float n_XPos = n->getPosition().x;
    float n_XSize = n->getBoundingBox().size.width;

    CCLOG("%f",n_XPos);

    if(n_XPos > (m_Cam->getPosition().x + m_visibleSize.width/2 + n_XSize + buffer) ||
            n_XPos < (m_Cam->getPosition().x - m_visibleSize.width/2 - n_XSize - buffer)){
        return false;
    }else{
        return true;
    }

}