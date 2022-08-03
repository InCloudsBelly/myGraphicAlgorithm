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
#include "LowResolutionPass.h"
#include "ShadingLowResolutionPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(true);
	
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SponzaGBufferPass>("SponzaGBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_LowResolutionPass>("LowResolutionPass", 2, MyGraphics::ERenderPassType::RenderPassType_Once));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_EdgeDetectionPass>("EdgeDetectionPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SplitGBufferPass>("SplitGBufferPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadingLowResolutionPass>("ShadingLowResolutionPass", 5, MyGraphics::ERenderPassType::RenderPassType_Once));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ShadingOnSplitedGBufferPass>("ShadingOnSplitedGBufferPass", 7));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GatherBufferPass>("GatherBufferPass",8));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GaussianBlurPass>("GaussianBlurPass", 9));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 10));

	MyGraphics::Camera::setMainCameraFront(glm::vec3(0, -0.575005, -0.818128));
	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}