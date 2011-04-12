#pragma once

namespace SPAUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Diagnostics;
	using namespace System::IO;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RichTextBox^  outputBox;
	private: System::Windows::Forms::RichTextBox^  inputBox;
	protected: 

	protected: 

	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  fileLocation;
	private: System::Windows::Forms::Label^  label3;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->outputBox = (gcnew System::Windows::Forms::RichTextBox());
			this->inputBox = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->fileLocation = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// outputBox
			// 
			this->outputBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->outputBox->Location = System::Drawing::Point(15, 28);
			this->outputBox->Name = L"outputBox";
			this->outputBox->Size = System::Drawing::Size(594, 253);
			this->outputBox->TabIndex = 0;
			this->outputBox->Text = L"";
			// 
			// inputBox
			// 
			this->inputBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->inputBox->Location = System::Drawing::Point(12, 335);
			this->inputBox->Name = L"inputBox";
			this->inputBox->Size = System::Drawing::Size(594, 125);
			this->inputBox->TabIndex = 1;
			this->inputBox->Text = L"";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(511, 466);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(98, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Send Command";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(20, 10);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Output:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(12, 319);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(34, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Input:";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(15, 288);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Refresh";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// fileLocation
			// 
			this->fileLocation->Location = System::Drawing::Point(80, 468);
			this->fileLocation->Name = L"fileLocation";
			this->fileLocation->Size = System::Drawing::Size(411, 20);
			this->fileLocation->TabIndex = 6;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(17, 471);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(57, 13);
			this->label3->TabIndex = 7;
			this->label3->Text = L"Query File:";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(621, 530);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->fileLocation);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->inputBox);
			this->Controls->Add(this->outputBox);
			this->Name = L"Form1";
			this->Text = L"SPA UI";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				writeFile();
				Process::Start("SPA.bat");
			 }
	private: System::Void readFile()
	{
		String^ textFile = L"tempOut.txt"; 
		this->outputBox->Text = "";
	    StreamReader ^reader=gcnew  StreamReader(textFile);
		do
		{
		    this->outputBox->Text += reader->ReadLine() + "\n";
		}   
		while(reader->Peek() != -1);
		reader->Close();
	}

	private: System::Void writeFile()
	{
		String^ outputFile = L"tempIn.txt";
		StreamWriter^ pwriter = gcnew StreamWriter(outputFile);
		pwriter->Write(this->inputBox->Text);
		pwriter->Close();
	}
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 readFile();
		 }
};
}

