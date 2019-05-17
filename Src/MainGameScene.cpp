/**
*@file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include<glm/gtc/matrix_transform.hpp>
#include <iostream>

/**
*プレイヤーの状態を更新する.
*
*@param deltaTime	経過時間.
*/
void PlayerActor::Update(float deltaTime) {
	
	//プレイヤーが死んでいたら、地面に倒れる(-90度になる)まで「倒れ具合」を大きくしていく.
	if (health <= 0) {
		rotation.x -= glm::radians(45.0f) * deltaTime;
		if (rotation.x < glm::radians(-90.0f)) {
			rotation.x = glm::radians(-90.0f);
		}
	}
	//const float mapSize = 20;
	//for (size_t i = 0; i < 3; ++i) {
	//	if (position[i] < -mapSize || position[i] > mapSize) {
	//		if (health > 0) {
	//			health -= 1;
	//		}
	//		break;
	//	}
	//}
	Actor::Update(deltaTime);
}

/**
*プレイヤーの弾の状態を更新する.
*/
void BulletActor::Update(float deltaTime) {
	const float mapSize = 20;
	for (size_t i = 0; i < 3; ++i) {
		if (position[i] < -mapSize || position[i] > mapSize) {
			health = 0;
			break;
		}
	}
	Actor::Update(deltaTime);
}

/**
*ゾンビの状態を更新.
*/
void ZombieActor::Update(float deltaTime) {
	if (!target) {
		return;
	}
	const float moveSpeed = baseSpeed * 2.0f;
	const float rotationSpeed = baseSpeed * glm::radians(60.0f);
	const float frontRange = glm::radians(15.0f);

	//ターゲット(プレイヤー)の方向を調べる.
	const glm::vec3 v = target->position - position;
	const glm::vec3 vTarget = glm::normalize(v);
	float radian = std::atan2(-vTarget.z, vTarget.x) - glm::radians(90.0f);
	
	if (radian <= 0) {
		radian += glm::radians(360.0f);
	}

	const glm::vec3 vZombieFront = glm::rotate(glm::mat4(1), rotation.y, glm::vec3(0, 1, 0)) * glm::vec4(0, 0, -1, 1);

	//ターゲットが正面にいなかったら、正面にとらえるような左右に旋回.
	if (std::abs(radian - rotation.y) > frontRange) {
		const glm::vec3 vRotDir = glm::cross(vZombieFront, vTarget);
		if (vRotDir.y >= 0) {
			rotation.y += rotationSpeed * deltaTime;
			if (rotation.y >= glm::radians(360.0f)) {
				rotation.y -= glm::radians(360.0f);
			}
		}
		else {
			rotation.y -= rotationSpeed * deltaTime;
			if (rotation.y < 0) {
				rotation.y += glm::radians(360.0f);
			}
		}
	}

	//十分に接近していなければ移動する。接近していれば攻撃する.
	if (glm::length(v) > 0.5f) {
		velocity = vZombieFront * moveSpeed;
	}
	else {
		velocity = glm::vec3(0);	//接近しているのでもう移動しない.
		//定期的に攻撃状態になる.
		if (isAttacking) {
			isAttacking = false;
			attackingTimer = 5.0f;	//次の攻撃は５秒後.
		}
		else {
			attackingTimer -= deltaTime;
			if (attackingTimer <= 0) {
				isAttacking = true;
			}
		}
	}
	Actor::Update(deltaTime);
}

void TreeActor::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void BillActor::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void ItemActor::Update(float deltaTime)
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	Timer1 += window.DeltaTime();
	if (Timer1 > 30.0f) {
		health = 0;
	}
	Actor::Update(deltaTime);
}

void Item2Actor::Update(float deltaTime)
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	Timer1 += window.DeltaTime();
	if (Timer1 > 30.0f) {
		health = 0;
	}
	Actor::Update(deltaTime);
}

void Item3Actor::Update(float deltaTime)
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	Timer1 += window.DeltaTime();
	if (Timer1 > 1.0f) {
		if (health > 0) {
			health = 0;
		}
		else {
			Timer1 = 0.0f;
			health = 1;
		}
	}
	if (health < 0) {
		Timer1 = 0.0f;
		health = 1;
	}
	Actor::Update(deltaTime);
}

