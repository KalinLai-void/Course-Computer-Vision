#pragma once

#include "VideoInfo.h"

namespace OpenCVtoGray {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;

	VideoInfo videoInfo;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
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
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ Origin_pictureBox;
	protected:
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ openVideoToolStripMenuItem;


	private: System::Windows::Forms::TrackBar^ VideoProcess_trackBar;
	private: System::Windows::Forms::Label^ CurVideoTime_label;
	private: System::Windows::Forms::Label^ TotalVideoTime_label;

	private: System::Windows::Forms::Timer^ Video_timer;
	private: System::Windows::Forms::Button^ Play_button;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::CheckBox^ IsGray_checkBox;

	private: System::ComponentModel::IContainer^ components;



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
			this->components = (gcnew System::ComponentModel::Container());
			this->Origin_pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->openVideoToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->VideoProcess_trackBar = (gcnew System::Windows::Forms::TrackBar());
			this->CurVideoTime_label = (gcnew System::Windows::Forms::Label());
			this->TotalVideoTime_label = (gcnew System::Windows::Forms::Label());
			this->Video_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->Play_button = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->IsGray_checkBox = (gcnew System::Windows::Forms::CheckBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Origin_pictureBox))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VideoProcess_trackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// Origin_pictureBox
			// 
			this->Origin_pictureBox->Location = System::Drawing::Point(12, 87);
			this->Origin_pictureBox->Name = L"Origin_pictureBox";
			this->Origin_pictureBox->Size = System::Drawing::Size(360, 300);
			this->Origin_pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->Origin_pictureBox->TabIndex = 0;
			this->Origin_pictureBox->TabStop = false;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openVideoToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(386, 33);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// openVideoToolStripMenuItem
			// 
			this->openVideoToolStripMenuItem->Name = L"openVideoToolStripMenuItem";
			this->openVideoToolStripMenuItem->Size = System::Drawing::Size(124, 29);
			this->openVideoToolStripMenuItem->Text = L"Open Video";
			this->openVideoToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::openVideoToolStripMenuItem_Click);
			// 
			// VideoProcess_trackBar
			// 
			this->VideoProcess_trackBar->BackColor = System::Drawing::SystemColors::Control;
			this->VideoProcess_trackBar->Location = System::Drawing::Point(12, 393);
			this->VideoProcess_trackBar->Maximum = 1000;
			this->VideoProcess_trackBar->Name = L"VideoProcess_trackBar";
			this->VideoProcess_trackBar->Size = System::Drawing::Size(360, 45);
			this->VideoProcess_trackBar->TabIndex = 4;
			this->VideoProcess_trackBar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->VideoProcess_trackBar->Scroll += gcnew System::EventHandler(this, &MyForm::VideoProcess_trackBar_Scroll);
			// 
			// CurVideoTime_label
			// 
			this->CurVideoTime_label->AutoSize = true;
			this->CurVideoTime_label->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->CurVideoTime_label->Location = System::Drawing::Point(12, 422);
			this->CurVideoTime_label->Name = L"CurVideoTime_label";
			this->CurVideoTime_label->Size = System::Drawing::Size(44, 16);
			this->CurVideoTime_label->TabIndex = 5;
			this->CurVideoTime_label->Text = L"00:00";
			// 
			// TotalVideoTime_label
			// 
			this->TotalVideoTime_label->AutoSize = true;
			this->TotalVideoTime_label->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->TotalVideoTime_label->Location = System::Drawing::Point(330, 422);
			this->TotalVideoTime_label->Name = L"TotalVideoTime_label";
			this->TotalVideoTime_label->Size = System::Drawing::Size(44, 16);
			this->TotalVideoTime_label->TabIndex = 6;
			this->TotalVideoTime_label->Text = L"00:00";
			// 
			// Video_timer
			// 
			this->Video_timer->Tick += gcnew System::EventHandler(this, &MyForm::Video_timer_Tick);
			// 
			// Play_button
			// 
			this->Play_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Play_button->Location = System::Drawing::Point(12, 36);
			this->Play_button->Name = L"Play_button";
			this->Play_button->Size = System::Drawing::Size(88, 45);
			this->Play_button->TabIndex = 8;
			this->Play_button->Text = L"Play";
			this->Play_button->UseVisualStyleBackColor = true;
			this->Play_button->Click += gcnew System::EventHandler(this, &MyForm::Play_button_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// IsGray_checkBox
			// 
			this->IsGray_checkBox->AutoSize = true;
			this->IsGray_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->IsGray_checkBox->Location = System::Drawing::Point(118, 48);
			this->IsGray_checkBox->Name = L"IsGray_checkBox";
			this->IsGray_checkBox->Size = System::Drawing::Size(72, 23);
			this->IsGray_checkBox->TabIndex = 9;
			this->IsGray_checkBox->Text = L"Gray\?";
			this->IsGray_checkBox->UseVisualStyleBackColor = true;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(386, 448);
			this->Controls->Add(this->IsGray_checkBox);
			this->Controls->Add(this->Play_button);
			this->Controls->Add(this->TotalVideoTime_label);
			this->Controls->Add(this->CurVideoTime_label);
			this->Controls->Add(this->VideoProcess_trackBar);
			this->Controls->Add(this->Origin_pictureBox);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"OpenCV Video Capture and to Gray";
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Origin_pictureBox))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VideoProcess_trackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		Bitmap^ gray;
		ColorPalette^ cp;

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void openVideoToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Play_button_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void VideoProcess_trackBar_Scroll(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Video_timer_Tick(System::Object^ sender, System::EventArgs^ e);

	private: System::Void Init(); // initialize after file opened
	private: System::Void SetVideoProcessBarInfo(bool bIsInit, long posNow);
	private: System::Void ToGray(cv::Mat input, cv::Mat& output);
	private: Bitmap^ ConvertMatToBitmap(cv::Mat mat);
	};
}
