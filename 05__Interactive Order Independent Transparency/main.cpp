#include "Interface.h"
#include "Sponza.h"
#include "DepthPeellingPass.h"
#include "ScreenQuadPass.h"
#include "CustomGUI.h"
#include "LightCamera.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightCamera>("LightCamera", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<DepthPeellingPass>("DepthPeellingPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 2));

	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}