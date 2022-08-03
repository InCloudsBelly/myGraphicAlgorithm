#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "ShadingWithRSMAndTilePass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "GaussianBlurPass.h"
#include "LightSourcePass.h"
#include "NormalComputePass.h"
#include "CalculatePlanePass.h"
#include "FitContinuityPlanePass.h"
#include "CalculateWeightPass.h"
#include "LowRSMBufferPass.h"
#include "CalculateEigenPass.h"
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
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CRSMBufferPass>("RSMBufferPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CLowRSMBufferPass>("LowRSMBufferPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CNormalComputePass>("NormalComputePass", 5));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CCalculateEigenPass>("CalculateEigenPass", 6));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CCalculatePlane>("CalculatePlanePass", 7));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CFitContinuityPlanePass>("FitContinuityPlanePass", 8));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CCalculateWeight>("CCalculateWeight", 9));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CGaussianBlurPass>("GaussianBlurPass", 10));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_DirectLightPass>("DirectLightPass", 12));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CShadingWithRSMAndTilePass>("ShadingWithRSMPass", 13));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 14));
	//MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_LightSourcePass>("LightSourcePass", 31));

	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}