#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "Stage.h"

class MainScene :public cocos2d::Layer
{
protected:
	MainScene();
	virtual ~MainScene();
	bool init() override;

private:
	
public:
	static cocos2d::Scene* createScene();
	void update(float dt) override;
	CREATE_FUNC(MainScene);
	//Stage
	CC_SYNTHESIZE_RETAIN(Stage*,_stage,Stage);
};

#endif /*defined(__MAIN_SCENE_H__) */
