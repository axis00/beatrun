#include "GameScene.h"
#include "Player.h"
#include "Definitions.h"
#include "MyBodyParser.h"
#include "GameManager.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    auto scene = GameScene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::initWithPhysics())
    {
        return false;
    }

    //str format : platType, x-offset from the x origin, y-offset from the middle of the screen
    /*
     * 1 = tri platform
     * 2 = flat platform
     * 3 = short platform
     *
     * 10 = diamond hazard
     */
    levelStr = "1,400,-200:2,825,-100:10,725,25,0,0:10,800,25,0,0:10,875,25,0,0:10,950,25,0,0:2,1825,-100:10,1725,25,0,0:10,1800,25,0,0:10,1875,25,0,0:10,1950,25,0,0";

    m_GameOver = false;
    m_IsPressed = false;
    m_Restarting = false;
    m_GameStarted = false;

    m_NodeCounter = 0;

    m_visibleSize = Director::getInstance()->getVisibleSize();
    m_origin = Director::getInstance()->getVisibleOrigin();

    gameCam = Camera::create();
    gameCam->setCameraFlag(CameraFlag::USER1);

    this->addChild(gameCam);

    bgSprite = Sprite::create("background.png");
    bgSprite->setPosition(gameCam->getPosition());
    bgSprite->setCameraMask((unsigned short)CameraFlag::USER1);
    SCALE_NODE(bgSprite,m_visibleSize);

    this->addChild(bgSprite,0);

    _physicsWorld->setSpeed(1);
    _physicsWorld->setSubsteps(2);

    MyBodyParser::getInstance()->parseJsonFile("Physics_Bodies.json");

    playerStreak = MotionStreak::create(0.1f,20.0f,64.0f,Color3B::WHITE,"trail.png");
    playerStreak->setFastMode(false);
    playerStreak->setCameraMask((unsigned short)CameraFlag::USER1,true);
    playerStreak->setVisible(false);
    this->addChild(playerStreak);

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));

    closeItem->setPosition(Vec2(m_origin.x + m_visibleSize.width - closeItem->getContentSize().width/2 ,
                                m_origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 3);

    m_Player = Player::createWithPhysics();
    m_Player->setPosition(400 + m_origin.x,m_visibleSize.height/2 + m_origin.y + 100);
    m_Player->getPhysicsBody()->setAngularVelocity(DEF_PLAYER_ANG_VEL);

    this->addChild(m_Player);

    //set touch event listener on screen
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan,this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded,this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

    this->getPhysicsWorld()->setGravity(Vec2(0,GAME_GRAVITY));
    //add platform test
    //populatePlatforms(700);

    complete_Vector = parseLevel(levelStr);
    loadLevel(complete_Vector);

    //set contact listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin,this);
    contactListener->onContactSeparate = CC_CALLBACK_1(GameScene::onContactSeparate,this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);

    //schedules update func to be called as often as possible(i.e. ever 1/60 seconds)
    schedule(schedule_selector(GameScene::update));

    CCLOG("init success");

    return true;
}


void GameScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
    #endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not
     * use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event
     * created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

/*
 * update function getting called every frame by the scheduler in init function
 */
void GameScene::update(float dt) {

    //TODO optimize
    updateVisibleQueue();

    //no need to update anything when restarting
    if (!m_Restarting) {
        //check if game over
        if (checkGameOver()) {
            onGameOver();
        } else {
            m_Player->update(dt);
            playerStreak->setPosition(m_Player->getPosition());
            float playerVel_X = m_Player->getPhysicsBody()->getVelocity().x;
            float camOffeset_X = playerVel_X * dt;
            float camPos_X = (gameCam->getPosition().x + camOffeset_X);
            gameCam->setPosition(camPos_X, m_origin.y + m_visibleSize.height / 2);
            bgSprite->setPosition(gameCam->getPosition());
        }
    }


}

/*
 * touch function getting called everytime the user touches the screen
 */
bool GameScene::onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event){

    if(!m_GameStarted && !m_Restarting) {
        onGameStart();
    }else if(m_GameOver){
        onRestart(0);
    }else{
        m_Player->drop();
        m_Player->normalizeRotation();
    }

    m_IsPressed = true;

    return true;
}

/*
 * touch funtion called when user lifts finger
 */
bool GameScene::onTouchEnded(cocos2d::Touch* touch,cocos2d::Event* event){

    m_IsPressed = false;

    if(m_Player->isInContact()) {
        m_Player->getPhysicsBody()->setVelocity(Vec2(DEF_PLAYER_VEL_X, DEF_PLAYER_VEL_Y));
        m_Player->getPhysicsBody()->setAngularVelocity(DEF_PLAYER_ANG_VEL);
    }

    return true;

}

