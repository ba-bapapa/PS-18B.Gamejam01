/**
*@file GLFWEW.cpp
*/

#include "GLFWEW.h"
#include<iostream>

///GLFWとGLFEWをラップするための名前空間.

namespace GLFWEW {

	/**
	*GLFWからのエラー報告を処理する.
	*
	*@param error	エラー番号.
	*@param desc	エラーの内容.
	*/

	void ErrorCallback(int error, const char* desc) {

		std::cerr << "ERROR:" << desc << std::endl;

	}

	/**
	*シングルトンインスタンスを取得する.
	*
	*@return Windowのシングルトンインスタンス.
	*/

	Window& Window::Instance() {

		static Window instance;
		return instance;

	}

	/**
	*コンストラクタ.
	*/

	Window::Window() {

	}

	/**
	*デストラクタ.
	*/

	Window::~Window() {

		if (isGLFWInitialized) {
			glfwTerminate();

		}
	}

	/**
	*GLFW/GLEWの初期化.
	*
	*@param w		ウィンドウの描画範囲の幅（ピクセル）.
	*@param h		ウィンドウの描画範囲の高さ（ピクセル）.
	*@param title	ウィンドウタイトル（UTF-８の０終端文字列）.
	*
	*@retval true	初期化成功.
	*@retval false	初期化失敗.
	*/

	bool Window::Init(int w, int h, const char* title) {

		if (isInitialized) {

			std::cerr << "ERROR:GLFWEWは既に初期化されています." << std::endl;
			return false;

		}

		if (!isGLFWInitialized) {

			glfwSetErrorCallback(ErrorCallback);
			
			if (glfwInit() != GL_TRUE) {
				return false;
			}

			isGLFWInitialized = true;
		}

		if (!window) {

			window = glfwCreateWindow(w, h, title, nullptr, nullptr);

			if (!window) {
				return false;

			}
			glfwMakeContextCurrent(window);
		}

		if (glewInit() != GLEW_OK) {

			std::cerr << "ERROR: GLEWの初期化に失敗しました." << std::endl;
			return false;
		}

		//OpenGLの情報をコンソールウィンドウへ出力する.

		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer:" << renderer << std::endl;
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version:" << version << std::endl;

		isInitialized = true;
		return true;

	}

	/**
	*ウィンドウを閉じるべきか調べる.
	*
	*@retval true	閉じる.
	*@retval false	閉じない.
	*/

	bool Window::ShouldClose()const {

		return glfwWindowShouldClose(window) != 0;
	}

	/**
	*フロントバッファとバックバッファを切り替える.
	*/

	void Window::SwapBuffers()const {

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	/**
	*キーが押されているか調べる.
	*
	*@param key 調べたいキー・コード(GLFW_KEY_Aなど）.
	*
	*@retval true	キーが押されている.
	*@retval false	キーが押されていない.
	*
	*使用できるキー・コードの一覧はglfw3.hにあります(「keyboard」で検索).
	*/

	bool Window::IsKeyPressed(int key) const {
		return glfwGetKey(window, key) == GLFW_PRESS;
	}

	/**
	*タイマーを初期化する.
	*/
	void Window::IniTimer() {

		glfwSetTime(0.0);
		previousTime = 0.0;
		deltaTime = 0.0;
	}

	/**
	*タイマーを更新する.
	*/
	void Window::UpDateTimer() {

		//経過時間を計測.
		const double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		//経過時間が長くなりすぎないように調整.
		const float upperLimit = 0.25f;	//経過時間として許容される上限.
		if (deltaTime > upperLimit) {
			deltaTime = 0.1f;
		}
	}

	/**
	*経過時間を取得する.
	*
	*@return	直前の2回のUpdateTimer()呼び出しの間に経過した時間.
	*/
	double Window::DeltaTime() const {
		return deltaTime;
	}

}//namespace GLFWEW