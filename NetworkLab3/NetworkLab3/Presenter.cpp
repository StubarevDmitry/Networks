
#include "Form.h"
#include "PlacesModel.hpp"
#include "Presenter.hpp"

namespace NetworkLab3 {

    Presenter::Presenter(MainForm^ view, PlacesAPI& places)
    {
        this->view = view;
        this->model = gcnew Model(places);
    }

    void Presenter::OnButton1Click(const std::wstring& chois)
    {
        std::vector<std::wstring> locations = model->GetLocations(chois);
        view->ShowLocations(locations);
    }

    void Presenter::OnButton2Click(int selectedIndex)
    {
        std::wstring weatherInfo = model->GetWeatherAndDescriptions(selectedIndex + 1);
        view->ShowWeatherAndDescriptions(weatherInfo);
    }
}