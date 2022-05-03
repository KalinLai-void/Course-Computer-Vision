#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

[STAThreadAttribute]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	OpenCVtoGray::MyForm myform;
	Application::Run(% myform);
	return 0;
}

System::Void OpenCVtoGray::MyForm::MyForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	Play_button->Enabled = false;
	VideoProcess_trackBar->Enabled = false;

	gray = gcnew Bitmap(1, 1, PixelFormat::Format8bppIndexed);
	cp = gray->Palette;
	for (int i = 0; i < 256; i++) 
	{
		cp->Entries[i] = Color::FromArgb(i, i, i);
	}

	return System::Void();
}

System::Void OpenCVtoGray::MyForm::openVideoToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	openFileDialog1->FileName = "";
	openFileDialog1->Filter = "Video File (*.avi)|*.avi";
	openFileDialog1->FilterIndex = 1;
	if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK && openFileDialog1->FileName != "")
	{
		const char* fileAnsi = (char*)((void*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(openFileDialog1->FileName).ToPointer()));
		std::string s = fileAnsi;
		videoInfo.videoCap = cv::VideoCapture(s);
		videoInfo.fps = videoInfo.videoCap.get(cv::CAP_PROP_FPS);
		videoInfo.frameCounts = videoInfo.videoCap.get(cv::CAP_PROP_FRAME_COUNT);
		Video_timer->Interval = 1000.0 / videoInfo.fps;
		videoInfo.videoCap >> videoInfo.input;
		Init();
	}
	return System::Void();
}

System::Void OpenCVtoGray::MyForm::Init()
{
	Video_timer->Enabled = true;
	Play_button->Text = "Pause";
	Play_button->Enabled = true;
	VideoProcess_trackBar->Enabled = true;
	SetVideoProcessBarInfo(true, 0);
	return System::Void();
}

System::Void OpenCVtoGray::MyForm::SetVideoProcessBarInfo(bool bIsInit, long posNow)
{
	long length = videoInfo.frameCounts / videoInfo.fps;
	if (bIsInit)
	{
		int mins = length / 60;
		int secs = length % 60;

		TotalVideoTime_label->Text = mins.ToString("00") + ":" + secs.ToString("00");
	}

	long now = posNow / videoInfo.fps;

	int mins = now / 60;
	int secs = now % 60;
	CurVideoTime_label->Text = mins.ToString("00") + ":" + secs.ToString("00");
	VideoProcess_trackBar->Value = (now * 1.0f / length) * VideoProcess_trackBar->Maximum;

	return System::Void();
}

System::Void OpenCVtoGray::MyForm::Play_button_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (Play_button->Text == "Play")
	{
		Play_button->Text = "Pause";
		Video_timer->Start();
	}
	else if (Play_button->Text == "Pause")
	{
		Play_button->Text = "Play";
		Video_timer->Stop();
	}
	return System::Void();
}

System::Void OpenCVtoGray::MyForm::VideoProcess_trackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	double now = (VideoProcess_trackBar->Value * 1.0f / VideoProcess_trackBar->Maximum) * videoInfo.frameCounts;
	videoInfo.videoCap.set(cv::CAP_PROP_POS_FRAMES, now);

	return System::Void();
}

System::Void OpenCVtoGray::MyForm::Video_timer_Tick(System::Object^ sender, System::EventArgs^ e)
{
	long posNow = videoInfo.videoCap.get(cv::CAP_PROP_POS_FRAMES);
	if (posNow >= videoInfo.frameCounts - 1)
	{
		videoInfo.videoCap.set(cv::CAP_PROP_POS_FRAMES, 0);
	}
	SetVideoProcessBarInfo(false, posNow);

	videoInfo.videoCap >> videoInfo.input;
	if (IsGray_checkBox->Checked)
		ToGray(videoInfo.input, videoInfo.output);
	else 
		videoInfo.output = videoInfo.input.clone();

	Origin_pictureBox->Image = ConvertMatToBitmap(videoInfo.output);

	return System::Void();
}

System::Void OpenCVtoGray::MyForm::ToGray(cv::Mat input, cv::Mat& output) {
	if (input.channels() == 1) 
	{
		output = input.clone();
		return;
	}

	cv::Mat process = cv::Mat(input.rows, input.cols, CV_8UC1);
	uchar* idata = input.data;
	uchar* pdata = process.data;

	for (int y = 0; y < input.rows; y++) 
	{
		for (int x = 0; x < input.cols; x++) 
		{
			int pixel = 0.114 * idata[0] + 0.587 * idata[1] + 0.299 * idata[2];
			pdata[0] = (uchar)pixel;
			idata += 3;
			pdata += 1;
		}
	}

	output = process.clone();
}

Bitmap^ OpenCVtoGray::MyForm::ConvertMatToBitmap(cv::Mat mat) {
	if (mat.channels() == 3)
		return gcnew Bitmap(mat.cols, mat.rows, mat.step, PixelFormat::Format24bppRgb, (IntPtr)mat.data);
	else 
	{
		Bitmap^ b = gcnew Bitmap(mat.cols, mat.rows, mat.step, PixelFormat::Format8bppIndexed, (IntPtr)mat.data);
		b->Palette = cp;
		return b;
	}
}
