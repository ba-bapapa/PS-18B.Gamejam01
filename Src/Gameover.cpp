/**
*@file GameoverScene.cpp
*/
#include "Gameover.h"
#include<glm/gtc/matrix_transform.hpp>

/**
*初期化.
*/

bool GameoverScene::Initialize() {

	if (!meshList.Allocate()) {
		return false;
	}

	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));

	texBackground.Reset(Texture::LoadImage2D("Res/Dark.tga"));
	texGameOver.Reset(Texture::LoadImage2D("Res/GAMEOVER.tga"));

	texButtonContinue.Reset(Texture::LoadImage2D("Res/Continue.tga"));
	texLightButtonContinue.Reset(Texture::LoadImage2D("Res/LightContinue.tga"));

	texLightButtonTitle.Reset(Texture::LoadImage2D("Res/LightTitle.tga"));
	texButtonTitle.Reset(Texture::LoadImage2D("Res/Title.tga"));

	timer = 1.0f;
	BlinkTimer = 0.5f;
	popTimer = 2.0f;

	isFinish = false;
	ButtonTitle_f = false;
	ButtonContinue_f = true;

	return true;
}

/**
*入力の反映.
*/

void GameoverScene::ProcessInput() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//Continue
	if (window.IsKeyPressed(GLFW_KEY_A) || window.IsKeyPressed(GLFW_KEY_LEFT)) {
		ButtonTitle_f = false;
		ButtonContinue_f = true;
	}
	//Title
	if (window.IsKeyPressed(GLFW_KEY_D) || window.IsKeyPressed(GLFW_KEY_RIGHT)) {
		ButtonTitle_f = true;
		ButtonContinue_f = false;
	}
	if (!isFinish && timer <= 0.0f) {
		//はじまるボタンが選ばれているときにENTERを押すとゲーム再開
		if (ButtonContinue_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
			NextScene("MainGameScene");
			isFinish = true;
		}
		//Titleボタンが選ばれているときにENTERを押すとTitleへ
		else if (ButtonTitle_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
			NextScene("TitleScene");
			isFinish = true;
		}
	}
}

/**
*状態の更新.
*/
void GameoverScene::Update() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (timer > 0.0f) {
		timer -= window.DeltaTime();
	}
}

/**
*描画.
*/

void GameoverScene::Render() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	meshList.BindVertexArraay();
	progSimple.Use();

	//正射影で描画するように行列を設定.

	const glm::mat4x4 matProj = glm::ortho(0.0f, 800.f, 0.0f, 600.0f, 1.0f, 500.0f);
	const glm::mat4x4 matView =
		glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	//背景
	progSimple.BindTexture(0, texBackground.Get());
	progSimple.Draw(meshList.Get(4),
	glm::vec3(400, 300, 0), glm::vec3(0), glm::vec3(400, 300, 1));

	popTimer -= window.DeltaTime();

	if (popTimer < 1.0f) {
		//GAMEOVER
		progSimple.BindTexture(0, texGameOver.Get());
		progSimple.Draw(meshList.Get(4),
		glm::vec3(400, 450, 1), glm::vec3(0), glm::vec3(350, 70, 1));
	}

	if (popTimer < 0.0f) {
		/*
		*Continueボタン青
		*/
		if (ButtonContinue_f) {
			//点滅
			if (BlinkTimer > 0.0f) {
				progSimple.BindTexture(0, texLightButtonContinue.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(250, 250, 1), glm::vec3(0), glm::vec3(150, 60, 1));
				BlinkTimer -= window.DeltaTime();
			}
			else {
				progSimple.BindTexture(0, texButtonContinue.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(250, 250, 1), glm::vec3(0), glm::vec3(150, 60, 1));
				BlinkTimer -= window.DeltaTime();
				if (BlinkTimer < -0.5) {
					BlinkTimer = 0.5;
				}
			}
		}
		//Continueボタン黒
		else {
			progSimple.BindTexture(0, texButtonContinue.Get());
			progSimple.Draw(meshList.Get(4),
				glm::vec3(250, 250, 1), glm::vec3(0), glm::vec3(150, 60, 1));
		}


		/*
		*Titleボタン青
		*/
		if (ButtonTitle_f) {
			//点滅
			if (BlinkTimer > 0.0f) {
				progSimple.BindTexture(0, texLightButtonTitle.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(550, 250, 1), glm::vec3(0), glm::vec3(110, 60, 1));
				BlinkTimer -= window.DeltaTime();
			}
			else {
				progSimple.BindTexture(0, texButtonTitle.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(550, 250, 1), glm::vec3(0), glm::vec3(110, 60, 1));
				BlinkTimer -= window.DeltaTime();
				if (BlinkTimer < -0.5) {
					BlinkTimer = 0.5;
				}
			}
		}
		//Titleボタン黒
		else {
			progSimple.BindTexture(0, texButtonTitle.Get());
			progSimple.Draw(meshList.Get(4),
				glm::vec3(550, 250, 1), glm::vec3(0), glm::vec3(110, 60, 1));
		}
	}
}

/**
*終了.
*/
void GameoverScene::Finalize() {

}

/**
*タイトル画面が終わったかどうかを調べる.
*
*@retval true	終わった.
*@retval false	終わっていない.
*/

bool GameoverScene::IsFinish()const {

	return isFinish;
}