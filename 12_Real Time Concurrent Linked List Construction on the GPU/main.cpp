#include "Interface.h"
#include "Sponza.h"
#include "Dragon.h"
#include "CreateLinkListPass.h"
#include "BlendPass.h"
int main()
{
	MyGraphics::WINDOW_KEYWORD::setWindowSize(400, 400);
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Sponza>("Sponza", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Dragon>("Dragon", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CreateLinkListPass>("CreateLinkListPass", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_BlendPass>("BlendPass", 2));


	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}