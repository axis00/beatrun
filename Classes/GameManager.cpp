/*
 * GameManager.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: POTATO
 */

#include "GameManager.h"
#include "cocos2d.h"
#include "Definitions.h"
#include "MyBodyParser.h"

USING_NS_CC;

GameManager* GameManager::m_Instance = NULL;

GameManager::GameManager() {
	setUpScaleFactor();
}

GameManager* GameManager::getInstance(){

	if(!m_Instance){
		m_Instance = new GameManager;
	}

	return m_Instance;
}

float GameManager::getScaleX(){
	return m_ScaleX;
}
float GameManager::getScaleY(){
	return m_ScaleY;
}
float GameManager::getScaleFactor(){
	return m_ScaleFactor;
}

void GameManager::scaleNode(cocos2d::Node* n,cocos2d::Size t){

	cocos2d::Rect o = cocos2d::utils::getCascadeBoundingBox(n);
	n->setScale(t.width/o.size.width,
				t.height/o.size.height);

}

float GameManager::scaleNodeVal(cocos2d::Node* n,float t){

	return t/n->getContentSize().height;

}

float GameManager::scale(float n){

	float contentScaleFactor = cocos2d::Director::getInstance()->getOpenGLView()->getFrameSize().height/1024;

	return n * contentScaleFactor;

}

void GameManager::setUpScaleFactor() {
	Size vissibleSize = Director::getInstance()->getVisibleSize();
	Size designResolution = DESIGN_RESOLUTION;

    m_ScaleX = vissibleSize.width/designResolution.width;
	m_ScaleY = vissibleSize.height/designResolution.height;

}

GameManager::~GameManager() {
	// TODO Auto-generated destructor stub
}

