#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "Stage.h"

class MainScene :public cocos2d::Layer
{
protected:
	MainScene();
	virtual ~MainScene();
	// bool init() override;
	/**
	 * [initWithLevel description]
	 * Use this function instead of init().
	 * @method initWithLevel
	 * @param  level         [description]
	 * @return               [description]
	 */
	bool initWithLevel(int level);

private:

public:
	// static cocos2d::Scene* createScene();
	/**
	 * [createSceneWithLevel description]
	 * Use this function instead of createScene().
	 * @method createSceneWithLevel
	 * @param  level                [description]
	 * @return                      [description]
	 */
	static cocos2d::Scene* createSceneWithLevel(int level);
	void update(float dt) override;
	//do not use this macro
	// CREATE_FUNC(MainScene);
	//Stage
	CC_SYNTHESIZE_RETAIN(Stage*,_stage,Stage);
	//If the screen is pressed this flag has true,or false
	CC_SYNTHESIZE(bool,_isPress,IsPress);
};

#endif /*defined(__MAIN_SCENE_H__) */
