#include "Interface.h"
#include "Sponza.h"
#include "Dragon.h"
#include "RecontructPass.h"
#include "OpacityShadingPass.h"
#include "FourierCoefficientPass.h"
#include "MergeShadingPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1920, 1152);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Dragon>("Dragon", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<COpacityShadingPass>("OpacityShadingPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CFourierCoefficientPass>("FourierCoefficientPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CRecontructPass>("RecontructPass", 3));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CMergeShadingPass>("MergeShadingPass", 4));


	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}