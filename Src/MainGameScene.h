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
*プレイヤーが操作するアクター.
*/

class PlayerActor : public Actor {

public:
	virtual ~PlayerActor() = default;
	virtual void Update(float deltaTime)override;
};

/**
*プレイヤーから発射される弾のアクター.
*/
class BulletActor : public Actor {

public:
	virtual ~BulletActor() = default;
	virtual void Update(float deltaTime) override;
};

/**
*敵(ゾンビ)のアクター.
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
*木のアクター.
*/
class TreeActor : public Actor {

public:
	virtual ~TreeActor() = default;
	virtual void Update(float deltaTime)override;
};

/**
*Billのアクター.
*/
class BillActor : public Actor {

public:
	virtual ~BillActor() = default;
	virtual void Update(float deltaTime)override;
};

/**
*アイテムのアクター.
*/
class ItemActor : public Actor {

public:
	virtual ~ItemActor() = default;
	virtual void Update(float deltaTime)override;
	float Timer1 = 0.0f;
};

/**
*アイテムのアクター.
*/
class Item2Actor : public Actor {

public:
	virtual ~Item2Actor() = default;
	virtual void Update(float deltaTime)override;
	float Timer1 = 0.0f;
};

/**
*アイテムのアクター.
*/
class Item3Actor : public Actor {

public:
	virtual ~Item3Actor() = default;
	virtual void Update(float deltaTime)override;
	float Timer1 = 0.0f;
};

/**
*メインゲーム画面.
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
	int Bulletremaining = 10;				//弾初期値
	int MaxBullet = 10;						//弾の最高値
	bool Mg_f = false;						//マシンガンフラグ
	bool Sg_f = false;						//マシンガンフラグ
	bool Be_f = false;						//マシンガンフラグ
	bool HP_f = false;						//マシンガンフラグ
	int useBullet = 0;						//使用弾丸数

	//ボスフラグ
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

	//情報表示用テクスチャ.
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

	//仮メニュー
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

	//ゲーム状態.
	enum class State {
		play,		//通常プレイ中.
		stageClear,	//ステージクリア.
		gameOver,	//ゲームオーバー.
		MenuScene,	//メニュー画面
		ranking,	//ランキングシーン
	};
	State state = State::play;

	int stageNo = 1;
	int score = 0;
	float enemySpeed = 1.0f;			//ゾンビの移動速度.
	float enemyPoppingInterval = 15.0f;	//ゾンビの出現間隔.
	float enemyPoppingTimer = 0.0f;		//ゾンビ出現タイマー.
	float enemyPoppingTimer2 = 0.0f;		//ゾンビ出現タイマー.
	float enemyPoppingTimer3 = 0.0f;		//ゾンビ出現タイマー.
	int enemyTotal = 20;				//敵の総数.
	int enemyLeft = 20;				//未登場の敵の数.敵を出現させるたびに減少していく.
	int enemyKilled = 0;				//殺した敵の数.この数値がenemyTotalと等しくなったらクリア.
	int enemyKilledAll = 0;		//キル数
	int itemLeft = 1;				//未登場の敵の数.敵を出現させるたびに減少していく.
	float itemPoppingTimer = 20.0f;		//アイテム出現タイマー.
	float itemPoppingInterval = 20.0f;	//アイテムの出現間隔.
	int bossenemyLeft = 1;				//未登場の敵の数.敵を出現させるたびに減少していく.
	int bossHP = 100;					//ボスの体力

	int item2Left = 1;				//未登場の敵の数.敵を出現させるたびに減少していく.
	float item2PoppingTimer = 30.0f;		//アイテム出現タイマー.
	float item2PoppingInterval = 30.0f;	//アイテムの出現間隔.

	int item3Left = 1;				//未登場の敵の数.敵を出現させるたびに減少していく.
	float item3PoppingTimer = 60.0f;		//アイテム出現タイマー.
	float item3PoppingInterval =60.0f;	//アイテムの出現間隔.

	//仮メニュー
	bool ButtonStart_f;
	bool ButtonEnd_f;
	float BlinkTimer;		//点滅間隔
	///
};

#endif // ! MAINGAMESCENE_H_INCLUDED

