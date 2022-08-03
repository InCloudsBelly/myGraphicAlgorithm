#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "VSMBufferPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "NonuniformBlurPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setSampleNum(4);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightCamera>("LightCamera", 2));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_DynamicObject>("DynamicObject", 3));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_VSMBufferPass>("VSMBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_NonuniformBlurPass>("NonuniformBlurPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_DirectLightPass>("DirectLightPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 4));

	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}