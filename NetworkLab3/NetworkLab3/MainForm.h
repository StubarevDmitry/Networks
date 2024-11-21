#pragma once
#include "Places.hpp"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <curl/curl.h>
#include <locale>
#include <codecvt>
#include "./include/json.hpp"
//#include "CompletableFuture.hpp"
#include <msclr/marshal_cppstd.h>
#include "ProcessCompletableFutures.h"
#include <Windows.h>
#include <future>

namespace NetworkLab3 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	std::wstring insertNewLines(const std::wstring& str, size_t maxLength) {
		std::wstring result;
		size_t currentLength = 0;

		for (wchar_t ch : str) {
			result += ch;
			currentLength++;

			if (currentLength >= maxLength && ch == L' ') {
				result += L'\n';
				currentLength = 0;
			}
		}

		return result;
	}

	/// <summary>
	/// Сводка для MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(Places& places) : places(places)
		{
			InitializeComponent();
		}
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::ListBox^ listBox1;
	public:

	protected:
		Places& places;
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ button1;

	private: System::Windows::Forms::Label^ label1;
	protected:

	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
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
			this->button1->Text = L"Выбрать";
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
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click_1);
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
#pragma endregion
	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e) {

	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {

		// Получаем текст из textBox1
		System::String^ chois = this->textBox1->Text;

		// Если вам нужно преобразовать System::String^ в std::string
		std::string choisStd = msclr::interop::marshal_as<std::string>(chois);

		std::vector<std::wstring> locations = places.getPlaces(choisStd);

		//отчистка
		this->listBox1->Items->Clear();

		label1->Visible = false;
		
		button2->Visible = true;

		listBox1->Visible = true;
		
		for (const auto& loc : locations) {
			System::String^ managedString = msclr::interop::marshal_as<System::String^>(loc);
			System::Console::WriteLine(managedString);

			managedString = managedString->Replace(L"\0", L"");

			this->listBox1->Items->Add(managedString);
		}

	}

	private: System::Void button2_Click_1(System::Object^ sender, System::EventArgs^ e) {
		int selectedIndex = listBox1->SelectedIndex;

		System::Console::WriteLine(selectedIndex);

		// Вызываем функцию для обработки CompletableFuture погода и разные места
		std::wstring string = insertNewLines(CompletableFuturesForOutputWeatherAndDescriptions(places, selectedIndex), 120);

		System::String^ managedString = 
			msclr::interop::marshal_as<System::String^>(string);
		
		managedString = managedString->Replace(L"\0", L"");
		
		label1->Visible = true;

		label1->Text = managedString;
	}

	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
};
}