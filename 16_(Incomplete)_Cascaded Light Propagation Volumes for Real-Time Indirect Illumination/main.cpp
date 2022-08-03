#include "Interface.h"
#include "Sponza.h"
#include "GBufferPass.h"
#include "ScreenQuadPass.h"
#include "RSMBufferPass.h"
#include "LightCamera.h"
#include "CustomGUI.h"
#include "DirectLightPass.h"
#include "AABBDebugPass.h"
#include "LightInjectPass.h"
#include "GeometryInjectPass.h"
#include "PropagationPass.h"
#include "IndirectLightPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setSampleNum(1);
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1080);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(true);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightCamera>("LightCamera", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<GBufferPass>("SponzaGBufferPass", 0));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CRSMBufferPass>("RSMBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_DirectLightPass>("DirectLightPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CLightInjectPass>("LightInjectPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CGeometryInjectPass>("GeometryInjectPass", 4));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CPropagationPass>("PropagationPass", 5));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CIndirectLightPass>("IndirectLightPass", 6));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 7));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CAABBDebugPass>("AABBDebugPass", 8));


	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_CustomGUI>("CustomGUI", 1));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}