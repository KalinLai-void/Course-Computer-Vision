#pragma once

namespace MorphologyOperation {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::Drawing::Design;

	/// <summary>
	/// Summary for Main
	/// </summary>
	public ref class Main : public System::Windows::Forms::Form
	{
	public:
		Main(void)
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
		~Main()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ openImageToolStripMenuItem;
	private: System::Windows::Forms::PictureBox^ Origin_pictureBox;
	private: System::Windows::Forms::PictureBox^ Result_pictureBox;
	private: System::Windows::Forms::NumericUpDown^ Threshold_numericUpDown;
	private: System::Windows::Forms::Button^ RunBtn;
	private: System::Windows::Forms::ComboBox^ Mode_comboBox;
	private: System::Windows::Forms::RadioButton^ WhiteText_radioButton;
	private: System::Windows::Forms::RadioButton^ BlackText_radioButton;
	private: System::Windows::Forms::ToolStripMenuItem^ resetImgToolStripMenuItem;

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
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->openImageToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->Origin_pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->Result_pictureBox = (gcnew System::Windows::Forms::PictureBox());
			this->Threshold_numericUpDown = (gcnew System::Windows::Forms::NumericUpDown());
			this->RunBtn = (gcnew System::Windows::Forms::Button());
			this->Mode_comboBox = (gcnew System::Windows::Forms::ComboBox());
			this->WhiteText_radioButton = (gcnew System::Windows::Forms::RadioButton());
			this->BlackText_radioButton = (gcnew System::Windows::Forms::RadioButton());
			this->resetImgToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Origin_pictureBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Result_pictureBox))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Threshold_numericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Font = (gcnew System::Drawing::Font(L"Segoe UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->openImageToolStripMenuItem,
					this->resetImgToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Padding = System::Windows::Forms::Padding(4, 2, 0, 2);
			this->menuStrip1->Size = System::Drawing::Size(732, 29);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// openImageToolStripMenuItem
			// 
			this->openImageToolStripMenuItem->Name = L"openImageToolStripMenuItem";
			this->openImageToolStripMenuItem->Size = System::Drawing::Size(107, 25);
			this->openImageToolStripMenuItem->Text = L"Open Image";
			this->openImageToolStripMenuItem->Click += gcnew System::EventHandler(this, &Main::openImageToolStripMenuItem_Click);
			// 
			// Origin_pictureBox
			// 
			this->Origin_pictureBox->Location = System::Drawing::Point(11, 66);
			this->Origin_pictureBox->Margin = System::Windows::Forms::Padding(2);
			this->Origin_pictureBox->Name = L"Origin_pictureBox";
			this->Origin_pictureBox->Size = System::Drawing::Size(352, 288);
			this->Origin_pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->Origin_pictureBox->TabIndex = 1;
			this->Origin_pictureBox->TabStop = false;
			// 
			// Result_pictureBox
			// 
			this->Result_pictureBox->Location = System::Drawing::Point(369, 66);
			this->Result_pictureBox->Margin = System::Windows::Forms::Padding(2);
			this->Result_pictureBox->Name = L"Result_pictureBox";
			this->Result_pictureBox->Size = System::Drawing::Size(352, 288);
			this->Result_pictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
			this->Result_pictureBox->TabIndex = 5;
			this->Result_pictureBox->TabStop = false;
			// 
			// Threshold_numericUpDown
			// 
			this->Threshold_numericUpDown->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Threshold_numericUpDown->Location = System::Drawing::Point(277, 33);
			this->Threshold_numericUpDown->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->Threshold_numericUpDown->Name = L"Threshold_numericUpDown";
			this->Threshold_numericUpDown->ReadOnly = true;
			this->Threshold_numericUpDown->Size = System::Drawing::Size(86, 28);
			this->Threshold_numericUpDown->TabIndex = 8;
			this->Threshold_numericUpDown->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			// 
			// RunBtn
			// 
			this->RunBtn->Font = (gcnew System::Drawing::Font(L"PMingLiU", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->RunBtn->Location = System::Drawing::Point(369, 33);
			this->RunBtn->Name = L"RunBtn";
			this->RunBtn->Size = System::Drawing::Size(134, 30);
			this->RunBtn->TabIndex = 7;
			this->RunBtn->Text = L"Run";
			this->RunBtn->UseVisualStyleBackColor = true;
			this->RunBtn->Click += gcnew System::EventHandler(this, &Main::RunBtn_Click);
			// 
			// Mode_comboBox
			// 
			this->Mode_comboBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->Mode_comboBox->Font = (gcnew System::Drawing::Font(L"PMingLiU", 15.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->Mode_comboBox->FormattingEnabled = true;
			this->Mode_comboBox->Items->AddRange(gcnew cli::array< System::Object^  >(4) { L"Dilation", L"Erosion", L"Opening", L"Closing" });
			this->Mode_comboBox->Location = System::Drawing::Point(12, 32);
			this->Mode_comboBox->Name = L"Mode_comboBox";
			this->Mode_comboBox->Size = System::Drawing::Size(259, 29);
			this->Mode_comboBox->TabIndex = 6;
			// 
			// WhiteText_radioButton
			// 
			this->WhiteText_radioButton->AutoSize = true;
			this->WhiteText_radioButton->Checked = true;
			this->WhiteText_radioButton->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->WhiteText_radioButton->Location = System::Drawing::Point(509, 40);
			this->WhiteText_radioButton->Name = L"WhiteText_radioButton";
			this->WhiteText_radioButton->Size = System::Drawing::Size(106, 20);
			this->WhiteText_radioButton->TabIndex = 10;
			this->WhiteText_radioButton->TabStop = true;
			this->WhiteText_radioButton->Text = L"¶Â©³¥Õ¦r¡H";
			this->WhiteText_radioButton->UseVisualStyleBackColor = true;
			// 
			// BlackText_radioButton
			// 
			this->BlackText_radioButton->AutoSize = true;
			this->BlackText_radioButton->Font = (gcnew System::Drawing::Font(L"PMingLiU", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->BlackText_radioButton->Location = System::Drawing::Point(614, 40);
			this->BlackText_radioButton->Name = L"BlackText_radioButton";
			this->BlackText_radioButton->Size = System::Drawing::Size(106, 20);
			this->BlackText_radioButton->TabIndex = 11;
			this->BlackText_radioButton->Text = L"¥Õ©³¶Â¦r¡H";
			this->BlackText_radioButton->UseVisualStyleBackColor = true;
			// 
			// resetImgToolStripMenuItem
			// 
			this->resetImgToolStripMenuItem->Name = L"resetImgToolStripMenuItem";
			this->resetImgToolStripMenuItem->Size = System::Drawing::Size(91, 25);
			this->resetImgToolStripMenuItem->Text = L"Reset Img";
			this->resetImgToolStripMenuItem->Click += gcnew System::EventHandler(this, &Main::resetImgToolStripMenuItem_Click);
			// 
			// Main
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(732, 364);
			this->Controls->Add(this->BlackText_radioButton);
			this->Controls->Add(this->WhiteText_radioButton);
			this->Controls->Add(this->Threshold_numericUpDown);
			this->Controls->Add(this->RunBtn);
			this->Controls->Add(this->Mode_comboBox);
			this->Controls->Add(this->Result_pictureBox);
			this->Controls->Add(this->Origin_pictureBox);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"Main";
			this->Text = L"Morphology Operation";
			this->Load += gcnew System::EventHandler(this, &Main::Main_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Origin_pictureBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Result_pictureBox))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Threshold_numericUpDown))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void Main_Load(System::Object^ sender, System::EventArgs^ e);
	private: System::Void openImageToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void resetImgToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void RunBtn_Click(System::Object^ sender, System::EventArgs^ e);
	
	private: Bitmap^ Binarize(Bitmap^ bmp);
	private: Bitmap^ DilationOrErosion(int mode, Bitmap^ bmp); /* mode = 0: Dilation, mode = 1 : Erosion */

	/* mode = 0: Dilation, mode = 1 : Erosion */
	private: System::Void GetOriginImgData(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ bmpPixel);
	/* mode = 0: Dilation, mode = 1 : Erosion */
	private: System::Void HandlePixels(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ bmpPixel); 
	private: Bitmap^ CopyPixelArrToOutput(Bitmap^ bmp, array<unsigned char, 2>^ pixelArray);
};
}
