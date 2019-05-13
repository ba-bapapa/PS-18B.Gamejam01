#include"GLFWEW.h"
#include"TitleScene.h"
#include"MainGameScene.h"
#include"Gameover.h"
#include<vector>

MeshList meshList;

/**
*@file Main.cpp
*/

/**
*エントリーポイント.
*/

int main() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	if (!window.Init(800, 600, "OpenGLTutorial")) {
		return 1;
	}

	Scene* pScene = new TitleScene;
	if(!pScene || !pScene->Initialize()){
		delete pScene;
		return 1;
	}
	
	//メインループ.

	window.IniTimer();
	while (!window.ShouldClose()) {
		window.UpDateTimer();

		pScene->ProcessInput();
		pScene->Update();
		pScene->Render();
		//NextSceneが設定されていたら、シーンを切り替える.
		if(!pScene->NextScene().empty()){
			const std::string SceneName =
				pScene->NextScene();
			pScene->Finalize();
			delete pScene;
			pScene = nullptr;
			if (SceneName == "TitleScene") {
				pScene = new TitleScene;
			}
			else if (SceneName == "MainGameScene") {
				pScene = new MainGameScene;

			}
			else if (SceneName == "GameoverScene") {
				pScene = new GameoverScene;

			}
			if (!pScene || !pScene->Initialize()) {
				break;
			}
		}

		window.SwapBuffers();

	}
	if (pScene) {
		pScene->Finalize();
		delete pScene;
	}

	return 0;
}