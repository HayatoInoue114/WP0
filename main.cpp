#include "Project/GameManager/GameManager.h"
#include "Project/Scene/GameScene/GameScene.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	std::unique_ptr<GameManager> gameManager =
		std::make_unique<GameManager>(new GameScene);
	gameManager->Run();
	return 0;
}