/*
 * gets called when 2 bodies come in contact with each other
 */
bool GameScene::onContactBegin(cocos2d::PhysicsContact &contact){

    PhysicsShape* a = contact.getShapeA();
    PhysicsShape* b = contact.getShapeB();

    //when player and platform touches
    if((a->getCollisionBitmask() == CollisionMask::playerMask &&
        b->getCollisionBitmask() == CollisionMask::basicPlatformMask) ||
       (b->getCollisionBitmask() == CollisionMask::playerMask &&
        a->getCollisionBitmask() == CollisionMask::basicPlatformMask)){

        return onContactWithPlatform(contact);

    }

    //if collides with hazard
    if((a->getCollisionBitmask() == CollisionMask::playerMask &&
        b->getCollisionBitmask() == CollisionMask::hazardMask) ||
       (b->getCollisionBitmask() == CollisionMask::playerMask &&
        a->getCollisionBitmask() == CollisionMask::hazardMask)){

        //TODO make hazard collide func
        //temp
        onGameOver();

    }

    return true;
}

bool GameScene::onContactSeparate(cocos2d::PhysicsContact &contact){

    PhysicsShape* a = contact.getShapeA();
    PhysicsShape* b = contact.getShapeB();

    //check if the one of bodies that separated was the player
    if(a->getCollisionBitmask() == CollisionMask::playerMask ||
            b->getCollisionBitmask() == CollisionMask::playerMask){
        m_Player->setInContact(false);
        if(m_IsPressed){
            m_Player->getPhysicsBody()->setVelocity(Vec2(DEF_PLAYER_VEL_X,PLAYER_SLOW_DROP_VEL));
            m_Player->getPhysicsBody()->setAngularVelocity(DEF_PLAYER_ANG_VEL);
        }
    }

    return true;

}

void GameScene::populatePlatforms(int n){

    std::srand(std::time(NULL));
    for(int i = 0; i < n; i++){
        Platform* plat = Platform::createWithPhysics(PlatformType::triangle);
        plat->setVisible(false);
        plat->getPhysicsBody()->setEnabled(false);

        if(complete_Vector.empty()){
            plat->setPosition(Vec2(m_origin.x + 400, m_origin.y + m_visibleSize.height/2 - 200));
        }else{

            plat->setPosition(Vec2(complete_Vector.back()->getPositionX() +
                                           complete_Vector.back()->getBoundingBox().size.width + std::rand() %
                                                                                               800,
                                   //,m_origin.y + m_visibleSize.height/2 - 200));
                                   (std::rand() % ((int)m_visibleSize.height - 800)) + 300));


        }
        this->addChild(plat);
        complete_Vector.push_back(plat);
    }

}

bool GameScene::onContactWithPlatform(cocos2d::PhysicsContact &contact){

    PhysicsShape* a = contact.getShapeA();
    PhysicsShape* b = contact.getShapeB();

    m_Player->setInContact(true);

    //if a is player
    if(a->getCollisionBitmask() == CollisionMask::playerMask){
        Node* plat = b->getBody()->getOwner();
        //if contact pt is on top of plat
        return handleContactWithPlatform(plat);

    }else if(b->getCollisionBitmask() == CollisionMask::playerMask){ //if b is player
        Node* plat = a->getBody()->getOwner();
        //if contact pt is on top of plat
        return handleContactWithPlatform(plat);

    }

    return false;

}

bool GameScene::handleContactWithPlatform(cocos2d::Node* plat){
    if((m_Player->getPosition().y - m_Player->getBoundingBox().size.height/2) >
       (plat->getPosition().y + plat->getBoundingBox().size.height/2 - 25)) {

        if (m_IsPressed) {
            m_Player->slide(plat);
            return true;
        } else {
            if (m_GameStarted) {
                m_Player->getPhysicsBody()->setVelocity(Vec2(DEF_PLAYER_VEL_X, DEF_PLAYER_VEL_Y));
                m_Player->getPhysicsBody()->setAngularVelocity(DEF_PLAYER_ANG_VEL);

            } else {
                m_Player->getPhysicsBody()->setVelocity(Vec2(0, DEF_PLAYER_VEL_Y));
            }
            return false;
        }
    }else{
        //m_Player->getPhysicsBody()->setVelocity(Vec2(0, 0));
        return true;
    }
}

bool GameScene::isNodeVisible(cocos2d::Node* n){

   // CCLOG("Checking");

    float buffer = 200;

    float n_XPos = n->getPosition().x;
    float n_XSize = n->getBoundingBox().size.width;
    
    if(n_XPos > (gameCam->getPosition().x + m_visibleSize.width/2 + n_XSize + buffer) ||
       n_XPos < (gameCam->getPosition().x - m_visibleSize.width/2 - n_XSize - buffer)){
        return false;
    }else{
        return true;
    }

}

