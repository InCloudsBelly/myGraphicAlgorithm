#include "Interface.h"
#include "Sponza.h"
#include "SponzaGBufferPass.h"
#include "ScreenQuadPass.h"
#include "NonuniformBlurPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1024, 1024);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_SponzaGBufferPass>("SponzaGBufferPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_NonuniformBlurPass>("NonuniformBlurPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_ScreenQuadPass>("ScreenQuadPass", 7));


	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}