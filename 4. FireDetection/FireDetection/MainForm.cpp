#include "MainForm.h"
#include "FlameThresholdVal.h"
#include "opencv2/opencv.hpp"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Design;

[STAThreadAttribute]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	FlameDetect::MainForm myform;
	Application::Run(% myform);
	return 0;
}

System::Void FlameDetect::MainForm::MainForm_Load(System::Object^ sender, System::EventArgs^ e)
{
	Method_comboBox->SelectedIndex = 0;
	Play_button->Enabled = false;
	VideoProcess_trackBar->Enabled = false;
	return System::Void();
}

System::Void FlameDetect::MainForm::openVideoToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	openFileDialog1->FileName = "";
	openFileDialog1->Filter = "Video File (*.avi, *.mp4)|*.avi;*.mp4";
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

System::Void FlameDetect::MainForm::Init()
{
	Video_timer->Enabled = true;
	Play_button->Text = "Pause";
	Play_button->Enabled = true;
	VideoProcess_trackBar->Enabled = true;
	SetVideoProcessBarInfo(true, 0);

	videoInfo.videoCap >> videoInfo.bg; // get first frame as background
	videoInfo.preFrame = videoInfo.input.clone(); // first, get frame to avoid bug

	return System::Void();
}

System::Void FlameDetect::MainForm::SetVideoProcessBarInfo(bool bIsInit, long posNow)
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

System::Void FlameDetect::MainForm::Play_button_Click(System::Object^ sender, System::EventArgs^ e)
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

System::Void FlameDetect::MainForm::VideoProcess_trackBar_Scroll(System::Object^ sender, System::EventArgs^ e)
{
	double now = (VideoProcess_trackBar->Value * 1.0f / VideoProcess_trackBar->Maximum) * videoInfo.frameCounts;
	videoInfo.videoCap.set(cv::CAP_PROP_POS_FRAMES, now);

	return System::Void();
}

System::Void FlameDetect::MainForm::Video_timer_Tick(System::Object^ sender, System::EventArgs^ e)
{	
	long posNow = videoInfo.videoCap.get(cv::CAP_PROP_POS_FRAMES);
	if (posNow >= videoInfo.frameCounts - 1)
	{
		videoInfo.videoCap.set(cv::CAP_PROP_POS_FRAMES, 0);
	}
	SetVideoProcessBarInfo(false, posNow);

	videoInfo.videoCap >> videoInfo.input;
	Origin_pictureBox->Image = ConvertMatToBitmap(videoInfo.input);

	videoInfo.process = videoInfo.input.clone();
	switch (Method_comboBox->SelectedIndex)
	{
		case 0: // Background Substraction
			Process_pictureBox->Image = BackgroundSubtraction(videoInfo.input, &videoInfo.process);
			break;
		case 1: // Frame Difference
			Process_pictureBox->Image = FrameDifference(videoInfo.input, &videoInfo.process);
			break;
	}

	cv::medianBlur(videoInfo.process, videoInfo.process, 7); 
	// to remove some noise in binary img. 
	// Originally I want to use my class "Filter.h", but its efficiency is to BAD.
	// so I use the function in OpenCV library.

	/* Get binary img for calculate bounding box */
	Bitmap^ binBmp = ConvertMatToBitmap(videoInfo.process);
	binBmp = DilationOrErosion(1, DilationOrErosion(0, binBmp));
	MarkFlamePixels(videoInfo.input, binBmp);
	
	Process_pictureBox->Image = IsProcessWhite_checkBox->Checked 
							? binBmp : GetFlamePixels(binBmp, (Bitmap^)Process_pictureBox->Image->Clone());

	/* Select Moving Objects */
	Result_pictureBox->Image = (Image^)Origin_pictureBox->Image->Clone();
	SelectObjects((Bitmap^)binBmp->Clone());

	videoInfo.preFrame = videoInfo.input.clone(); // get previous frame using to Frame Difference process

	return System::Void();
}

System::Void FlameDetect::MainForm::IsProcessWhite_checkBox_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
{
	if (IsProcessWhite_checkBox->Checked)
		Process_label->Text = "Binary Video\n(Red: Flame)";
	else
		Process_label->Text = "Flame Video";

	return System::Void();
}

Bitmap^ FlameDetect::MainForm::ConvertMatToBitmap(cv::Mat mat) 
{
	if (mat.channels() == 3) 
		return gcnew Bitmap(mat.cols, mat.rows, mat.step, PixelFormat::Format24bppRgb, (IntPtr)mat.data);
}

