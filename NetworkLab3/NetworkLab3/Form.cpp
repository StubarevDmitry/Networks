#include "Presenter.hpp"
#include "PlacesAPI.hpp"
#include "Form.h"
#include <msclr/marshal_cppstd.h>

namespace NetworkLab3 {

    MainForm::MainForm(PlacesAPI& places)
    {
        InitializeComponent();
        presenter = gcnew Presenter(this, places);
    }

    void MainForm::InitializeComponent()
    {
        this->button1 = (gcnew System::Windows::Forms::Button());
        this->label1 = (gcnew System::Windows::Forms::Label());
        this->button2 = (gcnew System::Windows::Forms::Button());
        this->textBox1 = (gcnew System::Windows::Forms::TextBox());
        this->listBox1 = (gcnew System::Windows::Forms::ListBox());
        this->SuspendLayout();
        // 
        // button1
        // 
        this->button1->Location = System::Drawing::Point(417, 49);
        this->button1->Name = L"button1";
        this->button1->Size = System::Drawing::Size(247, 44);
        this->button1->TabIndex = 0;
        this->button1->Text = L"выбрать";
        this->button1->UseVisualStyleBackColor = true;
        this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
        // 
        // label1
        // 
        this->label1->AutoSize = true;
        this->label1->Location = System::Drawing::Point(63, 383);
        this->label1->Name = L"label1";
        this->label1->Size = System::Drawing::Size(70, 25);
        this->label1->TabIndex = 2;
        this->label1->Text = L"label1";
        this->label1->Visible = false;
        this->label1->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
        // 
        // button2
        // 
        this->button2->Location = System::Drawing::Point(1374, 140);
        this->button2->Name = L"button2";
        this->button2->Size = System::Drawing::Size(175, 204);
        this->button2->TabIndex = 3;
        this->button2->Text = L"Выбрать";
        this->button2->UseVisualStyleBackColor = true;
        this->button2->Visible = false;
        this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
        // 
        // textBox1
        // 
        this->textBox1->Location = System::Drawing::Point(35, 53);
        this->textBox1->Name = L"textBox1";
        this->textBox1->Size = System::Drawing::Size(281, 31);
        this->textBox1->TabIndex = 4;
        this->textBox1->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox1_TextChanged);
        // 
        // listBox1
        // 
        this->listBox1->FormattingEnabled = true;
        this->listBox1->HorizontalScrollbar = true;
        this->listBox1->ItemHeight = 25;
        this->listBox1->Location = System::Drawing::Point(40, 140);
        this->listBox1->Name = L"listBox1";
        this->listBox1->Size = System::Drawing::Size(1315, 204);
        this->listBox1->TabIndex = 5;
        this->listBox1->Visible = false;
        // 
        // MainForm
        // 
        this->AutoScaleDimensions = System::Drawing::SizeF(12, 25);
        this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
        this->AutoScroll = true;
        this->ClientSize = System::Drawing::Size(1734, 1119);
        this->Controls->Add(this->listBox1);
        this->Controls->Add(this->textBox1);
        this->Controls->Add(this->button2);
        this->Controls->Add(this->label1);
        this->Controls->Add(this->button1);
        this->Name = L"MainForm";
        this->Text = L"MainForm";
        this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
        this->ResumeLayout(false);
        this->PerformLayout();

    }

    void MainForm::ShowLocations(const std::vector<std::wstring>& locations)
    {
        this->listBox1->Items->Clear();
        for (const auto& loc : locations) {
            System::String^ managedString = msclr::interop::marshal_as<System::String^>(loc);
            managedString = managedString->Replace(L"\0", L"");
            this->listBox1->Items->Add(managedString);
        }
        this->listBox1->Visible = true;
        this->button2->Visible = true;
    }

    void MainForm::ShowWeatherAndDescriptions(const std::wstring& weatherInfo)
    {
        System::String^ managedString = msclr::interop::marshal_as<System::String^>(weatherInfo);
        managedString = managedString->Replace(L"\0", L"");
        this->label1->Text = managedString;
        this->label1->Visible = true;
    }

    System::Void MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e) {}

    System::Void MainForm::button1_Click(System::Object^ sender, System::EventArgs^ e)
    {
        System::String^ chois = this->textBox1->Text;
        std::wstring choisStd = msclr::interop::marshal_as<std::wstring>(chois);
        presenter->OnButton1Click(choisStd);
    }

    System::Void MainForm::button2_Click(System::Object^ sender, System::EventArgs^ e)
    {
        int selectedIndex = listBox1->SelectedIndex;
        presenter->OnButton2Click(selectedIndex);
    }

    System::Void MainForm::textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {}

    System::Void MainForm::label1_Click(System::Object^ sender, System::EventArgs^ e) {}
}