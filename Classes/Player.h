#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player :public cocos2d::Sprite
{
protected:
	Player();
	virtual ~Player();
	bool init() override;

private:

public:
	void update(float dt) override;

	//driving force
	CC_SYNTHESIZE_PASS_BY_REF(cocos2d::Vec2,_acceleration,Acceleration);
	CREATE_FUNC(Player);
};

#endif /* defined(__PLAYER_H__) */
