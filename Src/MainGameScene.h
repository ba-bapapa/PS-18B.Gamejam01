/**
*@file MainGameScene.h
*/

#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include"GLFWEW.h"
#include"Texture.h"
#include"Shader.h"
#include"MeshList.h"
#include"Scene.h"
#include"Actor.h"
#include<vector>
#include<random>

/**
*�v���C���[�����삷��A�N�^�[.
*/

class PlayerActor : public Actor {

public:
	virtual ~PlayerActor() = default;
	virtual void Update(float deltaTime)override;
};

/**
*�v���C���[���甭�˂����e�̃A�N�^�[.
*/
class BulletActor : public Actor {

public:
	virtual ~BulletActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
*�G(�]���r)�̃A�N�^�[.
*/
class ZombieActor : public Actor {

public:
	virtual ~ZombieActor() = default;
	virtual void Update(float deltaTime)override;

	Actor* target = nullptr;
	float attackingTimer = 5.0f;
	bool isAttacking = false;
	float baseSpeed = 1.0f;
};

/**
*�؂̃A�N�^�[.
*/
class TreeActor : public Actor {

public:
	virtual ~TreeActor() = default;
	virtual void Update(float deltaTime)override;
};

/**
*Bill�̃A�N�^�[.
*/
class BillActor : public Actor {

public:
	virtual ~BillActor() = default;
	virtual void Update(float deltaTime)override;
};

/**
*�A�C�e���̃A�N�^�[.
*/
class ItemActor : public Actor {

public:
	virtual ~ItemActor() = default;
	virtual void Update(float deltaTime)override;
	float Timer1 = 0.0f;
};

/**
*�A�C�e���̃A�N�^�[.
*/
class Item2Actor : public Actor {

public:
	virtual ~Item2Actor() = default;
	virtual void Update(float deltaTime)override;
	float Timer1 = 0.0f;
};

/**
*�A�C�e���̃A�N�^�[.
*/
class Item3Actor : public Actor {

public:
	virtual ~Item3Actor() = default;
	virtual void Update(float deltaTime)override;
	float Timer1 = 0.0f;
};

/**
*���C���Q�[�����.
*/

class MainGameScene : public Scene{

public:
	MainGameScene() = default;
	virtual ~MainGameScene() = default;

	virtual bool Initialize()override;
	virtual void ProcessInput()override;
	virtual void Update()override;
	virtual void Render()override;
	virtual void Finalize()override;
	int Bulletremaining = 10;				//�e�����l
	int MaxBullet = 10;						//�e�̍ō��l
	bool Mg_f = false;						//�}�V���K���t���O
	bool Sg_f = false;						//�}�V���K���t���O
	bool Be_f = false;						//�}�V���K���t���O
	bool HP_f = false;						//�}�V���K���t���O
	int useBullet = 0;						//�g�p�e�ې�

	//�{�X�t���O
	bool boss_f = false;
	bool stagecrea_f = false;

private:
	MeshList meshList;

	Texture::Image2D texId;
	Texture::Image2D texTree;
	Texture::Image2D texHouse;
	Texture::Image2D texRock;
	Texture::Image2D texHuman;
	Texture::Image2D texBullet;
	Texture::Image2D texPlane;
	Texture::Image2D texGround;
	Texture::Image2D texEnemy1;
	Texture::Image2D texEnemy2;
	Texture::Image2D texEnemy3;
	Texture::Image2D texBossenemy;

	//���\���p�e�N�X�`��.
	Texture::Image2D texScore;
	Texture::Image2D texNumber[10];
	Texture::Image2D texHP;
	Texture::Image2D texStageClear;
	Texture::Image2D texGameOver;
	Texture::Image2D texReload;
	Texture::Image2D texSiro;
	Texture::Image2D texsyasen;
	Texture::Image2D texpause;
	Texture::Image2D texkill;
	Texture::Image2D texseseki;
	Texture::Image2D texscore2;
	Texture::Image2D texuse;
	Texture::Image2D texbullet23;
	Texture::Image2D texBoss;
	Texture::Image2D texmugen;

	//�����j���[
	Texture::Image2D texLightButtonStart;
	Texture::Image2D texButtonStart;
	Texture::Image2D texLightButtonEnd;
	Texture::Image2D texButtonEnd;
	///

	Shader::Program progSimple;
	Shader::Program progLighting;
	Shader::LightList lights;

	float angleY = 0;
	glm::vec3 viewPos;

	PlayerActor player;
	std::vector<Actor*>playerBullets;
	float playerBulletTimer = 0;

	std::vector<Actor*>enemies;
	std::vector<Actor*>bossenemies;

	std::mt19937 random;

	std::vector<Actor*>treeA;

	std::vector<Actor*>BillA;

	std::vector<Actor*>ItemA;

	std::vector<Actor*>Item2A;

	std::vector<Actor*>Item3A;

	//�Q�[�����.
	enum class State {
		play,		//�ʏ�v���C��.
		stageClear,	//�X�e�[�W�N���A.
		gameOver,	//�Q�[���I�[�o�[.
		MenuScene,	//���j���[���
		ranking,	//�����L���O�V�[��
	};
	State state = State::play;

	int stageNo = 1;
	int score = 0;
	float enemySpeed = 1.0f;			//�]���r�̈ړ����x.
	float enemyPoppingInterval = 15.0f;	//�]���r�̏o���Ԋu.
	float enemyPoppingTimer = 0.0f;		//�]���r�o���^�C�}�[.
	float enemyPoppingTimer2 = 0.0f;		//�]���r�o���^�C�}�[.
	float enemyPoppingTimer3 = 0.0f;		//�]���r�o���^�C�}�[.
	int enemyTotal = 20;				//�G�̑���.
	int enemyLeft = 20;				//���o��̓G�̐�.�G���o�������邽�тɌ������Ă���.
	int enemyKilled = 0;				//�E�����G�̐�.���̐��l��enemyTotal�Ɠ������Ȃ�����N���A.
	int enemyKilledAll = 0;		//�L����
	int itemLeft = 1;				//���o��̓G�̐�.�G���o�������邽�тɌ������Ă���.
	float itemPoppingTimer = 20.0f;		//�A�C�e���o���^�C�}�[.
	float itemPoppingInterval = 20.0f;	//�A�C�e���̏o���Ԋu.
	int bossenemyLeft = 1;				//���o��̓G�̐�.�G���o�������邽�тɌ������Ă���.
	int bossHP = 100;					//�{�X�̗̑�

	int item2Left = 1;				//���o��̓G�̐�.�G���o�������邽�тɌ������Ă���.
	float item2PoppingTimer = 30.0f;		//�A�C�e���o���^�C�}�[.
	float item2PoppingInterval = 30.0f;	//�A�C�e���̏o���Ԋu.

	int item3Left = 1;				//���o��̓G�̐�.�G���o�������邽�тɌ������Ă���.
	float item3PoppingTimer = 60.0f;		//�A�C�e���o���^�C�}�[.
	float item3PoppingInterval =60.0f;	//�A�C�e���̏o���Ԋu.

	//�����j���[
	bool ButtonStart_f;
	bool ButtonEnd_f;
	float BlinkTimer;		//�_�ŊԊu
	///
};

#endif // ! MAINGAMESCENE_H_INCLUDED

