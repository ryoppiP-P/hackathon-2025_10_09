/*****************************************************************//**
  \file    ÉVÅ[ÉìêÿÇËë÷Ç¶ [scene.h]

  \Author  Ryoto Kikuchi
  \data    2025/9/17
 *********************************************************************/
#ifndef SCENE_H
#define SCENE_H

#include "title.h"
#include "result.h"

enum Scene {
	SCENE_TITLE,
	SCENE_GAME_ZAKO,
	SCENE_GAME_BOSS,
	SCENE_RESULT,
	SCENE_MAX
};

void Scene_SetNextScene(Scene scene);
void Scene_ChangeScene();
Scene Scene_GetCurrentScene();

#endif // SCENE_H