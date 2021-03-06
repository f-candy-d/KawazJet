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
,_coinLabel(nullptr)
,_parallaxNode(nullptr)
,_isPress(false)
,_coin(0)
{

}

MainScene::~MainScene()
{
	CC_SAFE_RELEASE_NULL(_stage);
	CC_SAFE_RELEASE_NULL(_coinLabel);
	CC_SAFE_RELEASE_NULL(_parallaxNode);
}

// bool MainScene::init()
bool MainScene::initWithLevel(int level)
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

	auto winSize = Director::getInstance()->getWinSize();

	//make the stage
	auto stage = Stage::createWithLevel(level);
	this->setStage(stage);

	//parallax background
	auto background = Sprite::create("background.png");
	background->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	auto parallaxNode = ParallaxNode::create();
	this->setParallaxNode(parallaxNode);
	this->addChild(parallaxNode);

	auto mapWidh = stage->getTiledMap()->getContentSize().width;
	auto backgroundWidth = background->getContentSize().width;
	auto scrollRatio = (backgroundWidth - winSize.width) / mapWidh;

	parallaxNode->addChild(background,0,Vec2(scrollRatio,0),Vec2::ZERO);

	this->addChild(stage);

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
	contactListener->onContactBegin = [this](PhysicsContact& contact) {
		// log("hit")
		//get non player physicsbody
		auto otherShape = contact.getShapeA()->getBody() == _stage->getPlayer()->getPhysicsBody()
						? contact.getShapeB()
						: contact.getShapeA();
		auto body = otherShape->getBody();
		//get category
		auto category = body->getCategoryBitmask();

		//if the player and an enemy collide,game is over.
		if (category & static_cast<int>(Stage::TyleType::ENEMY)) {
			this->onGameOver();
		} else if(category & static_cast<int>(Stage::TyleType::COIN)) {
			//remove coin from the Stage
			body->getNode()->removeFromParent();
			_coin++;
			this->getCoinLabel()->setString(StringUtils::toString(_coin));
		}

		return true;
	};
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener,this);

	//displaying the number of coins Label
	auto label = Label::createWithCharMap("numbers.png",16,18,'0');
	this->addChild(label);
	label->setPosition(Vec2(winSize.width / 2.0,winSize.height - 20));
	label->enableShadow();
	this->setCoinLabel(label);


	//enable update() to be called every frames
	this->scheduleUpdate();

	return true;
}

void MainScene::onGameOver()
{
	//remove player from the parent node
	_stage->getPlayer()->removeFromParent();

	//show particle
	auto explosion = ParticleExplosion::create();
	explosion->setPosition(_stage->getPlayer()->getPosition());
	_stage->addChild(explosion);

	auto winSize = Director::getInstance()->getWinSize();

	//show 'Game-Over'
	auto gameOver = Sprite::create("gameover.png");
	gameOver->setPosition(Vec2(winSize.width / 2.0,winSize.height / 1.5));
	this->addChild(gameOver);

	//show 'Replay?' manu
	auto menuItem = MenuItemImage::create("replay.png",
										  "replay_pressed.png",
									  	  [this](Ref* sender) {
											  auto scene = MainScene::createSceneWithLevel(_stage->getLevel());
											  auto transition = TransitionFade::create(1.0,scene);
											  Director::getInstance()->replaceScene(transition);
										  });
	auto menu = Menu::create(menuItem,nullptr);
	menu->setPosition(Vec2(winSize.width / 2.0,winSize.height / 3.0));
	this->addChild(menu);
}

// Scene* MainScene::createScene()
Scene* MainScene::createSceneWithLevel(int level)
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
	// auto layer = MainScene::create();
	auto layer = new MainScene();
	if (layer && layer->initWithLevel(level)) {
		layer->autorelease();
	} else {
		CC_SAFE_DELETE(layer);
	}

	scene->addChild(layer);

	return scene;
}

void MainScene::update(float dt)
{
	//move background depending on the player`s moving
	_parallaxNode->setPosition(_stage->getPlayer()->getPosition() * -1);

	if (_isPress) {
		_stage->getPlayer()->getPhysicsBody()->applyImpulse(IMPULSE_ACCELERATION);
	}

	auto winSize = Director::getInstance()->getWinSize();
	auto position = _stage->getPlayer()->getPosition();
	const auto margin = 50;
	//if the player is out of the map,game is over.
	if(position.y < -margin || position.y >= winSize.height + margin) {
		//avoid calling onGameOver() function many times,
		//do not call it only when the player is in the map.
		if(_stage->getPlayer()->getParent() != nullptr) {
			this->onGameOver();
		}
	}
}
