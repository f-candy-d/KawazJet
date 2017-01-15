#include "Player.h"
#include "Stage.h"

USING_NS_CC;

//the num of animation frames
const int FRAME_COUNT = 4;
//max of lateral velocity
const int VELOCITY_LIMIT = 40;
//initial Acceleration
const Vec2 INITIAL_ACCELERATION = Vec2(200,0);


Player::Player()
{

}

Player::~Player()
{

}

bool Player::init()
{
	if (!Sprite::initWithFile("player.png")) {
		return false;
	}

	//get size of image for 1 frame
	auto frameSize = Size(this->getContentSize().width / FRAME_COUNT,this->getContentSize().height);
	this->setTextureRect(Rect(0,0,frameSize.width,frameSize.height));

	Vector<SpriteFrame*> frames;
	for(int i = 0;i < FRAME_COUNT;i++) {
		auto frame = SpriteFrame::create(
			"player.png",
			Rect(frameSize.width * i,
				0,
				frameSize.width,
				frameSize.height));
		frames.pushBack(frame);
	}

	auto animation = Animation::createWithSpriteFrames(frames);
	animation->setDelayPerUnit(0.05);
	this->runAction(RepeatForever::create(Animate::create(animation)));

	auto body = PhysicsBody::createCircle(this->getContentSize().width / 2.0);
	body->setRotationEnable(false);
	//set category
	body->setCategoryBitmask(static_cast<int>(Stage::TyleType::PLAYER));
	body->setCollisionBitmask(static_cast<int>(Stage::TyleType::WALL));
	body->setContactTestBitmask(INT_MAX);
	this->setPhysicsBody(body);

	_acceleration = INITIAL_ACCELERATION;

	this->scheduleUpdate();

	return true;
}

void Player::update(float dt)
{
	//add the player acceleration every frames
	this->getPhysicsBody()->applyImpulse(_acceleration);
	//lateral velocity must be smaller than the limit velocity
	auto v = this->getPhysicsBody()->getVelocity();
	if (v.x > VELOCITY_LIMIT) {
		v.x = VELOCITY_LIMIT;
		this->getPhysicsBody()->setVelocity(v);
	}
}
