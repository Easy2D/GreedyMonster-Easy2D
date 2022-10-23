//---------------------------------------------------------
// 程序名称：GreedyMonster
// 作者：Nomango
// 编译环境：Visual Studio 2019 / Easy2D v2.1.12
// 项目类型：Win32 Application
//---------------------------------------------------------

#include "MainScene.h"
#include "resource.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// 初始化
	if (Game::init())
	{
		// 设置窗口标题和大小
		Window::setTitle("Greedy Monster");
		Window::setSize(512, 362);
		Window::setIcon(IDI_ICON1);

		// 添加资源路径
		Path::add("res/images/");
		Path::add("res/audio/");

		// 预加载音乐
		MusicPlayer::preload(IDR_WAVE1, "WAVE");
		MusicPlayer::preload(IDR_WAVE2, "WAVE");

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
