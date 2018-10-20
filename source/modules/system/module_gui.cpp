#include "mcv_platform.h"
#include "module_gui.h"
#include "render/render_objects.h"
#include "gui/gui_parser.h"

//using namespace GUI;


CModuleGUI::CModuleGUI(const std::string& name)
	: IModule(name)
{}

bool CModuleGUI::start()
{

	const float width = 1920;
	const float height = 1080;
	_orthoCamera.setOrthographic(width, height);

	_technique = Resources.get("gui.tech")->as<CRenderTechnique>();
	_quadMesh = Resources.get("unit_quad_xy.mesh")->as<CRenderMesh>();
	_fontTexture = Resources.get("data/textures/gui/Letras.dds")->as<CTexture>();

	CParser parser;
	parser.parseFile("data/gui/lineasTemblor.json");
	parser.parseFile("data/gui/precarga.json");
	parser.parseFile("data/gui/inicio.json");
	parser.parseFile("data/gui/pause_menu.json");
	parser.parseFile("data/gui/option_menu.json");
	parser.parseFile("data/gui/splash.json");
	/*parser.parseFile("data/gui/main_menu.json");
	parser.parseFile("data/gui/gameplay.json");
	parser.parseFile("data/gui/game_over.json");*/
//	timer = 3.f;
	splash = false;
	firstTime = true;
	//activateWidget("text_tutorial");

	//_variables.setVariant("progress", 0.5f);

	//MAIN MENU
	mmc = new CMainMenuController();

	//PAUSE MENU
	pmc = new CPauseMenuController();
	registerController(pmc);
	desactivePauseMenu();

	//OPTION MENU
	omc = new COptionMenuController();
	registerController(omc);
	desactiveOptionMenu();

	//KEYBOARD MENU
	kmc = new CKeyboardMenuController();
	registerController(kmc);
	desactiveKeyboardMenu();

	//OMNI DASH
	odc = new COmnidashArrowController();
	registerController(odc);
	setOmindash(false);

	return true;
}

bool CModuleGUI::stop()
{
	return true;
}

void CModuleGUI::update(float delta)
{
	//dbg("-----------------------TIME PERROOOO: %f\n", timer);

	if(EngineInput[VK_F5].getsPressed())activateWidget("lineasTemblor");
	if (splash) {
		
		
		//if(firstTime)firstdt
		if (delta < 1.0f) {
			if (firstTime) {

				CEntity* player = (CEntity*)getEntityByName("The Player");

				TMsgSetFSMVariable pauseMsg;
				pauseMsg.variant.setName("pause");
				pauseMsg.variant.setBool(true);

				player->sendMsg(pauseMsg);

				timer = 3.f;
				firstTime = false;
				activateWidget("splashNvidia");
				nvidia = true;
				upf = false;
				mainMenu = false;
			}
			timer -= delta;
		}
		//dbg("-----------------------TIME SPLASH: %f\n", delta);
		//dbg("-----------------------TIME SPLASH: %f\n", timer);
		if (timer <= 0.f || EngineInput[VK_RETURN].getsPressed()) {
			if (nvidia) {
				desactivateWidget("splashNvidia");
				activateWidget("splashUPF");
				nvidia = false;
				upf = true;
				timer = 3.f;
			}
			else if (upf) {
				desactivateWidget("splashUPF");
				activateWidget("splash3DMax");
				upf = false;
				timer = 3.f;
			}
			else {
				desactivateWidget("splash3DMax");
				activateWidget("pantallaInicio");
				timer = 1.f;
				splash = false;
				mainMenu = true;
			}
		}
	}
	else if(mainMenu) {
		if (delta < 1.0f) timer -= delta;
		if (timer <= 0.f) {
			registerController(mmc);
			mainMenu = false;
		}
	}

	for (auto& wdgt : _activeWidgets)
	{
		wdgt->updateAll(delta);
	}
	for (auto& controller : _controllers)
	{
		controller->update(delta);
	}

	//// change bar value
	//float value = _variables.getFloat("progress");
	//if (EngineInput[VK_LEFT].isPressed())
	//{
	//  value = clamp(value - 0.5f * delta, 0.f, 1.f);
	//}
	//if (EngineInput[VK_RIGHT].isPressed())
	//{
	//  value = clamp(value + 0.5f * delta, 0.f, 1.f);
	//}
	//_variables.setVariant("progress", value);

}

void CModuleGUI::renderGUI()
{
	for (auto& wdgt : _activeWidgets)
	{
		wdgt->renderAll();
	}
}

//FUNCION APARTE DEL MODULO DE ALBERT
void CModuleGUI::desactiveMainMenu()
{
	unregisterController(mmc);
}

void CModuleGUI::activeMainMenu()
{
	cb_gui.main = 1.f;
	registerController(mmc);
}

void CModuleGUI::desactivePauseMenu()
{
	pmc->setActive(false);
}

