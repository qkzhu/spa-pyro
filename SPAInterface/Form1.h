#pragma once

namespace SPAInterface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void): pkb(NULL)
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
	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	protected: 
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  queryButton;
	private: System::Windows::Forms::RichTextBox^  queryInput;
	private: System::Windows::Forms::RichTextBox^  queryOutput;



	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  loadButton;
	private: System::Windows::Forms::Button^  browseButton;


	private: System::Windows::Forms::TextBox^  fileLocation;
	private: System::Windows::Forms::RichTextBox^  sourceOutput;


	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;

	private: PKB* pkb;

	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->queryButton = (gcnew System::Windows::Forms::Button());
			this->queryInput = (gcnew System::Windows::Forms::RichTextBox());
			this->queryOutput = (gcnew System::Windows::Forms::RichTextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->loadButton = (gcnew System::Windows::Forms::Button());
			this->browseButton = (gcnew System::Windows::Forms::Button());
			this->fileLocation = (gcnew System::Windows::Forms::TextBox());
			this->sourceOutput = (gcnew System::Windows::Forms::RichTextBox());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// splitContainer1
			// 
			this->splitContainer1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->splitContainer1->Location = System::Drawing::Point(0, 0);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->label3);
			this->splitContainer1->Panel1->Controls->Add(this->label1);
			this->splitContainer1->Panel1->Controls->Add(this->queryButton);
			this->splitContainer1->Panel1->Controls->Add(this->queryInput);
			this->splitContainer1->Panel1->Controls->Add(this->queryOutput);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->label2);
			this->splitContainer1->Panel2->Controls->Add(this->loadButton);
			this->splitContainer1->Panel2->Controls->Add(this->browseButton);
			this->splitContainer1->Panel2->Controls->Add(this->fileLocation);
			this->splitContainer1->Panel2->Controls->Add(this->sourceOutput);
			this->splitContainer1->Size = System::Drawing::Size(970, 471);
			this->splitContainer1->SplitterDistance = 498;
			this->splitContainer1->TabIndex = 0;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(16, 245);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(38, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Query:";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(42, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Output:";
			// 
			// queryButton
			// 
			this->queryButton->Location = System::Drawing::Point(404, 431);
			this->queryButton->Name = L"queryButton";
			this->queryButton->Size = System::Drawing::Size(75, 23);
			this->queryButton->TabIndex = 2;
			this->queryButton->Text = L"Send Query";
			this->queryButton->UseVisualStyleBackColor = true;
			this->queryButton->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// queryInput
			// 
			this->queryInput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->queryInput->Location = System::Drawing::Point(13, 268);
			this->queryInput->Name = L"queryInput";
			this->queryInput->Size = System::Drawing::Size(466, 157);
			this->queryInput->TabIndex = 1;
			this->queryInput->Text = L"";
			// 
			// queryOutput
			// 
			this->queryOutput->BackColor = System::Drawing::SystemColors::Window;
			this->queryOutput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->queryOutput->Location = System::Drawing::Point(13, 32);
			this->queryOutput->Name = L"queryOutput";
			this->queryOutput->ReadOnly = true;
			this->queryOutput->Size = System::Drawing::Size(466, 206);
			this->queryOutput->TabIndex = 0;
			this->queryOutput->Text = L"";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(4, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(44, 13);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Source:";
			// 
			// loadButton
			// 
			this->loadButton->Location = System::Drawing::Point(387, 436);
			this->loadButton->Name = L"loadButton";
			this->loadButton->Size = System::Drawing::Size(75, 23);
			this->loadButton->TabIndex = 3;
			this->loadButton->Text = L"Load";
			this->loadButton->UseVisualStyleBackColor = true;
			this->loadButton->Click += gcnew System::EventHandler(this, &Form1::loadButton_Click);
			// 
			// browseButton
			// 
			this->browseButton->Location = System::Drawing::Point(306, 436);
			this->browseButton->Name = L"browseButton";
			this->browseButton->Size = System::Drawing::Size(75, 23);
			this->browseButton->TabIndex = 2;
			this->browseButton->Text = L"Browse...";
			this->browseButton->UseVisualStyleBackColor = true;
			this->browseButton->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// fileLocation
			// 
			this->fileLocation->Location = System::Drawing::Point(3, 439);
			this->fileLocation->Name = L"fileLocation";
			this->fileLocation->Size = System::Drawing::Size(296, 20);
			this->fileLocation->TabIndex = 1;
			// 
			// sourceOutput
			// 
			this->sourceOutput->BackColor = System::Drawing::SystemColors::Window;
			this->sourceOutput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->sourceOutput->Location = System::Drawing::Point(3, 32);
			this->sourceOutput->Name = L"sourceOutput";
			this->sourceOutput->ReadOnly = true;
			this->sourceOutput->Size = System::Drawing::Size(453, 393);
			this->sourceOutput->TabIndex = 0;
			this->sourceOutput->Text = L"";
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(970, 471);
			this->Controls->Add(this->splitContainer1);
			this->Name = L"Form1";
			this->Text = L"Static Program Analyser";
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel1->PerformLayout();
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 		if(openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
					  {
						 this->fileLocation->Text = openFileDialog1->FileName;
					  }
			 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 try {
				 if (pkb == NULL)
					 throw new string("No source file loaded");

				 //obtaining the input query
				 string input;
				 MyMarshalString(queryInput->Text, input);

				 //outputting the results
				 std::list<std::string> results;

				 evaluate(input, results);

				 stringstream results_output;
				 while (results.size() > 0)
				 {
					 results_output << results.front() << ", ";
					 results.pop_front();
				 }
				 queryOutput->Text = gcnew String(results_output.str().c_str());
			 } catch (string *s) {
				 String^ msg = gcnew String(s->c_str());
				 show_error(msg, "Query Error");
				 queryOutput->Text = msg;
			 }
		 }
private: System::Void show_error(String^ msg, String^ title)
		 {
			 MessageBox::Show(msg, title, MessageBoxButtons::OK, MessageBoxIcon::Stop );
		 }

private: System::Void MyMarshalString ( String ^ s, string& os ) {
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}

private: System::Void evaluate(std::string query, std::list<std::string>& results){
	// call your evaluator to evaluate the query here
		//substitute away the newline
		static regex rx("\n");
		query = regex_replace(query, rx, string(""));
		
		PqlPreprocessor pql (query);
		QueryEvaluator bqe (pkb, &pql);
		bqe.evaluate();
		QueryResult qr = bqe.getResult();
		
		if(qr.isBoolSet()){
			if(qr.getBoolValue())
				results.push_back("true");
			else results.push_back("false"); 
		}else{
			vector<vector<int> > resultTuple = qr.getTuple();
			vector<int> resultTupleType = qr.getTypes();
			int count = 0;
			if((int)resultTupleType.at(0) != -1){
				for(vector<vector<int> >::iterator i=resultTuple.begin(); i<resultTuple.end(); i++){
					string result_s;	
					count = 0;
					for(vector<int>::iterator j = (*i).begin(); j < (*i).end(); j++){
						int type = resultTupleType.at(count++);
						if(type == bqe.PQL_getIndex("integer")){
							char tmp[10] ;
							itoa(*j, tmp, 10);
							string s(tmp);
							result_s = result_s + s;
						}else if(type == bqe.PQL_getIndex("varOfSimpl"))
							result_s = result_s + bqe.PKB_varDecode(*j);	
						else if(type == bqe.PQL_getIndex("procOfSimpl")) 
							result_s = result_s + bqe.PKB_procDecode(*j);
						else 
							throw new string("No type match when decode result for query result");
						if(j != (*i).end() -1) result_s = result_s + " ";
					}
					//cout << "answer = " <<result_s << endl;
					results.push_back(result_s);
				}
			}
		}
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
}

private: System::Void loadButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 delete pkb;
			 pkb = new PKB();
			 try {
				 //reading file location
				 string fileLocation;
				 String ^input =this->fileLocation->Text; 
				 MyMarshalString(input, fileLocation);

				 //parsing program
				Parser p(*pkb, fileLocation);
				p.parseProgram();

				//displaying program
				string result = printProgram(*pkb);
				this->sourceOutput->Text = gcnew String(result.c_str());

			 } catch (string *s) {
				 String^ msg = gcnew String(s->c_str());
				 show_error(msg, "Parsing error");
				 this->sourceOutput->Text = msg;
				 delete pkb;
				 pkb = NULL;
			 }
		 }
};
}