/**
*初期化.
*/
bool MainGameScene::Initialize() {

	random.seed(std::random_device()());

	//MeshList meshList;
	if (!meshList.Allocate()) {
		return false;
	}

	progSimple.Reset(Shader::BuildFromFile("Res/Simple.vert", "Res/Simple.frag"));
	progLighting.Reset(Shader::BuildFromFile("Res/FragmentLighting.vert", "Res/FragmentLighting.frag"));

	//OpenGLの情報をコンソールウィンドウへ出力する.
	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::cout << "Renderer:" << renderer << std::endl;
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Version:" << version << std::endl;
	const GLubyte* extensions = glGetString(GL_EXTENSIONS);
	std::cout << "Extensions:" << extensions << std::endl;

	//家のテクスチャを作成する.
	const int imageWidth = 8;
	const int imageHeight = 8;
	const uint32_t B = 0xff'00'00'00;
	const uint32_t W = 0xff'ff'ff'ff;
	const uint32_t imageData[imageWidth * imageHeight] = {
		W,W,B,W,W,W,W,W,
		W,W,B,W,W,W,W,W,
		W,W,B,W,W,W,W,W,
		B,B,B,B,B,B,B,B,
		W,W,W,W,W,W,B,W,
		W,W,W,W,W,W,B,W,
		W,W,W,W,W,W,B,W,
		B,B,B,B,B,B,B,B,
	};
	texId.Reset(Texture::CreateImage2D(imageWidth, imageHeight, imageData, GL_RGBA, GL_UNSIGNED_BYTE));
	texHouse.Reset(Texture::LoadImage2D("Res/Bill.tga"));
	texRock.Reset(Texture::LoadImage2D("Res/Iwa.tga"));
	texHuman.Reset(Texture::LoadImage2D("Res/Human.tga"));
	texBullet.Reset(Texture::LoadImage2D("Res/Bullet.tga"));
	texGround.Reset(Texture::LoadImage2D("Res/ground.tga"));
	texEnemy1.Reset(Texture::LoadImage2D("Res/enemy1.tga"));
	texEnemy2.Reset(Texture::LoadImage2D("Res/enemy2.tga"));
	texEnemy3.Reset(Texture::LoadImage2D("Res/enemy3.tga"));
	texBossenemy.Reset(Texture::LoadImage2D("Res/bossenemy.tga"));
	
	//情報表示用テクスチャを読み込む.
	texScore.Reset(Texture::LoadImage2D("Res/Score.tga"));
	for (int i = 0; i < 10; ++i) {
		const std::string filename =
			std::string("Res/Number_") + (char)('0' + i) + ".tga";
		texNumber[i].Reset(Texture::LoadImage2D(filename.c_str()));
	}
	texHP.Reset(Texture::LoadImage2D("Res/HP.tga"));
	texStageClear.Reset(Texture::LoadImage2D("Res/StageClear.tga"));
	texGameOver.Reset(Texture::LoadImage2D("Res/GameOver.tga"));
	texTree.Reset(Texture::LoadImage2D("Res/tree.tga"));
	texReload.Reset(Texture::LoadImage2D("Res/reload.tga"));
	texSiro.Reset(Texture::LoadImage2D("Res/siro.tga"));
	texsyasen.Reset(Texture::LoadImage2D("Res/syasen.tga"));
	texpause.Reset(Texture::LoadImage2D("Res/pause.tga"));
	texkill.Reset(Texture::LoadImage2D("Res/kill.tga"));
	texseseki.Reset(Texture::LoadImage2D("Res/seseki.tga"));
	texscore2.Reset(Texture::LoadImage2D("Res/score2.tga"));
	texbullet23.Reset(Texture::LoadImage2D("Res/use.tga"));
	texuse.Reset(Texture::LoadImage2D("Res/bullet23.tga"));
	texBoss.Reset(Texture::LoadImage2D("Res/Boss.tga"));
	texmugen.Reset(Texture::LoadImage2D("Res/MUGEN.tga"));

	//仮メニュー
	texLightButtonEnd.Reset(Texture::LoadImage2D("Res/EndLight.tga"));
	texButtonEnd.Reset(Texture::LoadImage2D("Res/End.tga"));
	texLightButtonStart.Reset(Texture::LoadImage2D("Res/LightStart.tga"));
	texButtonStart.Reset(Texture::LoadImage2D("Res/Start.tga"));
	///

	//ライトの設定.
	lights.ambient.color = glm::vec3(0.05f, 0.1f, 0.1f);			//環境光の色.
	lights.directional.direction = glm::normalize(glm::vec3(1, -1, -1));	//指向性ライトの方向.
	lights.directional.color = glm::vec3(1.0f, 1.0f, 1.0f);			//指向性ライトの色.
	lights.point.position[0] = glm::vec3(5, 4, 0);
	lights.point.color[0] = glm::vec3(1.0f, 0.8f, 0.4f) * 100.0f;
	const glm::vec3 tmpSpotLightDir = glm::normalize(glm::vec3(-1, -2, -2));
	lights.spot.dirAndCutOff[0] = glm::vec4(tmpSpotLightDir, std::cos(glm::radians(20.0f)));
	lights.spot.posAndInnerCutOff[0] = glm::vec4(-6, 6, 8, std::cos(glm::radians(15.0f)));
	lights.spot.color[0] = glm::vec3(0.4f, 0.8f, 1.0f) * 200.0f;

	player.Initialize(5, texHuman.Get(), 10,
		glm::vec3(8, 0, 8), glm::vec3(0), glm::vec3(1));
	player.colLocal =
	{ glm::vec3(-0.5f,0.0f,-0.5f),glm::vec3(1.0f,1.7f,1.0f) };

	playerBullets.resize(128);
	for (auto& bullet : playerBullets) {
		bullet = new BulletActor;
	}
	enemies.resize(128);
	for (auto& zombie : enemies) {
		zombie = new ZombieActor;
	}

	treeA.resize(10);
	for (auto& tree1 : treeA){
		tree1 = new TreeActor;
	}

	BillA.resize(10);
	for (auto& Bill1 : BillA) {
		Bill1 = new BillActor;
	}

	ItemA.resize(128);
	for (auto& item1 : ItemA) {
		item1 = new ItemActor;
	}

	Item2A.resize(128);
	for (auto& item12 : Item2A) {
		item12 = new Item2Actor;
	}

	Item3A.resize(128);
	for (auto& item13 : Item3A) {
		item13 = new Item3Actor;
	}

	const float treeCount = 10;		//木の本数.
	const float radius = 15;		//木を植える円の半径.

	for (float i = 0; i < treeCount; ++i) {
		const float theta = 3.14f * 2 / treeCount * i;
		const float x = std::cos(theta) * radius;
		const float z = std::sin(theta) * radius;

		Actor* tree = FindAvailableActor(treeA);
		if (tree) {
			tree->Initialize(0, texTree.Get(), 6, glm::vec3(x, 0, z), glm::vec3(0, theta * 5, 0), glm::vec3(1));
			tree->colLocal = { glm::vec3(-0.5f,0.0f,-0.5f),glm::vec3(1.0f,1.7f,1.0f) };
		}
	}

	const float houseCount = 4;		//家の数.
	const float houseRadius = 8;	//家を建てる半径.

	progLighting.BindTexture(0, texHouse.Get());

	for (float i = 0; i < houseCount; ++i)
	{
		const float x = std::cos(3.14f * 2 / houseCount * i) * houseRadius;
		const float z = std::sin(3.14f * 2 / houseCount * i) * houseRadius;

		Actor* Bill = FindAvailableActor(BillA);
		if (Bill) {
			Bill->Initialize(1, texHouse.Get(), 6, glm::vec3(x, 0, z), glm::vec3(0, 0, 0), glm::vec3(1));
			Bill->colLocal = { glm::vec3(-0.5f,0.0f,-0.5f),glm::vec3(1.0f,1.7f,1.0f) };
		}
	}

	//仮メニュー
	BlinkTimer = 0.5f;
	ButtonStart_f = true;
	ButtonEnd_f = false;
	///

	return true;
}

