#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenSpaceRayTracingPass.h"
#include "ScreenQuadPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setSampleNum(4);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_DynamicObject>("DynamicObject", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenSpaceRayTracingPass>("ScreenSpaceRayTracingPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 2));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}