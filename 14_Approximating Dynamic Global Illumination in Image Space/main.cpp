#include "Interface.h"
#include "CornellBox.h"
#include "ScreenQuadPass.h"
#include "GBufferPass.h"
#include "SSAOandSSDOPass.h"
#include "SSAOandSSDOBlurPass.h"

int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<CornellBox>("CornellBox", 1));

	
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GBufferPass>("GBufferPass", 1));
	//SSAO & SSDO Indirect Light
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SSAOandSSDOPass>("SSAO&SSDOPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SSAOandSSDOBlurPass>("SSAO&SSDOBlurPass", 3));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 4));



	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}