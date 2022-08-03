#include "Interface.h"
#include "Sponza.h"
#include "SponzaGBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "ShadingWithRSMPass.h"
#include "CalculateVPLRadiusPass.h"
#include "ShadowmapPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"

int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightCamera>("LightCamera", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SponzaGBufferPass>("SponzaGBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_RSMBufferPass>("RSMBufferPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadowmapPass>("ShadowmapPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_CalculateVPLRadiusPass>("CalculateVPLRadiusPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadingWithRSMPass>("ShadingWithRSMPass", 5));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 6));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}