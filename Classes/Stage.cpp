#include "Stage.h"

USING_NS_CC;

Stage::Stage()
:_tiledMap(nullptr)
,_player(nullptr)
{

}

Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_tiledMap);
	CC_SAFE_RELEASE_NULL(_player);
}

bool Stage::init()
{
	if (!Layer::init()) {
		return false;
	}

	//make a node from map file
	auto map = TMXTiledMap::create("stage1.tmx");
	this->addChild(map);
	this->setTiledMap(map);

	//get terrainLayer
	auto terrainLayer = map->getLayer("Terrain");
	//get object layer
	auto objectLayer = map->getLayer("Object");

	//map size
	auto mapSize = map->getMapSize();
	for(int x = 0;x < mapSize.width;x++) {
		for(int y = 0;y < mapSize.height;y++) {
			auto cooridnate = Vec2(x,y);
			this->addPhysicsBody(terrainLayer,cooridnate);
			this->addPhysicsBody(objectLayer,cooridnate);
		}
	}

	//make the player
	auto player = Player::create();
	player->setPosition(Vec2(40,160));
	this->addChild(player);
	this->setPlayer(player);

	//follow the player untile it is in the map
	// auto winSize = Director::getInstance()->getWinSize();
	this->runAction(Follow::create(
		player,
		Rect(0,0,_tiledMap->getContentSize().width,_tiledMap->getContentSize().height)));


	//enable update() to be called every frames
	this->scheduleUpdate();

	return true;
}

Sprite* Stage::addPhysicsBody(cocos2d::TMXLayer* layer,cocos2d::Vec2& cooridnate)
{
	auto sprite = layer->getTileAt(cooridnate);
	if(sprite) {
		//make a material for the physicsbody
		auto material = PhysicsMaterial();
		material.friction = 0;
		//make a physicsbody
		auto physicsbody = PhysicsBody::createBox(sprite->getContentSize(),material);
		physicsbody->setDynamic(false);
		//move anchor point
		sprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		sprite->setPhysicsBody(physicsbody);

		return sprite;
	}

	return nullptr;
}

void Stage::update(float dt)
{

}
