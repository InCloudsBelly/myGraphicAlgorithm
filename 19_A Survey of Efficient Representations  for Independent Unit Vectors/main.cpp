#include "Interface.h"
#include "EncodePass.h"
#include "DecodePass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1024, 1024);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);


	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<EncodePass>("EncodePass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<DecodePass>("DecodePass", 2));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}