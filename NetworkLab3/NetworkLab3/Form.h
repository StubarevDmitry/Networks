#ifndef MAIN_FORM_HPP
#define MAIN_FORM_HPP

#include "PlacesAPI.hpp"
#include <vector>
#include <string>

namespace NetworkLab3 {

    ref class Presenter;
    public ref class MainForm : public System::Windows::Forms::Form
    {
    private:
        System::Windows::Forms::Button^ button1;
        System::Windows::Forms::Button^ button2;
        System::Windows::Forms::TextBox^ textBox1;
        System::Windows::Forms::ListBox^ listBox1;
        System::Windows::Forms::Label^ label1;

        Presenter^ presenter;

    public:
        MainForm(PlacesAPI& places);

        void InitializeComponent();
        void ShowLocations(const std::vector<std::wstring>& locations);
        void ShowWeatherAndDescriptions(const std::wstring& weatherInfo);

    private:
        System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e);
        System::Void button1_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void button2_Click(System::Object^ sender, System::EventArgs^ e);
        System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e);
        System::Void label1_Click(System::Object^ sender, System::EventArgs^ e);
    };
    
    //class Places;
}

#endif