/*********************************** Moving Objects Segmentation ***********************************/

Bitmap^ FlameDetect::MainForm::BackgroundSubtraction(cv::Mat mat, cv::Mat* binMat)
{
	return GetDiff(mat, videoInfo.bg, binMat);
}

Bitmap^ FlameDetect::MainForm::FrameDifference(cv::Mat mat, cv::Mat* binMat)
{
	return GetDiff(mat, videoInfo.preFrame, binMat);
}

Bitmap^ FlameDetect::MainForm::GetDiff(cv::Mat mat, cv::Mat diffTarget, cv::Mat* binMat)
{
	Bitmap^ bmp = ConvertMatToBitmap(mat);
	Bitmap^ diffBmp = ConvertMatToBitmap(diffTarget);
	Bitmap^ out = gcnew Bitmap(bmp->Width, bmp->Height, bmp->PixelFormat);

	BitmapData^ bmpBD = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);
	BitmapData^ diffBmpBD = diffBmp->LockBits(Rectangle(0, 0, diffBmp->Width, diffBmp->Height), ImageLockMode::ReadWrite, diffBmp->PixelFormat);
	BitmapData^ outBD = out->LockBits(Rectangle(0, 0, out->Width, out->Height), ImageLockMode::ReadWrite, out->PixelFormat);

	int bmpByteSkip = bmpBD->Stride - bmpBD->Width * 3;
	int diffBmpByteSkip = diffBmpBD->Stride - diffBmpBD->Width * 3;
	int outByteSkip = outBD->Stride - outBD->Width * 3;

	Byte* bmpPtr = (Byte*)((void*)bmpBD->Scan0);
	Byte* diffBmpPtr = (Byte*)((void*)diffBmpBD->Scan0);
	Byte* outPtr = (Byte*)((void*)outBD->Scan0);

	for (int y = 0; y < bmpBD->Height; y++)
	{
		for (int x = 0; x < bmpBD->Width; x++)
		{
			/* Subtraction: originImg - diffTargetImg */
			int diff_b = bmpPtr[0] - diffBmpPtr[0];
			int diff_g = bmpPtr[1] - diffBmpPtr[1];
			int diff_r = bmpPtr[2] - diffBmpPtr[2];

			if (!(abs(diff_b) < DIVIATION
				&& abs(diff_g) < DIVIATION
				&& abs(diff_r) < DIVIATION))
			{
				outPtr[0] = bmpPtr[0];
				outPtr[1] = bmpPtr[1];
				outPtr[2] = bmpPtr[2];
				binMat->at<cv::Vec3b>(y, x) = cv::Vec3b(255, 255, 255);
			}
			else
			{
				outPtr[0] = outPtr[1] = outPtr[2] = 0;
				binMat->at<cv::Vec3b>(y, x) = cv::Vec3b(0, 0, 0);
			}

			bmpPtr += 3;
			diffBmpPtr += 3;
			outPtr += 3;
		}
		bmpPtr += bmpByteSkip;
		diffBmpPtr += diffBmpByteSkip;
		outPtr += outByteSkip;
	}

	bmp->UnlockBits(bmpBD);
	diffBmp->UnlockBits(diffBmpBD);
	out->UnlockBits(outBD);
	return out;
}

/****************************************** CCL Box Selection ******************************************/

System::Void FlameDetect::MainForm::SelectObjects(Bitmap^ binaryBmp)
{
	BitmapData^ binBmpBD = binaryBmp->LockBits(Rectangle(0, 0, binaryBmp->Width, binaryBmp->Height), ImageLockMode::ReadWrite, binaryBmp->PixelFormat);
	Byte* binBmpPtr = (Byte*)((void*)binBmpBD->Scan0);

	/* Connected Component Labeling Algorithm */

	int labelNum = 0;
	// Method 1: using iterative BFS to labeling
	labelNum = ConnectedComponentLabeling_BFS(binBmpBD, labelNum);

	// Method 2: get 1 target pixel, then using recursive DFS to labeling
	/*for (int y = 0; y < binBmpBD->Height; y++)
	{
		for (int x = 0; x < binBmpBD->Width; x++)
		{
			Byte* ptr = binBmpPtr + x * 3 + y * binBmpBD->Stride;
			if (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 255)
			{
				labelNum++;
				ptr[0] = ptr[1] = ptr[2] = labelNum;
				ConnectedComponentLabeling_DFS(binBmpBD, x, y, labelNum);
			}
		}
	}*/
	binaryBmp->UnlockBits(binBmpBD);

	for (int i = 1; i <= labelNum; i++)
		DrawArea(binaryBmp, i);

	return System::Void();
}

