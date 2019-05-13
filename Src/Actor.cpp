/**
*@file Actor.cpp
*/
#include"Actor.h"
#include<algorithm>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>

/**
*アクターを初期化する.
*
*@param mesh	アクターとして表示するメッシュID.
*@param tex		メッシュに貼るテクスチャID.
*@param hp		アクターの耐久力.
*@param pos		メッシュを表示する座標.
*@param rot		メッシュを表示する回転角度.
*@param scale	メッシュを表示する大きさ.
*/

void Actor::Initialize(int mesh, GLuint tex, int hp,
	const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) {

	this->mesh = mesh;
	texture = tex;
	position = pos;
	rotation = rot;
	this->scale = scale;
	health = hp;
}

	/**
	*アクターを破棄する.
	*/
	void Actor::Finalize() {

		health = 0;
	}
	
	/**
	*アクターを更新.
	*
	*@param deltaTime	経過時間.
	*/

	void Actor::Update(float deltaTime) {

		position += velocity * deltaTime;
		colWorld.origin = colLocal.origin + position;
		colWorld.size = colLocal.size;
	}

	/**
	*利用可能なアクターを取得する.
	*
	*@param actorList	検索対象のアクターリスト.
	*
	*@retval			検索対象のアクターのポインタ.
	*					検索対象のアクターが見つからなければnullptr.
	*/

	Actor* FindAvailableActor(std::vector<Actor*>& actorList) {

		for (auto& actor : actorList) {
			if (actor && actor->health <= 0) {

				return actor;
			}
		}
		return nullptr;
	}

	/**
	*アクターの状態を更新する.
	*
	*@param actorList	更新するアクターリスト.
	*@param deltaTime	前回の更新からの経過時間.
	*/

	void UpdateActorList(std::vector<Actor*>& actorList, float deltaTime) {

		for (auto& actor : actorList) {
			if (actor && actor->health > 0) {

				actor->Update(deltaTime);
			}
		}
	}

	/**
	*アクターを描画する.
	*
	*@param actorList	描画するアクターのリスト.
	*@param shader		描画に使用するシェーダ・オブジェクト.
	*@param meshList	描画に使用するメッシュリスト.
	*
	*事前にShader::Use(),Shader::SetViewProjectionMatrix()などの関数を実行しておくこと.
	*/

	void RenderActorList(std::vector<Actor*>& actorList,
		Shader::Program& shader, MeshList& meshList) {

		for (auto& actor : actorList) {
			if (actor && actor->health > 0) {
				shader.BindTexture(0, actor->texture);
				shader.Draw(meshList.Get(actor->mesh),
					actor->position, actor->rotation, actor->scale);
			}
		}
	}

	/**
	*アクターリストを空にする.
	*
	*@param actorList	空にするアクターリスト.
	*/
	void ClearActorList(std::vector<Actor*>& actorList) {

		for (auto& actor : actorList) {
			delete actor;
		}
		actorList.clear();
	}

	/**
	*2つの直方体の衝突判定を調べる.
	*
	*@param lhs	直方体その１.
	*@param rhs	直方体その２.
	*
	*@retval true	衝突している.
	*@retval false	衝突していない.
	*/

	bool DetectCollision(const Actor& lhs, const Actor& rhs) {

		return
			lhs.colWorld.origin.x < rhs.colWorld.origin.x + rhs.colWorld.size.x &&
			lhs.colWorld.origin.x + lhs.colWorld.size.x > rhs.colWorld.origin.x &&
			lhs.colWorld.origin.y < rhs.colWorld.origin.y + rhs.colWorld.size.y &&
			lhs.colWorld.origin.y + lhs.colWorld.size.y > rhs.colWorld.origin.y &&
			lhs.colWorld.origin.z < rhs.colWorld.origin.z + rhs.colWorld.size.z &&
			lhs.colWorld.origin.z + lhs.colWorld.size.z > rhs.colWorld.origin.z;
	}

	/**
	*2つのグループ間で衝突判定を行う.
	*
	*@param va		グループA.
	*@param vb		グループB.
	*@param func	A-B間の衝突を処理する関数.
	*/

	void DetectCollision(std::vector<Actor*>& va, std::vector<Actor*>& vb,
		CollisionHandlerType func) {

		for (auto& a : va) {
			if (a->health <= 0) {
				continue;
			}
			for (auto& b : vb) {
				if (b->health <= 0) {
					continue;
				}
				if (DetectCollision(*a, *b)) {
					func(*a, *b);
					if (a->health <= 0) {
						break;
					}
				}
			}
		}
	}