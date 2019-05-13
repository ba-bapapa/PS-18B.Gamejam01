/**
*@file Shader.h
*/

#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED
#include<GL/glew.h>
#include<glm/vec3.hpp>
#include<glm/mat4x4.hpp>

struct Mesh;

namespace Shader {

	GLuint Build(const GLchar* vsCode, const GLchar* fsCode);
	GLuint BuildFromFile(const char* vsPath, const char* fsPath);

	/**
	*環境光.
	*/
	struct AmbientLight {
		glm::vec3 color;
	};

	/**
	*指向性ライト.
	*/
	struct DirectionalLight {

		glm::vec3 direction;
		glm::vec3 color;
	};

	/**
	*ポイント・ライト.
	*/
	struct PointLight {
		glm::vec3 position[8];
		glm::vec3 color[8];
	};

	/**
	*スポットライト.
	*/
	struct SpotLight {

		glm::vec4 dirAndCutOff[4];		//光の方向(xyzに入れる)とcos(放射角)(wに入れる).
		glm::vec4 posAndInnerCutOff[4];	//光の位置(xyzに入れる)とcos(減衰開始角)(wに入れる).
		glm::vec3 color[4];

	};

	/**
	*ライトをまとめた構造体.
	*/
	struct LightList {

		AmbientLight ambient;
		DirectionalLight directional;
		PointLight point;
		SpotLight spot;

		void Init();
	};

	/**
	*シェーダ・プログラム.
	*/
	class Program {
	public:
		Program();
		explicit Program(GLuint programId);
		~Program();
		
		void Reset(GLuint programId);
		bool IsNull()const;
		void Use();
		void BindTexture(GLuint, GLuint);
		void SetLightList(const LightList&);
		void SetViewProjectionMatrix(const glm::mat4&);
		void Draw(const Mesh& mesh,
			const glm::vec3& translate, const glm::vec3& rotate, const glm::vec3& scale);

	private:
		GLuint id = 0;	//プログラムID.

		//uniform変数の位置.
		GLint locMatMVP = -1;
		GLint locAmbLightCol = -1;
		GLint locDirLightDir = -1;
		GLint locDirLightCol = -1;
		GLint locPointLightPos = -1;
		GLint locPointLightCol = -1;
		GLint locSpotLightDir = -1;
		GLint locSpotLightPos = -1;
		GLint locSpotLightCol = -1;

		glm::mat4 matVP = glm::mat4(1);	//ビュー・プロジェクション行列.
		LightList lights;
	};

}// Shader namespace

#endif //SHADER_H_INCLUDED