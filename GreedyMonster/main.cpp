#include "MainScene.h"

int main()
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
		Player::preload(IDR_WAVE1, L"WAVE");
		Player::preload(IDR_WAVE2, L"WAVE");

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
