#pragma once

#include "VideoInfo.h"

namespace MovingObjectSegmentation {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::Drawing::Design;

	VideoInfo videoInfo;

	struct Position
	{
		int row;
		int col;
	};

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
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
		~MainForm()
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
	private: System::Windows::Forms::PictureBox^ Process_pictureBox;
	private: System::Windows::Forms::PictureBox^ Result_pictureBox;

	private: System::Windows::Forms::Label^ CurVideoTime_label;
	private: System::Windows::Forms::Label^ TotalVideoTime_label;
	private: System::Windows::Forms::ComboBox^ Method_comboBox;
	private: System::Windows::Forms::Timer^ Video_timer;
	private: System::Windows::Forms::Button^ Play_button;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::CheckBox^ IsProcessWhite_checkBox;
	private: System::Windows::Forms::TrackBar^ VideoProcess_trackBar;

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
			this->Process_pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->Result_pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->CurVideoTime_label = (gcnew System::Windows::Forms::Label());
			this->TotalVideoTime_label = (gcnew System::Windows::Forms::Label());
			this->Method_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->Video_timer = (gcnew System::Windows::Forms::Timer(this->components));
			this->Play_button = (gcnew System::Windows::Forms::Button());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->IsProcessWhite_checkBox = (gcnew System::Windows::Forms::CheckBox());
			this->VideoProcess_trackBar = (gcnew System::Windows::Forms::TrackBar());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Origin_pictureBox))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Process_pictureBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Result_pictureBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VideoProcess_trackBar))->BeginInit();
			this->SuspendLayout();
			// 
			// Origin_pictureBox
			// 
			this->Origin_pictureBox->Location = System::Drawing::Point(30, 71);
			this->Origin_pictureBox->Name = L"Origin_pictureBox";
			this->Origin_pictureBox->Size = System::Drawing::Size(320, 240);
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
			this->menuStrip1->Size = System::Drawing::Size(1055, 33);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// openVideoToolStripMenuItem
			// 
			this->openVideoToolStripMenuItem->Name = L"openVideoToolStripMenuItem";
			this->openVideoToolStripMenuItem->Size = System::Drawing::Size(124, 29);
			this->openVideoToolStripMenuItem->Text = L"Open Video";
			this->openVideoToolStripMenuItem->Click += gcnew System::EventHandler(this, &MainForm::openVideoToolStripMenuItem_Click);
			// 
			// Process_pictureBox
			// 
			this->Process_pictureBox->Location = System::Drawing::Point(378, 71);
			this->Process_pictureBox->Name = L"Process_pictureBox";
			this->Process_pictureBox->Size = System::Drawing::Size(320, 240);
			this->Process_pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->Process_pictureBox->TabIndex = 2;
			this->Process_pictureBox->TabStop = false;
			// 
			// Result_pictureBox
			// 
			this->Result_pictureBox->Location = System::Drawing::Point(725, 71);
			this->Result_pictureBox->Name = L"Result_pictureBox";
			this->Result_pictureBox->Size = System::Drawing::Size(320, 240);
			this->Result_pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->Result_pictureBox->TabIndex = 3;
			this->Result_pictureBox->TabStop = false;
			// 
			// CurVideoTime_label
			// 
			this->CurVideoTime_label->AutoSize = true;
			this->CurVideoTime_label->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->CurVideoTime_label->Location = System::Drawing::Point(328, 334);
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
			this->TotalVideoTime_label->Location = System::Drawing::Point(704, 334);
			this->TotalVideoTime_label->Name = L"TotalVideoTime_label";
			this->TotalVideoTime_label->Size = System::Drawing::Size(44, 16);
			this->TotalVideoTime_label->TabIndex = 6;
			this->TotalVideoTime_label->Text = L"00:00";
			// 
			// Method_comboBox
			// 
			this->Method_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->Method_comboBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Method_comboBox->FormattingEnabled = true;
			this->Method_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Background Substraction", L"Frame Difference" });
			this->Method_comboBox->Location = System::Drawing::Point(12, 36);
			this->Method_comboBox->Name = L"Method_comboBox";
			this->Method_comboBox->Size = System::Drawing::Size(360, 29);
			this->Method_comboBox->TabIndex = 7;
			// 
			// Video_timer
			// 
			this->Video_timer->Tick += gcnew System::EventHandler(this, &MainForm::Video_timer_Tick);
			// 
			// Play_button
			// 
			this->Play_button->Font = (gcnew System::Drawing::Font(L"PMingLiU", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Play_button->Location = System::Drawing::Point(234, 322);
			this->Play_button->Name = L"Play_button";
			this->Play_button->Size = System::Drawing::Size(88, 38);
			this->Play_button->TabIndex = 8;
			this->Play_button->Text = L"Play";
			this->Play_button->UseVisualStyleBackColor = true;
			this->Play_button->Click += gcnew System::EventHandler(this, &MainForm::Play_button_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// IsProcessWhite_checkBox
			// 
			this->IsProcessWhite_checkBox->AutoSize = true;
			this->IsProcessWhite_checkBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->IsProcessWhite_checkBox->Location = System::Drawing::Point(378, 40);
			this->IsProcessWhite_checkBox->Name = L"IsProcessWhite_checkBox";
			this->IsProcessWhite_checkBox->Size = System::Drawing::Size(304, 25);
			this->IsProcessWhite_checkBox->TabIndex = 9;
			this->IsProcessWhite_checkBox->Text = L"Show the segmentation binary img";
			this->IsProcessWhite_checkBox->UseVisualStyleBackColor = true;
			// 
			// VideoProcess_trackBar
			// 
			this->VideoProcess_trackBar->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->VideoProcess_trackBar->BackColor = System::Drawing::SystemColors::Control;
			this->VideoProcess_trackBar->Location = System::Drawing::Point(378, 322);
			this->VideoProcess_trackBar->Maximum = 1000;
			this->VideoProcess_trackBar->Name = L"VideoProcess_trackBar";
			this->VideoProcess_trackBar->Size = System::Drawing::Size(320, 45);
			this->VideoProcess_trackBar->TabIndex = 10;
			this->VideoProcess_trackBar->TickStyle = System::Windows::Forms::TickStyle::None;
			this->VideoProcess_trackBar->Scroll += gcnew System::EventHandler(this, &MainForm::VideoProcess_trackBar_Scroll);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1055, 376);
			this->Controls->Add(this->VideoProcess_trackBar);
			this->Controls->Add(this->IsProcessWhite_checkBox);
			this->Controls->Add(this->Play_button);
			this->Controls->Add(this->Method_comboBox);
			this->Controls->Add(this->TotalVideoTime_label);
			this->Controls->Add(this->CurVideoTime_label);
			this->Controls->Add(this->Result_pictureBox);
			this->Controls->Add(this->Process_pictureBox);
			this->Controls->Add(this->Origin_pictureBox);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MainForm";
			this->Text = L"Moving Onject Segmentattion";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Origin_pictureBox))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Process_pictureBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Result_pictureBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->VideoProcess_trackBar))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void MainForm_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void openVideoToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Play_button_Click(System::Object^ sender, System::EventArgs^ e);
    private: System::Void VideoProcess_trackBar_Scroll(System::Object^ sender, System::EventArgs^ e);
	private: System::Void Video_timer_Tick(System::Object^ sender, System::EventArgs^ e);

	private: System::Void Init(); // initialize after file opened
	private: System::Void SetVideoProcessBarInfo(bool bIsInit, long posNow);
	private: Bitmap^ ConvertMatToBitmap(cv::Mat mat);

	private: Bitmap^ BackgroundSubtraction(cv::Mat mat, cv::Mat* binMat);
	private: Bitmap^ FrameDifference(cv::Mat mat, cv::Mat* binMat);

	private: Bitmap^ GetDiff(cv::Mat mat, cv::Mat diffTarget, cv::Mat* binMat);

	private: System::Void SelectObjects(Bitmap^ binaryBmp);
	private: System::Void ConnectedComponentLabeling_DFS(BitmapData^ bmpBD, int x, int y, int num); // using recursive DFS (sometimes overflow)
	private: int ConnectedComponentLabeling_BFS(BitmapData^ bmpBD, int num); // using iterative BFS (avoid overflow), return labelNum
	private: System::Void DrawArea(Bitmap^ binaryBmp, int num);

	/****************************************** Morphology Operation ******************************************/
	/* mode = 0: Dilation, mode = 1 : Erosion */
	private: Bitmap^ DilationOrErosion(int mode, Bitmap^ bmp);

	/* mode = 0: Dilation, mode = 1 : Erosion */
	private: System::Void GetOriginImgData(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ matPixel);
	/* mode = 0: Dilation, mode = 1 : Erosion */
	private: System::Void HandlePixels(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ matPixel);
	private: Bitmap^ CopyPixelArrToOutput(Bitmap^ bmp, array<unsigned char, 2>^ pixelArray);
};
}
