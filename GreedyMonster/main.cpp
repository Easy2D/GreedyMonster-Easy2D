//---------------------------------------------------------
// 程序名称：GreedyMonster
// 作者：Nomango
// 编译环境：Visual Studio 2019 / Easy2D v2.0.4
// 项目类型：Win32 Application
//---------------------------------------------------------

#include "MainScene.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	// 初始化
	if (Game::init())
	{
		// 设置窗口标题和大小
		Window::setTitle(L"Greedy Monster");
		Window::setSize(512, 362);
		// 关闭控制台
		Logger::showConsole(false);

		// 添加资源路径
		Path::add(L"res/images/");
		Path::add(L"res/audio/");

		// 预加载音乐
		MusicPlayer::preload(IDR_WAVE1, L"WAVE");
		MusicPlayer::preload(IDR_WAVE2, L"WAVE");

		// 创建场景并进入
		auto scene = gcnew MainScene;
		SceneManager::enter(scene);

		// 开始游戏
		Game::start();
	}
	// 销毁游戏资源
	Game::destroy();
	return 0;
}
