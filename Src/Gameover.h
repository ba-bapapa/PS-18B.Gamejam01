/**
*@file GAMEOVER.h
*/

#ifndef GAMEOVER_H_INCLUDED
#define GAMEOVER_H_INCLUDED

#include"GLFWEW.h"
#include"Texture.h"
#include"Shader.h"
#include"MeshList.h"
#include"Scene.h"

/**
*ゲームオーバー
*/

class GameoverScene : public Scene {

public:
	GameoverScene() = default;
	virtual ~GameoverScene() = default;

	virtual bool Initialize()override;
	virtual void ProcessInput()override;
	virtual void Update()override;
	virtual void Render()override;
	virtual void Finalize()override;

	bool IsFinish() const;

private:
	MeshList meshList;

	Texture::Image2D texBackground;
	Texture::Image2D texGameOver;

	Texture::Image2D texButtonContinue;
	Texture::Image2D texLightButtonContinue;
		;
	Texture::Image2D texLightButtonTitle;
	Texture::Image2D texButtonTitle;

	Shader::Program progSimple;

	float timer;
	bool isFinish;

	bool ButtonTitle_f;
	bool ButtonContinue_f;

	float BlinkTimer;		//点滅間隔
	float popTimer;
};

#endif // !GAMEOVER_H_INCLUDED


