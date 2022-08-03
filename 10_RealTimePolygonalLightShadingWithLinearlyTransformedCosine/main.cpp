#include <memory>
#include "DrawTextureArrayByFBO.h"
#include "LightSourcePass.h"
#include "GroundPass.h"
#include "LightSource.h"
#include "Ground.h"
#include "Interface.h"
#include "MyGUI.h"

int main()
{
	MyGraphics::Camera::setMainCameraPos({ -0.413941, 1.49538, 5.88081 });
	MyGraphics::Camera::setMainCameraFront({ 0.055595, -0.22376, -0.973057 });
	MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(true);
	MyGraphics::WINDOW_KEYWORD::setWindowSize(1080, 768);

	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_LightSource>("LightSource", 1));
	MyGraphics::ResourceManager::registerGameObject(std::make_shared<_Ground>("Ground", 2));

	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_DrawTextureArrayByFBO>("DrawTextureArrayByFBO", 1));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GroundPass>("GroundPass", 2));
	MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_LightSourcePass>("LightSourcePass", 3));

	MyGraphics::ResourceManager::registerSubGUI(std::make_shared<_MyGUI>("MyGUI", 1));

	MyGraphics::App::initApp();
	MyGraphics::App::updateApp();

	return 0;
}