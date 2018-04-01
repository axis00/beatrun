#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "Definitions.h"
#include "GameScene.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
    return SplashScene::create();
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto label = Label::createWithTTF("Splash","fonts/Marker Felt.ttf",50);
    label->setPosition(origin.x + visibleSize.width/2,origin.y + visibleSize.height/2);

    this->addChild(label);

    this->scheduleOnce(CC_SCHEDULE_SELECTOR(SplashScene::endSplash),SPLASH_DISPLAY_TIME);

    return true;
}

void SplashScene::endSplash(float dt)
{
    auto nextScene = GameScene::createScene();

    Director::getInstance()->replaceScene(TransitionFade::create(TRAINSITION_TIME,nextScene));
}