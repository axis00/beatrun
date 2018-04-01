/*
 * GameManager.h
 *
 *  Created on: Feb 8, 2016
 *      Author: POTATO
 */

#ifndef GAMEMANAGER_H_
#define GAMEMANAGER_H_

#include "cocos2d.h"

class GameManager {
public:
	GameManager();

	static GameManager* getInstance();
	void lateInit();

	float getScaleX();
	float getScaleY();
	float getScaleFactor();
	static void scaleNode(cocos2d::Node* n,cocos2d::Size t);
	static float scaleNodeVal(cocos2d::Node* n,float t);
	static float scale(float n);
	void setUpScaleFactor();

	virtual ~GameManager();

private:

	GameManager(GameManager const&){};
	GameManager& operator=(GameManager const&){};

	static GameManager* m_Instance;

	float m_ScaleX;
	float m_ScaleY;
	float m_ScaleFactor;

};

#endif /* GAMEMANAGER_H_ */
