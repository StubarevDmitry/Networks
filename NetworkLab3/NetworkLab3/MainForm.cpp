#include "Form.h"
#include "PlacesAPI.hpp"
#include <iostream>
#include <locale>

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<String^>^) {
	std::map<std::string, std::string> apiKeys = {
		{"weather", "31ad39ab2f83165c8fe141e33e2bf883"},
		{"placesArea", "5ae2e3f221c38a28845f05b64467bcc3564dd2196cebb523707263fe"},
		{"location", "dc9c35dd-f63c-463f-83c0-e94291899e56"}
	};

	PlacesAPI places(apiKeys);

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	NetworkLab3::MainForm form(places);
	Application::Run(% form);

    return 0;
}

