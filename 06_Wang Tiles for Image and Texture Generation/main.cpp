#include <memory>
#include "GroundPass.h"
#include "Ground.h"
#include "Interface.h"
#include "TilePakagePass.h"
int main()
{
	{
		MyGraphics::WINDOW_KEYWORD::setIsCursorDisable(false);
		MyGraphics::COMPONENT_CONFIG::setIsEnableGUI(false);
		MyGraphics::WINDOW_KEYWORD::setWindowSize(1080, 800);
		MyGraphics::Camera::setMainCameraPos(glm::vec3(-0.79372, 4.02423, 13.3243));
		MyGraphics::Camera::setMainCameraFront(glm::vec3(0.0708797, -0.280499, -1.0));

		MyGraphics::ResourceManager::registerRenderPass(std::make_shared<CTilePakagePass>("TilePakagePass", 1));
		MyGraphics::ResourceManager::registerRenderPass(std::make_shared<_GroundPass>("GroundPass", 2));

		MyGraphics::App::initApp();
		MyGraphics::App::updateApp();
	}

	return 0;
}