// using recursive DFS to labeling Connected-Component (sometimes Overflow)
System::Void FlameDetect::MainForm::ConnectedComponentLabeling_DFS(BitmapData^ bmpBD, int x, int y, int num)
{
	int threshold = 15;

	for (int i = (y - threshold < 0 ? 0 : y - threshold);
		i <= (y + threshold < bmpBD->Height ? y + threshold : bmpBD->Height - 1); i++)
	{
		for (int j = (x - threshold < 0 ? 0 : x - threshold);
			j <= (x + threshold < bmpBD->Width ? x + threshold : bmpBD->Width - 1); j++)
		{
			Byte* ptr = (Byte*)((void*)bmpBD->Scan0) + j * 3 + i * bmpBD->Stride;
			if (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 255) // finding neighbors is flame pixel
			{
				ptr[0] = ptr[1] = ptr[2] = num;
				ConnectedComponentLabeling_DFS(bmpBD, j, i, num); // find next
			}
		}
	}

	return System::Void();
}

// using iterative BFS to labeling Connected-Component (Avoid Overflow)
int FlameDetect::MainForm::ConnectedComponentLabeling_BFS(BitmapData^ bmpBD, int num)
{
	int threshold = 15;

	bool** isLabeled = new bool* [bmpBD->Height];
	for (int i = 0; i < bmpBD->Height; i++) isLabeled[i] = new bool[bmpBD->Width];

	for (int i = 0; i < bmpBD->Height; i++)
	{
		for (int j = 0; j < bmpBD->Width; j++)
		{
			Byte* ptr = (Byte*)((void*)bmpBD->Scan0) + j * 3 + i * bmpBD->Stride;
			if (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 255) isLabeled[i][j] = false;
			else isLabeled[i][j] = true;
		}
	}

	FlameDetect::Position loc, tmp;
	std::queue<FlameDetect::Position> q;

	for (int y = 0; y < bmpBD->Height; y++)
	{
		for (int x = 0; x < bmpBD->Width; x++)
		{
			Byte* ptr = (Byte*)((void*)bmpBD->Scan0) + x * 3 + y * bmpBD->Stride;
			if (!isLabeled[y][x] && (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 255)) // get a new target pixel
			{
				loc.row = y; loc.col = x;
				q.push(loc);
				isLabeled[y][x] = true;
				num++;
				ptr[0] = ptr[1] = ptr[2] = num;
			}
			else continue; // if isn't target pixel, then skip all

			while (!q.empty())
			{
				loc = q.front(); q.pop();

				// search in these range, purpose to combine overlaping region
				for (int i = (loc.row - threshold < 0 ? 0 : loc.row - threshold);
					i <= (loc.row + threshold < bmpBD->Height ? loc.row + threshold : bmpBD->Height - 1); i++)
				{
					for (int j = (loc.col - threshold < 0 ? 0 : loc.col - threshold);
						j <= (loc.col + threshold < bmpBD->Width ? loc.col + threshold : bmpBD->Width - 1); j++)
					{
						ptr = (Byte*)((void*)bmpBD->Scan0) + j * 3 + i * bmpBD->Stride;
						if (!isLabeled[i][j] && (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 255))
						{
							isLabeled[i][j] = true;
							tmp.row = i; tmp.col = j;
							ptr[0] = ptr[1] = ptr[2] = num;
							q.push(tmp);
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < bmpBD->Height; i++) delete[] isLabeled[i];
	delete[] isLabeled;

	return num;
}

System::Void FlameDetect::MainForm::DrawArea(Bitmap^ binaryBmp, int num)
{
	BitmapData^ binBmpBD = binaryBmp->LockBits(Rectangle(0, 0, binaryBmp->Width, binaryBmp->Height), ImageLockMode::ReadWrite, binaryBmp->PixelFormat);
	Byte* binBmpPtr = (Byte*)((void*)binBmpBD->Scan0);

	int left = binBmpBD->Width, right = 0;
	int top = binBmpBD->Height, bottom = 0;

	/* find the boundary */
	for (int y = 0; y < binBmpBD->Height; y++)
	{
		for (int x = 0; x < binBmpBD->Width; x++)
		{
			Byte* ptr = binBmpPtr + x * 3 + y * binBmpBD->Stride;
			if (ptr[0] == num && ptr[1] == num && ptr[2] == num)
			{
				if (x < left) left = x;
				if (x > right) right = x;
				if (y < top) top = y;
				if (y > bottom) bottom = y;
			}
		}
	}
	binaryBmp->UnlockBits(binBmpBD);

	int area = (right - left) * (bottom - top);
	if (area >= SMALLEST_DISPLAY_SELECT_AREA) // throwing some small area out, and drawing area
	{
		Graphics^ g = Graphics::FromImage(Result_pictureBox->Image);
		g->DrawRectangle(gcnew Pen(Color::Red, 2), left, top, right - left, bottom - top);
	}

	return System::Void();
}

/****************************************** Flame Detection ******************************************/

System::Void FlameDetect::MainForm::MarkFlamePixels(cv::Mat mat, Bitmap^ binaryBmp)
{
	Bitmap^ bmp = ConvertMatToBitmap(mat);
	BitmapData^ bmpBD = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);
	Byte* bmpPtr = (Byte*)((void*)bmpBD->Scan0);

	BitmapData^ binBmpBD = binaryBmp->LockBits(Rectangle(0, 0, binaryBmp->Width, binaryBmp->Height), ImageLockMode::ReadWrite, binaryBmp->PixelFormat);
	Byte* binBmpPtr = (Byte*)((void*)binBmpBD->Scan0);

	/* find the boundary */
	for (int y = 0; y < binBmpBD->Height; y++)
	{
		for (int x = 0; x < binBmpBD->Width; x++)
		{
			Byte* ptr = binBmpPtr + x * 3 + y * binBmpBD->Stride;
			if (ptr[0] == 255 && ptr[1] == 255 && ptr[2] == 255)
			{
				Byte* ptrOri = bmpPtr + x * 3 + y * bmpBD->Stride;
				if (IsFlamePixel(ptrOri))
				{
					ptr[0] = ptr[1] = 0;
					ptr[2] = 255;
				}
			}
		}
	}
	binaryBmp->UnlockBits(binBmpBD);

	return System::Void();
}

Bitmap^ FlameDetect::MainForm::GetFlamePixels(Bitmap^ markedBmp, Bitmap^ oriBmp)
{
	Bitmap^ bmp = gcnew Bitmap(oriBmp->Width, oriBmp->Height, oriBmp->PixelFormat);

	BitmapData^ bmpBD = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);
	BitmapData^ markedBmpBD = markedBmp->LockBits(Rectangle(0, 0, markedBmp->Width, markedBmp->Height), ImageLockMode::ReadWrite, markedBmp->PixelFormat);
	BitmapData^ oriBmpBD = oriBmp->LockBits(Rectangle(0, 0, oriBmp->Width, oriBmp->Height), ImageLockMode::ReadWrite, oriBmp->PixelFormat);

	Byte* bmpPtr = (Byte*)((void*)bmpBD->Scan0);
	Byte* markedBmpPtr = (Byte*)((void*)markedBmpBD->Scan0);
	Byte* oriBmpPtr = (Byte*)((void*)oriBmpBD->Scan0);

	int bmpByteSkip = bmpBD->Stride - bmpBD->Width * 3;

	for (int y = 0; y < markedBmpBD->Height; y++)
	{
		for (int x = 0; x < markedBmpBD->Width; x++)
		{
			Byte* ptr = markedBmpPtr + x * 3 + y * markedBmpBD->Stride;
			Byte* oriPtr = oriBmpPtr + x * 3 + y * oriBmpBD->Stride;
			if (ptr[0] == 0 && ptr[1] == 0 && ptr[2] == 255) // if it's flame pixel
			{
				bmpPtr[0] = oriPtr[0];
				bmpPtr[1] = oriPtr[1];
				bmpPtr[2] = oriPtr[2];
			}
			else bmpPtr[0] = bmpPtr[1] = bmpPtr[2] = 0;
			bmpPtr += 3;
		}
		bmpPtr += bmpByteSkip;
	}
	bmp->UnlockBits(bmpBD);
	markedBmp->UnlockBits(markedBmpBD);
	oriBmp->UnlockBits(oriBmpBD);

	return bmp;
}

bool FlameDetect::MainForm::IsFlamePixel(Byte* bmpPtr)
{
	bool metRule1 = false, metRule2 = false, metRule3 = false, metRule4 = false;
	unsigned char b = bmpPtr[0], g = bmpPtr[1], r = bmpPtr[2];

	if (r > R_T) metRule1 = true; // Rule 1: R > R_T
	if ((r >= g) && (g > b)) metRule2 = true; // Rule 2: R >= G > B

	if (r + g + b > 0) // avoid divide zero
	{
		// Rule 3: S >= (255 - R)* S_T / R_T
		double s = (1 - (3 * Math::Min(Math::Min(b, g), r) / (double)(r + g + b))) * 255;
		if (s >= (255 - r) * S_T / R_T) metRule3 = true;

		// Rule 4: I > I_T
		double i = (r + g + b) / 3;
		if (i > I_T) metRule4 = true;

		if (s <= 255) // through HSI Model to select brighter or darkerenvironment
		{
			if (s >= 30) // brighter environment
				return metRule1 && metRule2 && metRule3;

			if (s >= 20) // darker environment
				return metRule1 && metRule2 && metRule3 || metRule4;
		}
		return metRule1 && metRule2 && metRule3 || metRule4;
	}

	return false;
}

/****************************************** Morphology Operation ******************************************/

Bitmap^ FlameDetect::MainForm::DilationOrErosion(int mode, Bitmap^ bmp)
{
	BitmapData^ bmpBD = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);

	array<bool, 2>^ bIsTextPixel = gcnew array<bool, 2>(bmp->Height, bmp->Width); // to record the text pixel
	array<unsigned char, 2>^ matPixel = gcnew array<unsigned char, 2>(bmp->Height, bmp->Width); // to handle pixel

	GetOriginImgData(mode, bmpBD, bIsTextPixel, matPixel);
	HandlePixels(mode, bmpBD, bIsTextPixel, matPixel);

	bmp->UnlockBits(bmpBD);
	return CopyPixelArrToOutput(bmp, matPixel);
}

System::Void FlameDetect::MainForm::GetOriginImgData(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ matPixel)
{
	int bmpByteSkip = bmpBD->Stride - bmpBD->Width * 3;
	Byte* bmpPtr = (Byte*)((void*)bmpBD->Scan0);

	/* Record Image Data */
	for (int i = 0; i < bmpBD->Height; i++)
	{
		for (int j = 0; j < bmpBD->Width; j++)
		{
			matPixel[i, j] = bmpPtr[0];

			switch (mode)
			{
			case 0: // Delation
				if (matPixel[i, j] == 255)
					bIsTextPixel[i, j] = true;
				break;
			case 1: // Erosion
				if (matPixel[i, j] == 0)
					bIsTextPixel[i, j] = true;
				break;
			}
			bmpPtr += 3;
		}
		bmpPtr += bmpByteSkip;
	}
	return System::Void();
}

System::Void FlameDetect::MainForm::HandlePixels(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ matPixel)
{
	int threshold = 3;

	for (int i = threshold; i < bmpBD->Height - threshold; i++)
		for (int j = threshold; j < bmpBD->Width - threshold; j++)
			if (bIsTextPixel[i, j])
				for (int y = i - threshold; y <= i + threshold; y++)
					for (int x = j - threshold; x <= j + threshold; x++)
						matPixel[y, x] = mode == 0 ? 255 : 0;

	return System::Void();
}

Bitmap^ FlameDetect::MainForm::CopyPixelArrToOutput(Bitmap^ bmp, array<unsigned char, 2>^ pixelArray)
{
	Bitmap^ output = gcnew Bitmap(bmp->Width, bmp->Height, bmp->PixelFormat);
	BitmapData^ outputBD = output->LockBits(Rectangle(0, 0, output->Width, output->Height), ImageLockMode::ReadWrite, output->PixelFormat);
	int outputByteSkip = outputBD->Stride - outputBD->Width * 3;
	Byte* outputPtr = (Byte*)((void*)outputBD->Scan0);

	for (int i = 0; i < output->Height; i++)
	{
		for (int j = 0; j < output->Width; j++)
		{
			outputPtr[0] = outputPtr[1] = outputPtr[2] = pixelArray[i, j];
			outputPtr += 3;
		}
		outputPtr += outputByteSkip;
	}

	output->UnlockBits(outputBD);
	return output;
}