void GameScene::onGameOver(){
    m_Player->getPhysicsBody()->setVelocity(Vec2(0,0));
    playerStreak->setVisible(false);
    m_GameOver = true;
    m_Restarting = true;
    m_GameStarted = false;
    //scheduleOnce(schedule_selector(GameScene::onRestart),.5);
}

void GameScene::updateVisibleQueue(){

    //CCLOG("visible : %d complete : %d",visible_Queue.size(),complete_Vector.size());

    if(!visible_Queue.empty()) {
        //CCLOG("%f",visible_Queue.front()->getPosition().x);
        if (!isNodeVisible(visible_Queue.front())) {
            //CCLOG("not visible");
            visible_Queue.front()->setVisible(false);
            visible_Queue.front()->getPhysicsBody()->setEnabled(false);
            visible_Queue.pop();
        }
    }
    if(!complete_Vector.empty() && m_NodeCounter < complete_Vector.size()) {
        if (isNodeVisible(complete_Vector.at(m_NodeCounter))) {
            complete_Vector.at(m_NodeCounter)->setVisible(true);
            complete_Vector.at(m_NodeCounter)->getPhysicsBody()->setEnabled(true);
            visible_Queue.push(complete_Vector.at(m_NodeCounter));
            m_NodeCounter++;
        }

    }
}

void GameScene::onRestart(float dt){

    m_Player->setPosition(400 + m_origin.x,m_visibleSize.height/2 + m_origin.y + 100);
    m_Player->reset();

    //reset visible queue
    std::queue<cocos2d::Node*> empty;
    std::swap(visible_Queue, empty );

    for(int i = 0; i < complete_Vector.size(); i++){
        complete_Vector.at(i)->setVisible(false);
        complete_Vector.at(i)->getPhysicsBody()->setEnabled(false);
    }

    m_NodeCounter = 0;

    gameCam->setPosition(Vec2(m_origin.x + m_visibleSize.width/2,m_origin.y + m_visibleSize.height/2));

    m_Restarting = false;
}

bool GameScene::checkGameOver(){
    if(m_Player->getPosition().y < 0){
        return true;
    }
    return false;
}

void GameScene::onGameStart(){
    m_GameStarted = true;
    m_GameOver = false;
    playerStreak->setVisible(true);
}

void GameScene::loadLevel(std::vector<cocos2d::Node*> level){

    for(int i = 0; i < level.size(); i++){
        this->addChild(level.at(i));
    }

}
void GameScene::resetLevel(){

}

std::vector<Node*> GameScene::parseLevel(std::string& l){
    std::vector<Node*> res;
    std::vector<std::string> firstPass;
    std::vector<std::string> buffer;

    split(l,":",firstPass);

    for(int i = 0; i < firstPass.size(); i++){
        split(firstPass.at(i),",",buffer);
        int t = atoi(buffer.at(0).c_str());
        float x = (float)atof(buffer.at(1).c_str());
        float y = (float)atof(buffer.at(2).c_str());

        Node* lvlElement;

        switch (t){
            //node is a platform
            case 1:
            case 2:
            case 3:
                lvlElement = createPlat(t,x,y);
                break;
            //node is a hazard
            case 10:
                float rot = (float)atof(buffer.at(3).c_str());
                float angVel = (float)atof(buffer.at(4).c_str());
                lvlElement = createHaz(t,x,y,rot,angVel);
                break;
        }

        buffer.clear();

        res.push_back(lvlElement);
    }


    return res;
}

Platform* GameScene::createPlat(int t,float x, float y){
    Platform* res = Platform::createWithPhysics(t);
    res->setVisible(false);
    res->getPhysicsBody()->setEnabled(false);
    res->setPosition(Vec2(m_origin.x + x, m_origin.y + m_visibleSize.height/2 + y));
    return res;
}

Hazard* GameScene::createHaz(int t, float x, float y, float rot,float angVel) {
    Hazard* res = Hazard::createWithPhysics(t);
    res->setVisible(false);
    res->getPhysicsBody()->setEnabled(false);
    res->setPosition(Vec2(m_origin.x + x, m_origin.y + m_visibleSize.height/2 + y));
    res->setRotation(rot);
    res->setAngularVel(angVel);
    return res;
}

void GameScene::split(const std::string &s,const char* delim, std::vector<std::string> &v){
    // to avoid modifying original string
    // first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = strtok(dup, delim);
    while(token != NULL){
        v.push_back(std::string(token));
        token = strtok(NULL, delim);
    }
    free(dup);
}