//---------------------------------------------------------
// �������ƣ�GreedyMonster
// ���ߣ�Nomango
// ���뻷����Visual Studio 2019 / Easy2D v2.0.4
// ��Ŀ���ͣ�Win32 Application
//---------------------------------------------------------

#include "MainScene.h"

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	// ��ʼ��
	if (Game::init())
	{
		// ���ô��ڱ���ʹ�С
		Window::setTitle(L"Greedy Monster");
		Window::setSize(512, 362);
		// �رտ���̨
		Logger::showConsole(false);

		// �����Դ·��
		Path::add(L"res/images/");
		Path::add(L"res/audio/");

		// Ԥ��������
		MusicPlayer::preload(IDR_WAVE1, L"WAVE");
		MusicPlayer::preload(IDR_WAVE2, L"WAVE");

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
