/**
*@file GameoverScene.cpp
*/
#include "Gameover.h"
#include<glm/gtc/matrix_transform.hpp>

/**
*������.
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
*���͂̔��f.
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
		//�͂��܂�{�^�����I�΂�Ă���Ƃ���ENTER�������ƃQ�[���ĊJ
		if (ButtonContinue_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
			NextScene("MainGameScene");
			isFinish = true;
		}
		//Title�{�^�����I�΂�Ă���Ƃ���ENTER��������Title��
		else if (ButtonTitle_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
			NextScene("TitleScene");
			isFinish = true;
		}
	}
}

/**
*��Ԃ̍X�V.
*/
void GameoverScene::Update() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (timer > 0.0f) {
		timer -= window.DeltaTime();
	}
}

/**
*�`��.
*/

void GameoverScene::Render() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	meshList.BindVertexArraay();
	progSimple.Use();

	//���ˉe�ŕ`�悷��悤�ɍs���ݒ�.

	const glm::mat4x4 matProj = glm::ortho(0.0f, 800.f, 0.0f, 600.0f, 1.0f, 500.0f);
	const glm::mat4x4 matView =
		glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	progSimple.SetViewProjectionMatrix(matProj * matView);

	//�w�i
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
		*Continue�{�^����
		*/
		if (ButtonContinue_f) {
			//�_��
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
		//Continue�{�^����
		else {
			progSimple.BindTexture(0, texButtonContinue.Get());
			progSimple.Draw(meshList.Get(4),
				glm::vec3(250, 250, 1), glm::vec3(0), glm::vec3(150, 60, 1));
		}


		/*
		*Title�{�^����
		*/
		if (ButtonTitle_f) {
			//�_��
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
		//Title�{�^����
		else {
			progSimple.BindTexture(0, texButtonTitle.Get());
			progSimple.Draw(meshList.Get(4),
				glm::vec3(550, 250, 1), glm::vec3(0), glm::vec3(110, 60, 1));
		}
	}
}

/**
*�I��.
*/
void GameoverScene::Finalize() {

}

/**
*�^�C�g����ʂ��I��������ǂ����𒲂ׂ�.
*
*@retval true	�I�����.
*@retval false	�I����Ă��Ȃ�.
*/

bool GameoverScene::IsFinish()const {

	return isFinish;
}