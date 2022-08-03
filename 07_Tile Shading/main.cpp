#include "Interface.h"
#include "Sponza.h"
#include "LightSource.h"
#include "SponzaGBufferPass.h"
#include "TilePass.h"
#include "ScreenQuadPass.h"

int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightSources>("LightSources", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SponzaGBufferPass>("SponzaGBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_TilePass>("TilePass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("SponzaGBufferPass", 3));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}