/**
*入力の反映.
*/
void MainGameScene::ProcessInput() {
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (state == State::play) {

		//プレイヤーを移動する.
		player.velocity = glm::vec3(0);
			if (window.IsKeyPressed(GLFW_KEY_A)) {
				player.velocity.x = -1;
			}
			else if (window.IsKeyPressed(GLFW_KEY_D)) {
				player.velocity.x += 1;
			}
			if (window.IsKeyPressed(GLFW_KEY_W)) {
				player.velocity.z = -1;
			}
			else if (window.IsKeyPressed(GLFW_KEY_S)) {
				player.velocity.z = 1;
			}

		if (player.velocity.x || player.velocity.z) {
			player.velocity = glm::normalize(player.velocity);
			//ショットボタンが押されていなければ方向転換.
			if (!window.IsKeyPressed(GLFW_KEY_SPACE)) {
				player.rotation.y = std::atan2(-player.velocity.z, player.velocity.x);
				player.rotation.y -= glm::radians(90.0f);
			}
			const float speed = 10.0f;			//移動速度
			if (window.IsKeyPressed(GLFW_KEY_LEFT_SHIFT)) {			//SHIFTを押している間ダッシュする
				player.velocity *= speed * 2;
			}

			else {													//SHIFTを押してなければ歩く
				player.velocity *= speed;
			}
		}

		//ショットボタンが押されていたら弾を発射.
		if (window.IsKeyPressed(GLFW_KEY_SPACE) && Bulletremaining > 0 && Mg_f == true) {
			if (playerBulletTimer <= 0) {
				Actor* bullet = FindAvailableActor(playerBullets);
				if (bullet) {
					const int meshId = 6;		//弾のメッシュID.
					const float speed = 60.0f;	//弾の移動速度(m/秒).
					const glm::mat4 matRotY =
						glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
					bullet->Initialize(meshId, texEnemy3.Get(), 1, player.position, player.rotation, glm::vec3(1));
					bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
					bullet->colLocal = { glm::vec3(-0.25f,-0.25f,-0.25f),glm::vec3(1,1,1) };
					playerBulletTimer = 1.0f / 10.0f;	//秒間８連射.
					Bulletremaining -= 1;
					useBullet += 1;
				}
			}
		}

		else if (window.IsKeyPressed(GLFW_KEY_SPACE) && Bulletremaining > 0 && Sg_f == true) {
			for (int i = 0; i < 5; i++) {
				if (playerBulletTimer <= 0) {
					Actor* bullet = FindAvailableActor(playerBullets);
					if (bullet) {
						const int meshId = 6;		//弾のメッシュID.
						const float speed = 30.0f;	//弾の移動速度(m/秒).
						const int x[] = { 0,10,-10,20,-20 };
						const glm::mat4 matRotY =
							glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
						bullet->Initialize(meshId, texBullet.Get(), 1, player.position, player.rotation, glm::vec3(1));
						bullet->velocity = matRotY * glm::vec4(x[i], 0, -speed, 1);
						bullet->colLocal = { glm::vec3(-0.25f,-0.25f,-0.25f),glm::vec3(1,1,1) };
						useBullet += 1;
						Bulletremaining -= 1;
					}
				}
			}
			playerBulletTimer = 1.0f / 1.0f;	//秒間８連射.
		}

		else if (window.IsKeyPressed(GLFW_KEY_SPACE) && Be_f == true) {
			Actor* bullet = FindAvailableActor(playerBullets);
			if (bullet) {
				const int meshId = 6;		//弾のメッシュID.
				const float speed = 100.0f;	//弾の移動速度(m/秒).
				const glm::mat4 matRotY =
					glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
				bullet->Initialize(6, texEnemy2.Get(), 1, player.position, player.rotation, glm::vec3(1));
				bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
				bullet->colLocal =
				{ glm::vec3(-0.25f,-0.25f,-0.25f),glm::vec3(1,1,1) };
				useBullet += 1;
			}
		}

		else if (window.IsKeyPressed(GLFW_KEY_SPACE) && Bulletremaining > 0) {
				if (playerBulletTimer <= 0) {
					Actor* bullet = FindAvailableActor(playerBullets);
					if (bullet) {
						const int meshId = 6;		//弾のメッシュID.
						const float speed = 40.0f;	//弾の移動速度(m/秒).
						const glm::mat4 matRotY =
							glm::rotate(glm::mat4(1), player.rotation.y, glm::vec3(0, 1, 0));
						bullet->Initialize(meshId, texBullet.Get(), 1, player.position, player.rotation, glm::vec3(1));
						bullet->velocity = matRotY * glm::vec4(0, 0, -speed, 1);
						bullet->colLocal = { glm::vec3(-0.25f,-0.25f,-0.25f),glm::vec3(1,1,1) };
						playerBulletTimer = 1.0f / 8.0f;	//秒間８連射.
						Bulletremaining -= 1;
						useBullet += 1;
					}
				}
		}

		else if (window.IsKeyPressed(GLFW_KEY_R) && !window.IsKeyPressed(GLFW_KEY_SPACE)) {				//リロード
			Bulletremaining = MaxBullet;
		}

		else {
			playerBulletTimer = 0.0f;
		}
		if (window.IsKeyPressed(GLFW_KEY_E)) {						//一時停止
			if (player.health >= 0 && enemyKilled <= enemyTotal) {
				player.velocity.x = player.velocity.z = 0;
				state = State::MenuScene;
			}
		}
	}
	else if (state == State::stageClear) {
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
			++stageNo;
			player.position = glm::vec3(8, 0, 8);
			state = State::play;

			//より多くの敵を、より早く出現させる.
			enemyTotal = 10 + stageNo * 10;
			enemyLeft = enemyTotal;
			enemyKilled = 0;
			enemyPoppingInterval = 15.0f - (float)(stageNo - 1) * 2.0f;
			if (enemyPoppingInterval < 5.0f) {
				enemyPoppingInterval = 5.0f;
			}
			enemyPoppingTimer = 0;
			enemyPoppingTimer2 = 0;
			enemyPoppingTimer3 = 0;

			//ボスの体力をより多くする
			bossHP = 100 + stageNo * 20;
			bossenemyLeft = 1;
			boss_f = false;
			stagecrea_f = false;

			itemLeft = 1;				//未登場のアイテムの数.アイテムを出現させるたびに減少していく.
			itemPoppingTimer = 20.0f;		//アイテム出現タイマー.
			itemPoppingInterval = 20.0f;	//アイテムの出現間隔.

			item2Left = 1;				//未登場のアイテムの数.アイテムを出現させるたびに減少していく.
			item2PoppingTimer = 30.0f;		//アイテム出現タイマー.
			item2PoppingInterval = 30.0f;	//アイテムの出現間隔.

			item3Left = 1;				//未登場のアイテムの数.アイテムを出現させるたびに減少していく.
			item3PoppingTimer = 60.0f;		//アイテム出現タイマー.
			item3PoppingInterval = 60.0f;	//アイテムの出現間隔.

			//敵の移動速度を上げる.
			enemySpeed = 1.0f + (float)(stageNo - 1) * 0.2f;

			//武器を初期武器にする
			Mg_f = false;
			Sg_f = false;
			Be_f = false;
			MaxBullet = 10;
			if (Bulletremaining > 10) {
				Bulletremaining = 10;
			}
		}
	}

	else if (state == State::MenuScene && window.IsKeyPressed(GLFW_KEY_0)) {
		Mg_f = false;
		Sg_f = false;
		Be_f = false;
		HP_f = false;
		MaxBullet = 10;
		if (Bulletremaining > 10) {
			Bulletremaining = 10;
		}
	}

	else if (state == State::MenuScene && window.IsKeyPressed(GLFW_KEY_1)) {
		Mg_f = true;
		Sg_f = false;
		Be_f = false;
		MaxBullet = 30;
	}

	else if (state == State::MenuScene && window.IsKeyPressed(GLFW_KEY_2)) {
		Mg_f = false;
		Sg_f = true;
		Be_f = false;
		MaxBullet = 25;
	}

	else if (state == State::MenuScene && window.IsKeyPressed(GLFW_KEY_3)) {
		Mg_f = false;
		Sg_f = false;
		Be_f = true;
	}

	else if (state == State::MenuScene && window.IsKeyPressed(GLFW_KEY_4)) {
		Mg_f = false;
		Sg_f = false;
		Be_f = false;
		HP_f = true;
	}

	else if (state == State::MenuScene) {
	//仮メニュー
		//はじまる
		if (window.IsKeyPressed(GLFW_KEY_W) || window.IsKeyPressed(GLFW_KEY_UP)) {
			ButtonEnd_f = false;
			ButtonStart_f = true;
		}
		//おわる
		if (window.IsKeyPressed(GLFW_KEY_S) || window.IsKeyPressed(GLFW_KEY_DOWN)) {
			ButtonEnd_f = true;
			ButtonStart_f = false;
		}
			//はじまるボタンが選ばれているときにENTERを押すとゲームスタート
			if (ButtonStart_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
				state = State::play;
				Mg_f = false;
				Sg_f = true;
				Be_f = false;
				MaxBullet = 25;
			}
			//おわるボタンが選ばれているときにENTERを押すとゲーム終了
			else if (ButtonEnd_f && window.IsKeyPressed(GLFW_KEY_ENTER)) {
				//NextScene("TitleScene");
				state = State::play;
				Mg_f = false;
				Sg_f = false;
				Be_f = true;
			}
	}

	else if(state == State::gameOver){
		player.velocity.x = player.velocity.z = 0;
		if (window.IsKeyPressed(GLFW_KEY_ENTER)) {
			//state = State::ranking;
			NextScene("GameoverScene");
		}
	}
	else if (state == State::ranking) {
	player.velocity.x = player.velocity.z = 0;
	if (window.IsKeyPressed(GLFW_KEY_SPACE)) {
		NextScene("GameoverScene");
		}
	}
}

