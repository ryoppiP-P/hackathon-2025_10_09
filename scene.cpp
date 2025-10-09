///*****************************************************************//**
//  \file    シーン管理 [scene.cpp]
//  
//  \Author  Ryoto Kikuchi
//  \data    2025/9/17
// *********************************************************************/
//#include "scene.h"
//#include "title.h"
//#include "game.h"
//#include "result.h"
//
//static Scene g_Scene = SCENE_TITLE;
//static Scene g_NextScene = SCENE_TITLE;
//static bool g_ChangeRequest = false;
//
//void Scene_Initialize() {
//	switch (g_Scene) {
//		case SCENE_TITLE:
//			Title_Initialize();
//			break;
//		case SCENE_GAME:
//			Game_Initialize();
//			break;
//		case SCENE_RESULT:
//			Result_Initialize();
//			break;
//	}
//}
//
//void Scene_Finalize() {
//	switch (g_Scene) {
//	case SCENE_TITLE:
//		Title_Finalize();  // 修正: Finalizeを呼ぶ
//		break;
//	case SCENE_GAME:
//		Game_Finalize();   // 修正: Finalizeを呼ぶ
//		break;
//	case SCENE_RESULT:
//		Result_Finalize(); // 修正: Finalizeを呼ぶ
//		break;
//	}
//}
//
//void Scene_Update(double elapsed_time) {
//	// シーン切り替えチェック
//	if (g_ChangeRequest) {
//		Scene_ChangeScene();
//		g_ChangeRequest = false;
//	}
//
//	// 現在のシーンの更新
//	switch (g_Scene) {
//	case SCENE_TITLE:
//		Title_Update(elapsed_time);  // 修正: Updateを呼ぶ
//		break;
//	case SCENE_GAME:
//		Game_Update(elapsed_time);   // 修正: Updateを呼ぶ
//		break;
//	case SCENE_RESULT:
//		Result_Update(elapsed_time); // 修正: Updateを呼ぶ
//		break;
//	}
//}
//
//void Scene_Draw() {
//	switch (g_Scene) {
//	case SCENE_TITLE:
//		Title_Draw();    // 修正: Drawを呼ぶ
//		break;
//	case SCENE_GAME:
//		Game_Draw();     // 修正: Drawを呼ぶ
//		break;
//	case SCENE_RESULT:
//		Result_Draw();   // 修正: Drawを呼ぶ
//		break;
//	}
//}
//
//void Scene_SetNextScene(Scene scene) {
//	g_NextScene = scene;      // 修正: 次のシーンを設定
//	g_ChangeRequest = true;   // 修正: 切り替えフラグを立てる
//}
//
//void Scene_ChangeScene() {
//	// 現在のシーンの終了処理
//	Scene_Finalize();
//	
//	// シーンの切り替え
//	g_Scene = g_NextScene;
//	
//	// 新しいシーンの初期化
//	Scene_Initialize();
//}
//
//Scene Scene_GetCurrentScene() {
//	return g_Scene;
//}
