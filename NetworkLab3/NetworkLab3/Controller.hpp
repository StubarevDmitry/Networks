#pragma once
#include "Places.hpp"
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

class Controller
{
public:
	Controller(std::map<std::string, std::string> apiKeys): apiKeys(apiKeys), places(Places(apiKeys)){

		Application::SetCompatibleTextRenderingDefault(false);
		Application::EnableVisualStyles();
		NetworkLab3::MainForm form(places);
		Application::Run(% form);
	}

private:
	std::map<std::string, std::string> apiKeys;
	Places places;
};