/**
*状態の更新.
*/
void MainGameScene::Update() {

	const float deltaTime = (float)GLFWEW::Window::Instance().DeltaTime();

	//モデルのY軸回転角を更新.
	angleY += glm::radians(1.0f);
	if (angleY > glm::radians(360.0f)) {
		angleY -= glm::radians(360.0f);
	}

	//視点の位置を更新.
	const glm::vec3 viewOffset(0, 5, 20);
	viewPos = player.position + viewOffset;

	//プレイヤーの状態を更新.
	player.Update(deltaTime);

	//プレイヤーの弾の発射タイマーを更新.
	if (playerBulletTimer > 0) {
		playerBulletTimer -= deltaTime;
	}

	//プレイヤーの弾の状態を更新.
	UpdateActorList(playerBullets, deltaTime);

	UpdateActorList(treeA,deltaTime);

	UpdateActorList(BillA, deltaTime);

	UpdateActorList(ItemA, deltaTime);
	if (itemLeft > 0) {
		if (itemPoppingTimer >= 0) {
			itemPoppingTimer -= deltaTime;
		}
		else {
			itemPoppingTimer += itemPoppingInterval;

				int popCount1 = 1;	//同時に発生させるアイテムの最大数.
				if (itemLeft < popCount1) {
					popCount1 = itemLeft;
				}
				itemLeft -= popCount1;

				//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
				std::uniform_int_distribution<int> rangeBase(-15, 15);
				std::uniform_int_distribution<int> range(-5, 5);

				glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
				for (int i = 0; i < popCount1; ++i) {
					ItemActor* item1 = (ItemActor*)FindAvailableActor(ItemA);
					if (item1) {
						glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
						item1->Initialize(6, texEnemy3.Get(), 1, pos, glm::vec3(0), glm::vec3(1));
						item1->colLocal =
						{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
					}
					else {
						++itemLeft;	//出現できなかった場合、残り数を増やして次回の出現を待つ.
					}
				}
		}
	}

	UpdateActorList(Item2A, deltaTime);
	if (item2Left > 0) {
		if (item2PoppingTimer >= 0) {
			item2PoppingTimer -= deltaTime;
		}
		else {
			item2PoppingTimer += item2PoppingInterval;

			int popCount1 = 1;	//同時に発生させるアイテムの最大数.
			if (item2Left < popCount1) {
				popCount1 = item2Left;
			}
			item2Left -= popCount1;

			//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
			std::uniform_int_distribution<int> rangeBase(-15, 15);
			std::uniform_int_distribution<int> range(-5, 5);

			glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
			for (int i = 0; i < popCount1; ++i) {
				Item2Actor* item12 = (Item2Actor*)FindAvailableActor(Item2A);
				if (item12) {
					glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
					item12->Initialize(6, texEnemy1.Get(), 1, pos, glm::vec3(0), glm::vec3(1));
					item12->colLocal =
					{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
				}
				else {
					++item2Left;	//出現できなかった場合、残り数を増やして次回の出現を待つ.
				}
			}
		}
	}

	UpdateActorList(Item3A, deltaTime);
	if (item3Left > 0) {
		if (item3PoppingTimer >= 0) {
			item3PoppingTimer -= deltaTime;
		}
		else {
			item3PoppingTimer += item3PoppingInterval;

			int popCount1 = 1;	//同時に発生させるアイテムの最大数.
			if (item3Left < popCount1) {
				popCount1 = item3Left;
			}
			item3Left -= popCount1;

			//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
			std::uniform_int_distribution<int> rangeBase(-15, 15);
			std::uniform_int_distribution<int> range(-5, 5);

			glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
			for (int i = 0; i < popCount1; ++i) {
				Item2Actor* item13 = (Item2Actor*)FindAvailableActor(Item3A);
				if (item13) {
					glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
					item13->Initialize(6, texBossenemy.Get(), 1, pos, glm::vec3(0), glm::vec3(1));
					item13->colLocal =
					{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
				}
				else {
					++item3Left;	//出現できなかった場合、残り数を増やして次回の出現を待つ.
				}
			}
		}
	}

	//ゾンビの状態を更新.
	if (state != State::MenuScene) {
		UpdateActorList(enemies, deltaTime);
	}

	//ゾンビの発生.
	if (enemyLeft > 0 && state != State::MenuScene || bossenemyLeft > 0 && state != State::MenuScene) {

		//ボス戦
		if (boss_f == true) {
			enemyPoppingTimer += enemyPoppingInterval;

			int popCount = 1;	//同時に発生させるゾンビの最大数.
			if (bossenemyLeft < popCount) {
				popCount = bossenemyLeft;
			}
			bossenemyLeft -= popCount;

			//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
			std::uniform_int_distribution<int> rangeBase(-15, 15);
			std::uniform_int_distribution<int> range(-5, 5);

			glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
			for (int i = 0; i < popCount; ++i) {
				ZombieActor* zombie = (ZombieActor*)FindAvailableActor(enemies);
				if (zombie) {
					glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
					zombie->Initialize(8, texBossenemy.Get(), bossHP, pos, glm::vec3(0), glm::vec3(1));
					zombie->colLocal =
					{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
					zombie->target = &player;
					zombie->baseSpeed  = enemySpeed * 1.2;
				}
			}
		}
		else {
			if (stageNo >= 3) {
				if (enemyPoppingTimer3 >= 0) {
					enemyPoppingTimer3 -= deltaTime;
				}
				else {
					enemyPoppingTimer3 += enemyPoppingInterval;

					int popCount = 0;	//同時に発生させるゾンビの最大数.
					if (stageNo >= 5) {
						popCount = 8;	//同時に発生させるゾンビの最大数.
					}
					else {
						popCount = 5;	//同時に発生させるゾンビの最大数.
					}

					if (enemyLeft < popCount) {
						popCount = enemyLeft;
					}
					enemyLeft -= popCount;

					//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
					std::uniform_int_distribution<int> rangeBase(-15, 15);
					std::uniform_int_distribution<int> range(-5, 5);

					glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
					for (int i = 0; i < popCount; ++i) {
						ZombieActor* zombie = (ZombieActor*)FindAvailableActor(enemies);
						if (zombie) {
							glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
							zombie->Initialize(5, texEnemy3.Get(), 15, pos, glm::vec3(0), glm::vec3(1));

							zombie->colLocal =
							{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
							zombie->target = &player;
							zombie->baseSpeed = enemySpeed;
						}
						else {
							++enemyLeft;	//出現できなかった場合、残り数を増やして次回の出現を待つ.
						}
					}
				}
			}
			if (stageNo >= 2) {
				if (enemyPoppingTimer2 >= 0) {
					enemyPoppingTimer2 -= deltaTime;
				}
				else {
					enemyPoppingTimer2 += enemyPoppingInterval;

					int popCount = 0;	//同時に発生させるゾンビの最大数.

					if (stageNo >= 5) {
						popCount = 5;	//同時に発生させるゾンビの最大数.
					}
					else if (stageNo >= 3) {
						popCount = 8;	//同時に発生させるゾンビの最大数.
					}
					else {
						popCount = 5;	//同時に発生させるゾンビの最大数.
					}

					if (enemyLeft < popCount) {
						popCount = enemyLeft;
					}
					enemyLeft -= popCount;

					//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
					std::uniform_int_distribution<int> rangeBase(-15, 15);
					std::uniform_int_distribution<int> range(-5, 5);

					glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
					for (int i = 0; i < popCount; ++i) {
						ZombieActor* zombie = (ZombieActor*)FindAvailableActor(enemies);
						if (zombie) {
							glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
							zombie->Initialize(5, texEnemy2.Get(), 10, pos, glm::vec3(0), glm::vec3(1));

							zombie->colLocal =
							{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
							zombie->target = &player;
							zombie->baseSpeed = enemySpeed;
						}
						else {
							++enemyLeft;	//出現できなかった場合、残り数を増やして次回の出現を待つ.
						}
					}
				}
			}
			if (enemyPoppingTimer >= 0) {
				enemyPoppingTimer -= deltaTime;
			}
			else {
				enemyPoppingTimer += enemyPoppingInterval;

				int popCount = 0;	//同時に発生させるゾンビの最大数.

				if (stageNo >= 5) {
					popCount = 2;	//同時に発生させるゾンビの最大数.
				}
				else if (stageNo >= 2) {
					popCount = 5;	//同時に発生させるゾンビの最大数.
				}
				else{
					popCount = 10;	//同時に発生させるゾンビの最大数.
				}
				if (enemyLeft < popCount) {
					popCount = enemyLeft;
				}
				enemyLeft -= popCount;

				//マップの前後左右15ｍの範囲で出現ポイントを決め、そこを中心として前後左右5ｍの範囲に出現.
				std::uniform_int_distribution<int> rangeBase(-15, 15);
				std::uniform_int_distribution<int> range(-5, 5);

				glm::vec3 posBase(rangeBase(random), 0, rangeBase(random));
				for (int i = 0; i < popCount; ++i) {
					ZombieActor* zombie = (ZombieActor*)FindAvailableActor(enemies);
					if (zombie) {
						glm::vec3 pos = posBase + glm::vec3(range(random), 0, range(random));
						zombie->Initialize(5,texEnemy1.Get(), 5, pos, glm::vec3(0), glm::vec3(1));

						zombie->colLocal =
						{ glm::vec3(-0.5f,0,-0.5f),glm::vec3(1,1.8f,1) };
						zombie->target = &player;
						zombie->baseSpeed = enemySpeed;
					}
					else {
						++enemyLeft;	//出現できなかった場合、残り数を増やして次回の出現を待つ.
					}
				}
			}
		}
	}

	//プレイヤーの弾と敵の衝突判定.
	DetectCollision(playerBullets, enemies, [&](Actor& bullet, Actor& zombie) {
		if (HP_f == true) {
			zombie.health = 0;
			bullet.health = 0;
		}
		else {
			zombie.health -= 1;
			bullet.health = 0;
		}
		if (zombie.health <= 0) {
			if (boss_f == true) {
				score += 1000;
				++enemyKilledAll;
				stagecrea_f = true;
			}
			else {
				score += 200;
				++enemyKilled;
				++enemyKilledAll;
			}
		}
		else {
			score += 10;
		}
	});

	//木と弾の衝突判定
	DetectCollision(playerBullets, treeA, [&](Actor& bullet, Actor& tree) {
		bullet.health = 0;
	});

	//ビルと弾の衝突判定
	DetectCollision(playerBullets, BillA, [&](Actor& bullet, Actor& bill) {
		bullet.health = 0;
	});

	//アイテムと弾の衝突判定
	DetectCollision(playerBullets, ItemA, [&](Actor& bullet, Actor& item) {
		bullet.health = 0;
		item.health = 0;
		Mg_f = true;
		Sg_f = false;
		Be_f = false;
		MaxBullet = 30;
	});

	//アイテムと弾の衝突判定
	DetectCollision(playerBullets, Item2A, [&](Actor& bullet, Actor& item2) {
		bullet.health = 0;
		item2.health = 0;
		Sg_f = true;
		Mg_f = false;
		Be_f = false;
		MaxBullet = 25;
	});

	//アイテムと弾の衝突判定
	DetectCollision(playerBullets, Item3A, [&](Actor& bullet, Actor& item3) {
		bullet.health = 0;
		item3.health = 0;
		Sg_f = false;
		Mg_f = false;
		Be_f = true;
	});

	if (state == State::play && enemyKilled >= enemyTotal && stagecrea_f == false) {
		boss_f = true;
	}
	//ステージクリア判定.
	else if (state == State::play && enemyKilled >= enemyTotal && stagecrea_f == true) {
		state = State::stageClear;
	}

	//ゾンビの攻撃.
	for (auto& actor : enemies) {
		if (actor->health <= 0) {
			continue;
		}
		//ターゲットの体力が０以下なら攻撃しない.
		ZombieActor* zombie = (ZombieActor*)actor;
		if (zombie->target->health <= 0) {
			continue;
		}
		if (zombie->isAttacking) {
			//ゾンビの正面左右45度の範囲にターゲットがいたら、ターゲットの体力を１減らす.
			const glm::vec3 vFront =
				glm::rotate(glm::mat4(1), zombie->rotation.y, glm::vec3(0, 1, 0))* glm::vec4(0, 0, -1, 1);
			const glm::vec3 vTarget = zombie->target->position - zombie->position;
			const float angle = std::acos(glm::dot(vFront, glm::normalize(vTarget)));
			if (std::abs(angle) < glm::radians(45.0f) && glm::length(vTarget) < 1.5f) {
				if (boss_f == true) {
					--zombie->target->health -= 4;
				}
				else {
					--zombie->target->health;
				}
			}
		}
	}
	//ゲームオーバー判定.
	if (state == State::play && player.health <= 0) {
		state = State::gameOver;
	}
}

/**
*描画.
*/
void MainGameScene::Render(){
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	const float deltaTime = (float)window.DeltaTime();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	progLighting.Use();

	//座標変換行列を作成してシェーダーに転送する.

	const glm::mat4x4 matProj =
		glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 500.0f);
	const glm::mat4x4 matView =
		glm::lookAt(viewPos, player.position, glm::vec3(0, 1, 0));
	progLighting.SetViewProjectionMatrix(matProj * matView);

	meshList.BindVertexArraay();

	//光源を設定する.
	progLighting.SetLightList(lights);

	progLighting.BindTexture(0, player.texture);
	progLighting.Draw(meshList.Get(player.mesh),
		player.position, player.rotation, player.scale);

	RenderActorList(playerBullets, progLighting, meshList);
	RenderActorList(enemies, progLighting, meshList);

	RenderActorList(treeA, progLighting, meshList);

	RenderActorList(BillA, progLighting, meshList);

	RenderActorList(ItemA, progLighting, meshList);

	RenderActorList(Item2A, progLighting, meshList);

	RenderActorList(Item3A, progLighting, meshList);

	progLighting.BindTexture(0, texGround.Get());			//床
	progLighting.Draw(meshList.Get(7),
		glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1));

	//ポイント・ライトの位置が分かるように適当なモデルを表示.
	{
		//ライトの位置に木のモデルを表示.
		progLighting.BindTexture(0, texId.Get());
		for (int i = 0; i < 8; ++i) {
			const glm::mat4 matModelT = glm::translate(glm::mat4(1), lights.point.position[i]);
			const glm::mat4 matModelR = glm::rotate(glm::mat4(1), angleY, glm::vec3(0, 1, 0));
			const glm::mat4 matModelS = glm::scale(glm::mat4(1), glm::vec3(1, -0.25f, 1));
			progLighting.Draw(meshList.Get(0),
				lights.point.position[i], glm::vec3(0, angleY, 0), glm::vec3(1, -0.25f, 1));
		}

		//情報を表示.
		{
			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			progSimple.Use();

			//正射影で描画するように行列を設定.
			const glm::mat4x4 matProj = glm::ortho(
				-400.0f, 400.0f, -300.0f, 300.0f, 1.0f, 500.0f);
			
			const glm::mat4x4 matView = glm::lookAt(
				glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
			progSimple.SetViewProjectionMatrix(matProj * matView);

			const Mesh& planeMeshId = meshList.Get(4);

			//スコアを表示.
			progSimple.BindTexture(0, texScore.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-110, 270, 0), glm::vec3(0), glm::vec3(96, 32, 1));
			const int maxScoreDigits = 8;	//表示する桁数.
			int tmpScore = score;
			for (int i = 0; i < maxScoreDigits; ++i) {
				const int posX = -22 + 32 * (maxScoreDigits - i);
				const int number = tmpScore % 10;
				tmpScore /= 10;
				progSimple.BindTexture(0, texNumber[number].Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(posX, 270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
			}

			if (Be_f == true) {
				progSimple.BindTexture(0, texmugen.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(350, -260, 0), glm::vec3(0), glm::vec3(32, 32, 1));
			}
			else {
				//残弾数
				progSimple.BindTexture(0, texsyasen.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(310, -270, 0), glm::vec3(0), glm::vec3(15, 32, 1));
				const int maxBZ = 2;	//表示する桁数.
				int tmpBZ = Bulletremaining;
				for (int i = 0; i < maxBZ; ++i) {
					const int posX = 210 + 32 * (maxBZ - i);
					const int number = tmpBZ % 10;
					tmpBZ /= 10;
					progSimple.BindTexture(0, texNumber[number].Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(posX, -270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
				}

				//MAX弾数
				const int maxB = 2;	//表示する桁数.
				int tmpB = MaxBullet;
				for (int i = 0; i < maxB; ++i) {
					const int posX = 310 + 32 * (maxB - i);
					const int number = tmpB % 10;
					tmpB /= 10;
					progSimple.BindTexture(0, texNumber[number].Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(posX, -270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
				}
			}

			//リロードを表示
			if (Bulletremaining <= 0) {
				progSimple.BindTexture(0, texReload.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(0, -50, 0), glm::vec3(0), glm::vec3(100,30, 1));
			}

			//プレイヤーの画像表示.
			progSimple.BindTexture(0, texHuman.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-335, -245, 0), glm::vec3(0), glm::vec3(64, 64, 1));

			//プレイヤーの体力を表示.
			progSimple.BindTexture(0, texHP.Get());
			progSimple.Draw(planeMeshId,
				glm::vec3(-220, -215, 0), glm::vec3(0), glm::vec3(48, 32, 1));
			const int maxHealthDigits = 2;	//表示する桁数.
			int tmpHealth = player.health;
			for (int i = 0; i < maxHealthDigits; ++i) {
				const int posX = -200 + 32 * (maxHealthDigits - i);
				const int number = tmpHealth % 10;
				tmpHealth /= 10;
				progSimple.BindTexture(0, texNumber[number].Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(posX, -215, 0), glm::vec3(0), glm::vec3(32, 32, 1));
			}

			//ステージ番号を表示.
			if (boss_f == true) {
				progSimple.BindTexture(0, texBoss.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(-300, 270, 0), glm::vec3(0), glm::vec3(100, 32, 1));
			}
			else{
			const int maxStageDigits = 1;	//表示する桁数.
			for (int i = 0; i < maxStageDigits; ++i) {
				const int posX = -400 + 32 * (maxStageDigits - i);
				const int number = stageNo;
				progSimple.BindTexture(0, texNumber[number].Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(posX, 270, 0), glm::vec3(0), glm::vec3(32, 32, 1));
				}
			}
			//ステージクリア・ゲームオーバー表示.
			if (state == State::stageClear) {
				progSimple.BindTexture(0, texStageClear.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(0), glm::vec3(0), glm::vec3(350, 60, 1));
			}
			else if (state == State::gameOver) {
				progSimple.BindTexture(0, texGameOver.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(0), glm::vec3(0), glm::vec3(300, 60, 1));
			}

//メニュー画面
			else if (state == State::MenuScene){
				progSimple.BindTexture(0,texpause.Get());
				progSimple.Draw(planeMeshId,
				glm::vec3(-300, 120, 0), glm::vec3(0), glm::vec3(100, 32, 1));
				/*
				*はじめるボタン青
				*/
				if (ButtonStart_f) {
					//点滅
					if (BlinkTimer > 0.0f) {
						progSimple.BindTexture(0, texLightButtonStart.Get());
						progSimple.Draw(planeMeshId,
							glm::vec3(-300, 70, 0), glm::vec3(0), glm::vec3(100, 32, 1));
						BlinkTimer -= window.DeltaTime();
					}
					else {
						progSimple.BindTexture(0, texButtonStart.Get());
						progSimple.Draw(planeMeshId,
							glm::vec3(-300, 70, 0), glm::vec3(0), glm::vec3(100, 32, 1));
						BlinkTimer -= window.DeltaTime();
						if (BlinkTimer < -0.5) {
							BlinkTimer = 0.5;
						}
					}
				}
				//はじめるボタン黒
				else {
					progSimple.BindTexture(0, texButtonStart.Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(-300, 70, 0), glm::vec3(0), glm::vec3(100, 32, 1));
				}


				/*
				*おわるボタン青
				*/
				if (ButtonEnd_f) {
					//点滅
					if (BlinkTimer > 0.0f) {
						progSimple.BindTexture(0, texLightButtonEnd.Get());
						progSimple.Draw(planeMeshId,
							glm::vec3(-300, 20, 0), glm::vec3(0), glm::vec3(100, 32, 1));
						BlinkTimer -= window.DeltaTime();
					}
					else {
						progSimple.BindTexture(0, texButtonEnd.Get());
						progSimple.Draw(planeMeshId,
							glm::vec3(-300, 20, 0), glm::vec3(0), glm::vec3(100, 32, 1));
						BlinkTimer -= window.DeltaTime();
						if (BlinkTimer < -0.5) {
							BlinkTimer = 0.5;
						}
					}
				}
				//おわるボタン黒
				else {
					progSimple.BindTexture(0, texButtonEnd.Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(-300, 20, 0), glm::vec3(0), glm::vec3(100, 32, 1));
				}
			}

			else if (state == State::ranking) {							//ランキング
				progSimple.BindTexture(0, texSiro.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(0, 0, 0), glm::vec3(0), glm::vec3(400, 300, 1));

				progSimple.BindTexture(0, texseseki.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(0, 250, 0), glm::vec3(0), glm::vec3(100, 50, 1));

				//使用した球数
				progSimple.BindTexture(0, texuse.Get());
				progSimple.Draw(meshList.Get(4),
					glm::vec3(-260, -170, 0), glm::vec3(0), glm::vec3(120, 50, 1));
				const int useBDigits = 6;	//表示する桁数.
				int tmpBAll = useBullet;
				for (int i = 0; i < useBDigits; ++i) {
					const int posX = -140 + 60 * (useBDigits - i);
					const int number = tmpBAll % 10;
					tmpBAll /= 10;
					progSimple.BindTexture(0, texNumber[number].Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(posX, -170, 0), glm::vec3(0), glm::vec3(60, 50, 1));
				}

				//スコアを表示.
				progSimple.BindTexture(0, texscore2.Get());
				progSimple.Draw(planeMeshId,
					glm::vec3(-260, 130, 0), glm::vec3(0), glm::vec3(120, 50, 1));
				const int maxScoreDigits = 8;	//表示する桁数.
				int tmpScore = score;
				for (int i = 0; i < maxScoreDigits; ++i) {
					const int posX = -140 + 60 * (maxScoreDigits - i);
					const int number = tmpScore % 10;
					tmpScore /= 10;
					progSimple.BindTexture(0, texNumber[number].Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(posX, 130, 0), glm::vec3(0), glm::vec3(60, 50, 1));

					//敵の合計kill数を表示.
					progSimple.BindTexture(0, texkill.Get());
					progSimple.Draw(planeMeshId,
						glm::vec3(-260, -20, 0), glm::vec3(0), glm::vec3(120, 50, 1));
					const int maxKillDigits = 6;	//表示する桁数.
					int tmpKillAll = enemyKilledAll;
					for (int i = 0; i < maxKillDigits; ++i) {
						const int posX = -140 + 60 * (maxKillDigits - i);
						const int number = tmpKillAll % 10;
						tmpKillAll /= 10;
						progSimple.BindTexture(0, texNumber[number].Get());
						progSimple.Draw(planeMeshId,
							glm::vec3(posX, -20, 0), glm::vec3(0), glm::vec3(60, 50, 1));
					}
				}
			}
		}
	}
}

/**
*終了.
*/
void MainGameScene::Finalize() {
	ClearActorList(playerBullets);
	ClearActorList(enemies);
	ClearActorList(treeA);
	ClearActorList(BillA);
	ClearActorList(ItemA);
	ClearActorList(Item2A);
	ClearActorList(Item3A);
}
