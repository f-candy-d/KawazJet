#ifndef __STAGE_H__
#define __STAGE_H__

#include "cocos2d.h"
#include "Player.h"

class Stage :public cocos2d::Layer
{
protected:
	Stage();
	virtual ~Stage();
	bool init() override;

private:
	/**
	 * [addPhysicsBody description]
	 * @method addPhysicsBody
	 * @param  layer          [description]
	 * @param  cooridnate     [description]
	 * @return                [description]
	 */
	cocos2d::Sprite* addPhysicsBody(cocos2d::TMXLayer* layer,cocos2d::Vec2& cooridnate);

public:
	void update(float dt) override;

	//tiled map
	CC_SYNTHESIZE_RETAIN(cocos2d::TMXTiledMap*,_tiledMap,TiledMap);
	//player
	CC_SYNTHESIZE_RETAIN(Player*,_player,Player);
	//cocos macro
	CREATE_FUNC(Stage);

	//Tile type
	enum class TyleType {
		WALL = 1 << 0,
		PLAYER = 1 << 1,
		ENEMY = 1 << 2,
		COIN = 1 << 3
	};
};

#endif /* defined(__STAGE_H__) */
