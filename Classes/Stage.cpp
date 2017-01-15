#include "Stage.h"

USING_NS_CC;

//the format of stage file.the file must be in the Resources/map/ directory
const char* STAGE_FILE_FORMAT = "stage%d.tmx";

Stage::Stage()
:_tiledMap(nullptr)
,_player(nullptr)
,_level(0)
{

}

Stage::~Stage()
{
	CC_SAFE_RELEASE_NULL(_tiledMap);
	CC_SAFE_RELEASE_NULL(_player);
}

// bool Stage::init()
bool Stage::initWithLevel(int level)
{
	if (!Layer::init()) {
		return false;
	}

	//set stage level
	_level = level;

	//make a node from map file
	auto stageFile = StringUtils::format(STAGE_FILE_FORMAT,level);
	// auto map = TMXTiledMap::create("stage1.tmx");
	auto map = TMXTiledMap::create(stageFile);
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

		//get tile id
		auto gid = layer->getTileGIDAt(cooridnate);
		//get tile properties
		auto properties = _tiledMap->getPropertiesForGID(gid).asValueMap();
		if (properties.count("category") > 0) {
			auto category = properties.at("category").asInt();
			//set property to a physicsbody
			physicsbody->setCategoryBitmask(category);

			physicsbody->setContactTestBitmask(static_cast<int>(TyleType::PLAYER));
			physicsbody->setCollisionBitmask(static_cast<int>(TyleType::PLAYER));
		}

		return sprite;
	}

	return nullptr;
}

Stage* Stage::createWithLevel(int level)
{
	Stage* ret = new Stage();
	if (ret->initWithLevel(level)) {
		ret->autorelease();

		return ret;
	}

	CC_SAFE_RELEASE_NULL(ret);

	return nullptr;
}

void Stage::update(float dt)
{

}
