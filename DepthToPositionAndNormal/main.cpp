#include "Interface.h"
#include "Sponza.h"
#include "DepthPass.h"
#include "RecontructPass.h"

int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1260, 720);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_DepthPass>("DepthPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_RecontructPass>("RecontructPass", 2));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}