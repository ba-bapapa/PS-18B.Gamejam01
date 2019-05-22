/**
*@file TitleScene.cpp
*/
#include "TitleScene.h"
#include<glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include"Audio3.h"


/**
*初期化.
*/

bool TitleScene::Initialize() {

	if (!meshList.Allocate()) {
		return false;
	}

	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));
	texLogo.Reset(Texture::LoadImage2D("Res/tosi_haikyo00.tga"));
	texLightButtonEnd.Reset(Texture::LoadImage2D("Res/EndLight.tga"));
	texButtonEnd.Reset(Texture::LoadImage2D("Res/End.tga"));
	texLightButtonStart.Reset(Texture::LoadImage2D("Res/LightStart.tga"));
	texButtonStart.Reset(Texture::LoadImage2D("Res/Start.tga"));

	timer = 1.0f;
	BlinkTimer = 0.5f;

	isFinish = false;
	ButtonStart_f = true;
	ButtonEnd_f = false;

	if (!TitleAudio.Initialize()) {
		printf("TitleAudio error");
	}

	titlebgm = TitleAudio.Prepare("Res/Audio/game_maoudamashii_7_rock50.mp3");
	titlebgm->Play(Audio::Flag_Loop);

	return true;	

	
}

/**
*入力の反映.
*/

void TitleScene::ProcessInput() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	
	//はじまる
	if (window.IsKeyPressed(GLFW_KEY_W) || window.IsKeyPressed(GLFW_KEY_UP)) {
		/*Audio::Engine::Instance().Prepare("Res/Audio/decision2.mp3")->Play();*/
		ButtonEnd_f = false;
		ButtonStart_f = true;
		
	}
	//おわる
	if (window.IsKeyPressed(GLFW_KEY_S) || window.IsKeyPressed(GLFW_KEY_DOWN)) {
		/*Audio::Engine::Instance().Prepare("Res/Audio/decision2.mp3")->Play();*/
		ButtonEnd_f = true;
		ButtonStart_f = false;
		
	}
	if (!isFinish && timer <= 0.0f) {
		//はじまるボタンが選ばれているときにENTERを押すとゲームスタート
		if (ButtonStart_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
			Audio::Engine::Instance().Prepare("Res/Audio/decision2.mp3")->Play();
			NextScene("MainGameScene");
			isFinish = true;
			
			
		}
		//おわるボタンが選ばれているときにENTERを押すとゲーム終了
		else if (ButtonEnd_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
			Audio::Engine::Instance().Prepare("Res/Audio/decision2.mp3")->Play();
			exit(0);
			
		}
	}
}

/**
*状態の更新.
*/
void TitleScene::Update() {

	GLFWEW::Window& window = GLFWEW::Window::Instance();
	TitleAudio.Update();

	if (timer > 0.0f) {
		timer -= window.DeltaTime();
	}

}

/**
*描画.
*/

void TitleScene::Render() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//半透明合成機能を有効にする.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	meshList.BindVertexArraay();
	progSimple.Use();

	//正射影で描画するように行列を設定.

	const glm::mat4x4 matProj = glm::ortho(0.0f, 800.f, 0.0f, 600.0f, 1.0f, 500.0f);
	const glm::mat4x4 matView =
	glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	//背景とロゴをウィンドウの中心に描画.
	progSimple.BindTexture(0, texLogo.Get());
	progSimple.Draw(meshList.Get(4),
	glm::vec3(400, 300, 0), glm::vec3(0), glm::vec3(400, 300, 1));

	/*
	*はじめるボタン青
	*/
	if (ButtonStart_f) {
		//点滅
		if (BlinkTimer > 0.0f) {
			progSimple.BindTexture(0, texLightButtonStart.Get());
			progSimple.Draw(meshList.Get(4),
			glm::vec3(650, 300, 1), glm::vec3(0), glm::vec3(80, 30, 1));
			BlinkTimer -= window.DeltaTime();
		}
		else {
			progSimple.BindTexture(0, texButtonStart.Get());
			progSimple.Draw(meshList.Get(4),
			glm::vec3(650, 300, 1), glm::vec3(0), glm::vec3(80, 30, 1));
			BlinkTimer -= window.DeltaTime();
			if (BlinkTimer < -0.5) {
				BlinkTimer = 0.5;
			}
		}
	}
	//はじめるボタン黒
	else {
		progSimple.BindTexture(0, texButtonStart.Get());
		progSimple.Draw(meshList.Get(4),
		glm::vec3(700, 300, 1), glm::vec3(0), glm::vec3(80, 30, 1));
	}


	/*
	*おわるボタン青
	*/
	if (ButtonEnd_f) {
		//点滅
		if (BlinkTimer > 0.0f) {
			progSimple.BindTexture(0, texLightButtonEnd.Get());
			progSimple.Draw(meshList.Get(4),
				glm::vec3(650, 250, 1), glm::vec3(0), glm::vec3(80, 30, 1));
			BlinkTimer -= window.DeltaTime();
		}
		else {
			progSimple.BindTexture(0,texButtonEnd.Get());
			progSimple.Draw(meshList.Get(4),
				glm::vec3(650, 250, 1), glm::vec3(0), glm::vec3(80, 30, 1));
			BlinkTimer -= window.DeltaTime();
			if (BlinkTimer < -0.5) {
				BlinkTimer = 0.5;
			}
		}
	}
	//おわるボタン黒
	else {
		progSimple.BindTexture(0, texButtonEnd.Get());
		progSimple.Draw(meshList.Get(4),
			glm::vec3(700, 250, 1), glm::vec3(0), glm::vec3(80, 30, 1));
	}
}

/**
*終了.
*/
void TitleScene::Finalize() {

	TitleAudio.Destroy();
}

/**
*タイトル画面が終わったかどうかを調べる.
*
*@retval true	終わった.
*@retval false	終わっていない.
*/

bool TitleScene::IsFinish()const {

	return isFinish;
}