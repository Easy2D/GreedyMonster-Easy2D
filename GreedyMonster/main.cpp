//---------------------------------------------------------
// �������ƣ�GreedyMonster
// ���ߣ�Nomango
// ���뻷����Visual Studio 2019 / Easy2D v2.1.12
// ��Ŀ���ͣ�Win32 Application
//---------------------------------------------------------

#include "MainScene.h"
#include "resource.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ��ʼ��
	if (Game::init())
	{
		// ���ô��ڱ���ʹ�С
		Window::setTitle("Greedy Monster");
		Window::setSize(512, 362);
		Window::setIcon(IDI_ICON1);

		// �����Դ·��
		Path::add("res/images/");
		Path::add("res/audio/");

		// Ԥ��������
		MusicPlayer::preload(IDR_WAVE1, "WAVE");
		MusicPlayer::preload(IDR_WAVE2, "WAVE");

		// ��������������
		auto scene = gcnew MainScene;
		SceneManager::enter(scene);

		// ��ʼ��Ϸ
		Game::start();
	}
	// ������Ϸ��Դ
	Game::destroy();
	return 0;
}
