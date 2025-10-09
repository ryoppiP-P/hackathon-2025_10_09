///*****************************************************************//**
//  \file    �V�[���Ǘ� [scene.cpp]
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
//		Title_Finalize();  // �C��: Finalize���Ă�
//		break;
//	case SCENE_GAME:
//		Game_Finalize();   // �C��: Finalize���Ă�
//		break;
//	case SCENE_RESULT:
//		Result_Finalize(); // �C��: Finalize���Ă�
//		break;
//	}
//}
//
//void Scene_Update(double elapsed_time) {
//	// �V�[���؂�ւ��`�F�b�N
//	if (g_ChangeRequest) {
//		Scene_ChangeScene();
//		g_ChangeRequest = false;
//	}
//
//	// ���݂̃V�[���̍X�V
//	switch (g_Scene) {
//	case SCENE_TITLE:
//		Title_Update(elapsed_time);  // �C��: Update���Ă�
//		break;
//	case SCENE_GAME:
//		Game_Update(elapsed_time);   // �C��: Update���Ă�
//		break;
//	case SCENE_RESULT:
//		Result_Update(elapsed_time); // �C��: Update���Ă�
//		break;
//	}
//}
//
//void Scene_Draw() {
//	switch (g_Scene) {
//	case SCENE_TITLE:
//		Title_Draw();    // �C��: Draw���Ă�
//		break;
//	case SCENE_GAME:
//		Game_Draw();     // �C��: Draw���Ă�
//		break;
//	case SCENE_RESULT:
//		Result_Draw();   // �C��: Draw���Ă�
//		break;
//	}
//}
//
//void Scene_SetNextScene(Scene scene) {
//	g_NextScene = scene;      // �C��: ���̃V�[����ݒ�
//	g_ChangeRequest = true;   // �C��: �؂�ւ��t���O�𗧂Ă�
//}
//
//void Scene_ChangeScene() {
//	// ���݂̃V�[���̏I������
//	Scene_Finalize();
//	
//	// �V�[���̐؂�ւ�
//	g_Scene = g_NextScene;
//	
//	// �V�����V�[���̏�����
//	Scene_Initialize();
//}
//
//Scene Scene_GetCurrentScene() {
//	return g_Scene;
//}
