#include "Interface.h"
#include "CornellBox.h"
#include "ScreenQuadPass.h"
#include "GBufferPass.h"
#include "HBAOPass.h"
#include "HBAOBlurPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1280, 720);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<CornellBox>("CornellBox", 1));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GBufferPass>("GBufferPass", 1));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_HBAOPass>("SSAOPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_HBAOBlurPass>("SSAOBlurPass", 3));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 6));



	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}