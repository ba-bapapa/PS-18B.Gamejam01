/**
*@file TitleScene.h
*/

#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED

#include"GLFWEW.h"
#include"Texture.h"
#include"Shader.h"
#include"MeshList.h"
#include"Scene.h"
#include"Audio3.h"

/**
*タイトル画面.
*/

class TitleScene : public Scene{

public:
	TitleScene() = default;
	virtual ~TitleScene() = default;

	virtual bool Initialize()override;
	virtual void ProcessInput()override;
	virtual void Update()override;
	virtual void Render()override;
	virtual void Finalize()override;
	
	bool IsFinish() const;

private:
	MeshList meshList;

	Texture::Image2D texLogo;
	Texture::Image2D texLightButtonStart;
	Texture::Image2D texButtonStart;
	Texture::Image2D texLightButtonEnd;
	Texture::Image2D texButtonEnd;

	Audio::EngineRef TitleAudio = Audio::Engine::Instance();
	Audio::SoundPtr titlebgm;
	Audio::SoundPtr titleok;
	Audio::SoundPtr titlemove;
	Audio::SoundPtr titlecancel;

	Shader::Program progSimple;

	float timer;
	bool isFinish;

	bool ButtonStart_f;
	bool ButtonEnd_f;	

	float BlinkTimer;		//点滅間隔
	float btntimer = 0.1f;
};

#endif // !TITLESCENE_H_INCLUDED

