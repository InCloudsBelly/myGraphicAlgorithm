#include "Interface.h"
#include "Sponza.h"
#include "ShadingPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadingPass>("ShadingPass", 1));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}