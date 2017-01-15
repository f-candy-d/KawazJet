#include "MainScene.h"

USING_NS_CC;

//for serch path
const char* SEARCH_PATH_3D = "3d";
const char* SEARCH_PATH_FONTS = "fonts";
const char* SEARCH_PATH_IMAGE = "image";
const char* SEARCH_PATH_IMAGE_HD = "inage-hd";
const char* SEARCH_PATH_MAP = "map";
const char* SEARCH_PATH_MODEL = "model";
const char* SEARCH_PATH_MUSIC = "music";
const char* SEARCH_PATH_RES = "res";
const char* SEARCH_PATH_SE = "se";
//gravity vector
const Vec2 GRAVITY_ACCELERATION = Vec2(0,-10);
//buoyancy vector
const Vec2 IMPULSE_ACCELERATION = Vec2(0,200);

MainScene::MainScene()
:_stage(nullptr)
,_isPress(false)
{

}

MainScene::~MainScene()
{
	CC_SAFE_RELEASE_NULL(_stage);
}

bool MainScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	//setting for search path
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_3D);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_FONTS);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_IMAGE);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_IMAGE_HD);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_MAP);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_MODEL);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_MUSIC);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_RES);
	FileUtils::getInstance()->addSearchPath(SEARCH_PATH_SE);

	//make the stage 1
	auto stage = Stage::createWithLevel(1);
	this->addChild(stage);
	this->setStage(stage);

	//when the screen is pressed, set a flag
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [this](Touch* touch,Event* event) {
		this->setIsPress(true);
		return true;
	};
	listener->onTouchEnded = [this](Touch* touch,Event* event) {
		this->setIsPress(false);
	};
	listener->onTouchCancelled = [this](Touch* touch,Event* event) {
		this->setIsPress(false);
	};

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	//get contact among physicsbodys
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = [](PhysicsContact& contact) {
		log("hit");
		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);

	//enable update() to be called every frames
	this->scheduleUpdate();

	return true;
}

Scene* MainScene::createScene()
{
	//make a scene which have a physics engine
	auto scene = Scene::createWithPhysics();
	//get physics world
	auto world = scene->getPhysicsWorld();
	//set gravity
	world->setGravity(GRAVITY_ACCELERATION);

	//only when debuging
#if COCOS2D_DEBUG > 0
	world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif

	//set speed
	world->setSpeed(6.0);

	//make a Layer
	auto layer = MainScene::create();
	scene->addChild(layer);

	return scene;
}

void MainScene::update(float dt)
{
	if (_isPress) {
		_stage->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
	}
}