void CModuleGUI::activePauseMenu()
{
	pmc->setActive(true);
}

void CModuleGUI::resetPauseMenu()
{
	pmc->resetOptions();
}

void CModuleGUI::desactiveOptionMenu()
{
	omc->setActive(false);
}

void CModuleGUI::activeOptionMenu()
{
	omc->setActive(true);
}

void CModuleGUI::desactiveKeyboardMenu()
{
	kmc->setActive(false);
}

void CModuleGUI::activeKeyboardMenu()
{
	kmc->setActive(true);
}

void CModuleGUI::setOmindash(bool omnidash)
{
	odc->setActive(omnidash);
}

void CModuleGUI::setFullScreenOption(int x, int y)
{
	omc->setFullScreenOption(x, y);
}

void CModuleGUI::setResolutionOption(int x, int y)
{
	omc->setResolutionOption(x, y);
}

void CModuleGUI::changeResolution(int _x, int _y)
{
	/*for (auto& wdgt : _registeredWidgets)
	{
		wdgt->resizeAll(_x, _y, Render.width, Render.height);
	}*/
	omc->changeResolution(_x, _y);
}

void CModuleGUI::fullScreen(bool _fullscreen)
{
	omc->fullScreen(_fullscreen);
}

void CModuleGUI::registerWidget(CWidget* wdgt)
{
	_registeredWidgets.push_back(wdgt);
}

CWidget* CModuleGUI::getWidget(const std::string& name, bool recursive) const
{
	for (auto& rwdgt : _registeredWidgets)
	{
		if (rwdgt->getName() == name)
		{
			return rwdgt;
		}
	}

	if (recursive)
	{
		for (auto& rwdgt : _registeredWidgets)
		{
			CWidget* wdgt = rwdgt->getChild(name, true);
			if (wdgt)
			{
				return wdgt;
			}
		}
	}

	return nullptr;
}

void CModuleGUI::activateWidget(const std::string& name)
{
	CWidget* wdgt = getWidget(name);
	if (wdgt)
	{
		bool active = false;
		for (int i = 0; i < _activeWidgets.size(); i++) {
			auto name2 = _activeWidgets[i]->getName();
			if (_activeWidgets[i]->getName() == name) {
				active = true;
			}
		}
		if (!active)
			_activeWidgets.push_back(wdgt);
	}
}

//FUNCION APARTE DEL MODULO DE ALBERT
void CModuleGUI::desactivateWidget(const std::string& name)
{
	for (int i = 0; i < _activeWidgets.size(); i++) {
		auto name2 = _activeWidgets[i]->getName();
		if (_activeWidgets[i]->getName() == name) {
			_activeWidgets.erase(_activeWidgets.begin() + i);
			break;
		}
	}
}

void CModuleGUI::registerController(GUI::CController* controller)
{
	auto it = std::find(_controllers.begin(), _controllers.end(), controller);
	if (it == _controllers.end())
	{
		_controllers.push_back(controller);
	}
}

void CModuleGUI::unregisterController(GUI::CController* controller)
{
	auto it = std::find(_controllers.begin(), _controllers.end(), controller);
	if (it != _controllers.end())
	{
		_controllers.erase(it);
	}
}

CCamera& CModuleGUI::getCamera()
{
	return _orthoCamera;
}

MVariants& CModuleGUI::getVariables()
{
	return _variables;
}

void CModuleGUI::renderTexture(const MAT44& world, const CTexture* texture, const VEC2& minUV, const VEC2& maxUV, const VEC4& color)
{
	assert(_technique && _quadMesh);

	cb_object.obj_world = world;
	cb_object.obj_color = VEC4(1, 1, 1, 1);
	cb_object.updateGPU();

	cb_gui.minUV = minUV;
	cb_gui.maxUV = maxUV;
	cb_gui.tint_color = color;
	cb_gui.updateGPU();

	_technique->activate();
	if (texture)
		texture->activate(TS_ALBEDO);

	_quadMesh->activateAndRender();
}

void CModuleGUI::renderText(const MAT44& world, const std::string& text, const VEC4& color)
{
	assert(_fontTexture);

	int cellsPerRow = 8;
	float cellSize = 1.f / 8.f;
	char firstCharacter = ' ';
	for (size_t i = 0; i < text.size(); ++i)
	{
		char c = text[i];

		int cell = c - firstCharacter;
		int row = cell / cellsPerRow;
		int col = cell % cellsPerRow;

		VEC2 minUV = VEC2(col * cellSize, row * cellSize);
		VEC2 maxUV = minUV + VEC2(1, 1) * cellSize;
		VEC2 gap = i * VEC2(1, 0);
		MAT44 w = MAT44::CreateTranslation(gap.x, gap.y, 0.f) * world;

		renderTexture(w, _fontTexture, minUV, maxUV, color);
	}
}
