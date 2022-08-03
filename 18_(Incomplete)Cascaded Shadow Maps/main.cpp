#include "Interface.h"
#include "ScreenQuadPass.h"
#include "GBufferPass.h"
#include "Sponza.h"
#include "CascadedShadowMapPass.h"
#include "DirectLightPass.h"
#include "CustomGUI.h"
#include "LightCamera.h"
#include "CSMDebugPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightCamera>("LightCamera", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("GBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CascadedShadowMapPass>("CascadedShadowMapPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_DirectLightPass>("DirectLightPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_CSMDebugPass>("CSMDebugPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 5));

	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));
	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}