#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;

[STAThreadAttribute]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MovingObjectSegmentation::MainForm myform;
	Application::Run(% myform);
	return 0;
}

System::Void MovingObjectSegmentation::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	Method_comboBox->SelectedIndex = 0;
	Play_button->Enabled = false;
	VideoProcess_trackBar->Enabled = false;
	return System::Void();
}

System::Void MovingObjectSegmentation::MainForm::openVideoToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
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

System::Void MovingObjectSegmentation::MainForm::Init()
{
	Video_timer->Enabled = true;
	Play_button->Text = "Pause";
	Play_button->Enabled = true;
	VideoProcess_trackBar->Enabled = true;
	SetVideoProcessBarInfo(true, 0);

	videoInfo.videoCap >> videoInfo.bg; // get first frame as background

	return System::Void();
}

System::Void MovingObjectSegmentation::MainForm::SetVideoProcessBarInfo(bool bIsInit, long posNow)
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

System::Void MovingObjectSegmentation::MainForm::Play_button_Click(System::Object^ sender, System::EventArgs^ e)
{
	if (Play_button->Text == "Play")
	{
		Play_button->Text = "Pause";
		Video_timer->Stop();
	}
	else if (Play_button->Text == "Pause")
	{
		Play_button->Text = "Play";
		Video_timer->Start();
	}
	return System::Void();
}

System::Void MovingObjectSegmentation::MainForm::VideoProcess_trackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	double now = (VideoProcess_trackBar->Value * 1.0f / VideoProcess_trackBar->Maximum) * videoInfo.frameCounts;
	videoInfo.videoCap.set(cv::CAP_PROP_POS_FRAMES, now);

	return System::Void();
}

System::Void MovingObjectSegmentation::MainForm::Video_timer_Tick(System::Object^ sender, System::EventArgs^ e)
{	
	long posNow = videoInfo.videoCap.get(cv::CAP_PROP_POS_FRAMES);
	if (posNow >= videoInfo.frameCounts - 1)
	{
		videoInfo.videoCap.set(cv::CAP_PROP_POS_FRAMES, 0);
	}
	SetVideoProcessBarInfo(false, posNow);

	videoInfo.videoCap >> videoInfo.input;
	Origin_pictureBox->Image = ConvertMatToBitmap(videoInfo.input);

	switch (Method_comboBox->SelectedIndex)
	{
		case 0: // Background Substraction
			Process_pictureBox->Image = BackgroundSubstraction(videoInfo.input, &videoInfo.output);
			break;
		case 1: // Frame Difference
			break;
	}
	
	return System::Void();
}

Bitmap^ MovingObjectSegmentation::MainForm::ConvertMatToBitmap(cv::Mat mat) {
	if (mat.channels() == 3) 
		return gcnew Bitmap(mat.cols, mat.rows, mat.step, PixelFormat::Format24bppRgb, (IntPtr)mat.data);
}

Bitmap^ MovingObjectSegmentation::MainForm::BackgroundSubstraction(cv::Mat mat, cv::Mat* outMat)
{
	cv::absdiff(videoInfo.bg, mat, *outMat);
	return ConvertMatToBitmap(*outMat);
}
