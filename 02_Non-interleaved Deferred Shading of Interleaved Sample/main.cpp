#include "Interface.h"
#include "Sponza.h"
#include "LightSource.h"
#include "SponzaGBufferPass.h"
#include "ScreenQuadPass.h"
#include "SplitGBufferPass.h"
#include "ShadingOnSplitedGBufferPass.h"
#include "GatherBufferPass.h"
#include "EgdeDetectionPass.h"
#include "GaussianBlurPass.h"
#include "CustomGUI.h"

int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightSources>("LightSources", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SponzaGBufferPass>("SponzaGBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_EdgeDetectionPass>("EdgeDetectionPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SplitGBufferPass>("SplitGBufferPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadingOnSplitedGBufferPass>("ShadingOnSplitedGBufferPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GatherBufferPass>("GatherBufferPass", 5));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GaussianBlurPass>("GaussianBlurPass", 6));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 7));

#ifdef _DEBUG
	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));
#endif

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}