#ifndef PRES_HPP
#define PRES_HPP

#include <string>
#include <vector>

class PlacesAPI;

namespace NetworkLab3 {

    // Forward declaration
    ref class MainForm;
    ref class Model;

    public ref class Presenter
    {
    private:
        Model^ model;
        MainForm^ view;

    public:
        Presenter(MainForm^ view, PlacesAPI& places);

        void OnButton1Click(const std::wstring& chois);
        void OnButton2Click(int selectedIndex);
    };
}

#endif