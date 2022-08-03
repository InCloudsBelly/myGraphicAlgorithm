#include "Interface.h"
#include "HalfCornelBox.h"
#include "Lucy.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "ShadingWithRSMPass.h"
#include "ShadowmapPass.h"

int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_HalfCornellBox>("HalfCornellBox", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<Lucy>("Lucy", 2));


	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GBufferPass>("GBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_RSMBufferPass>("RSMBufferPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadowmapPass>("ShadowmapPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadingWithRSMPass>("ShadingWithRSMPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 5));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}