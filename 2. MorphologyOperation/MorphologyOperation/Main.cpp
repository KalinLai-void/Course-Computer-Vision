#include "Main.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Design;

[STAThreadAttribute]
int main(void) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MorphologyOperation::Main form;
	Application::Run(%form);
	return 0;
}

System::Void MorphologyOperation::Main::Main_Load(System::Object^ sender, System::EventArgs^ e)
{
	Mode_comboBox->SelectedIndex = 0;
	return System::Void();
}

System::Void MorphologyOperation::Main::openImageToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	OpenFileDialog^ ofd = gcnew OpenFileDialog();
	ofd->FileName = "";
	ofd->Filter = "BMP Files(*.bmp)|*.bmp";
	ofd->FilterIndex = 1;
	if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK && ofd->FileName != "") {
		Bitmap^ bmp = gcnew Bitmap(ofd->FileName);
		Origin_pictureBox->Image = bmp;
		Result_pictureBox->Image = Binarize((Bitmap^)Origin_pictureBox->Image);
	}
	return System::Void();
}

System::Void MorphologyOperation::Main::resetImgToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
{
	Result_pictureBox->Image = Binarize((Bitmap^)Origin_pictureBox->Image);
	return System::Void();
}

System::Void MorphologyOperation::Main::RunBtn_Click(System::Object^ sender, System::EventArgs^ e)
{
	switch (Mode_comboBox->SelectedIndex)
	{
		case 0: // Dilation
		case 1: // Erosion
			Result_pictureBox->Image = DilationOrErosion(Mode_comboBox->SelectedIndex, (Bitmap^)Result_pictureBox->Image);
			break;
		case 2: // Opening
			Result_pictureBox->Image = DilationOrErosion(0, DilationOrErosion(1, (Bitmap^)Result_pictureBox->Image));
			break;
		case 3: // Closing
			Result_pictureBox->Image = DilationOrErosion(1, DilationOrErosion(0, (Bitmap^)Result_pictureBox->Image));
			break;
	}
	return System::Void();
}

Bitmap^ MorphologyOperation::Main::Binarize(Bitmap^ bmp)
{
	Bitmap^ output = gcnew Bitmap(bmp->Width, bmp->Height, bmp->PixelFormat);

	BitmapData^ bmpBD = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);
	BitmapData^ outputBD = output->LockBits(Rectangle(0, 0, output->Width, output->Height), ImageLockMode::ReadWrite, output->PixelFormat);
	
	int bmpByteSkip = bmpBD->Stride - bmpBD->Width * 3;
	int outputByteSkip = outputBD->Stride - outputBD->Width * 3;
	
	Byte* bmpPtr = (Byte*)((void*)bmpBD->Scan0);
	Byte* outputPtr = (Byte*)((void*)outputBD->Scan0);

	for (int i = 0; i < bmpBD->Height; i++)
	{
		for (int j = 0; j < bmpBD->Width; j++)
		{
			unsigned char gray = 0.114 * bmpPtr[0] + 0.587 * bmpPtr[1] + 0.299 * bmpPtr[2];
			outputPtr[0] = outputPtr[1] = outputPtr[2] = (gray > 127 ? 255 : 0);

			bmpPtr += 3;
			outputPtr += 3;
		}
		bmpPtr += bmpByteSkip;
		outputPtr += outputByteSkip;
	}

	bmp->UnlockBits(bmpBD);
	output->UnlockBits(outputBD);
	return output;
}

Bitmap^ MorphologyOperation::Main::DilationOrErosion(int mode, Bitmap^ bmp)
{
	BitmapData^ bmpBD = bmp->LockBits(Rectangle(0, 0, bmp->Width, bmp->Height), ImageLockMode::ReadWrite, bmp->PixelFormat);

	array<bool, 2>^ bIsTextPixel = gcnew array<bool, 2>(bmp->Height, bmp->Width); // to record the text pixel
	array<unsigned char, 2>^ bmpPixel = gcnew array<unsigned char, 2>(bmp->Height, bmp->Width); // to handle pixel

	GetOriginImgData(mode, bmpBD, bIsTextPixel, bmpPixel);
	HandlePixels(mode, bmpBD, bIsTextPixel, bmpPixel);

	bmp->UnlockBits(bmpBD);
	return CopyPixelArrToOutput(bmp, bmpPixel);
}

System::Void MorphologyOperation::Main::GetOriginImgData(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ bmpPixel)
{
	int bmpByteSkip = bmpBD->Stride - bmpBD->Width * 3;
	Byte* bmpPtr = (Byte*)((void*)bmpBD->Scan0);

	/* Record Image Data */
	for (int i = 0; i < bmpBD->Height; i++)
	{
		for (int j = 0; j < bmpBD->Width; j++)
		{
			bmpPixel[i, j] = bmpPtr[0];

			switch (mode)
			{
				case 0: // Delation
					if (WhiteText_radioButton->Checked) // ¶Â©³¥Õ¦r
					{
						if (bmpPixel[i, j] == 255)
						{
							bIsTextPixel[i, j] = true;
						}
					}
					else if (BlackText_radioButton->Checked) // ¥Õ©³¶Â¦r
					{
						if (bmpPixel[i, j] == 0)
						{
							bIsTextPixel[i, j] = true;
						}
					}
					break;
				case 1: // Erosion
					if (WhiteText_radioButton->Checked) // ¶Â©³¥Õ¦r
					{
						if (bmpPixel[i, j] == 0)
						{
							bIsTextPixel[i, j] = true;
						}
					}
					else if (BlackText_radioButton->Checked) // ¥Õ©³¶Â¦r
					{
						if (bmpPixel[i, j] == 255)
						{
							bIsTextPixel[i, j] = true;
						}
					}
					break;
			}

			bmpPtr += 3;
		}
		bmpPtr += bmpByteSkip;
	}
	return System::Void();
}

System::Void MorphologyOperation::Main::HandlePixels(int mode, BitmapData^ bmpBD, array<bool, 2>^ bIsTextPixel, array<unsigned char, 2>^ bmpPixel)
{
	int threshold = (int)Threshold_numericUpDown->Value;

	for (int i = threshold; i < bmpBD->Height - threshold; i++)
	{
		for (int j = threshold; j < bmpBD->Width - threshold; j++)
		{
			if (bIsTextPixel[i, j])
			{
				for (int y = i - threshold; y <= i + threshold; y++)
				{
					for (int x = j - threshold; x <= j + threshold; x++)
					{
						if (WhiteText_radioButton->Checked) // ¶Â©³¥Õ¦r
						{
							bmpPixel[y, x] = mode == 0 ? 255 : 0;
						}
						else if (BlackText_radioButton->Checked) // ¥Õ©³¶Â¦r
						{
							bmpPixel[y, x] = mode == 0 ? 0 : 255;
						}
					}
				}
			}
		}
	}
	return System::Void();
}

Bitmap^ MorphologyOperation::Main::CopyPixelArrToOutput(Bitmap^ bmp, array<unsigned char, 2>^ pixelArray)
{
	Bitmap^ output = gcnew Bitmap(bmp->Width, bmp->Height, bmp->PixelFormat);
	BitmapData^ outputBD = output->LockBits(Rectangle(0, 0, output->Width, output->Height), ImageLockMode::ReadWrite, output->PixelFormat);
	int outputByteSkip = outputBD->Stride - outputBD->Width * 3;
	Byte* outputPtr = (Byte*)((void*)outputBD->Scan0);

	for (int i = 0; i < outputBD->Height; i++)
	{
		for (int j = 0; j < outputBD->Width; j++)
		{
			outputPtr[0] = outputPtr[1] = outputPtr[2] = pixelArray[i, j];

			outputPtr += 3;
		}
		outputPtr += outputByteSkip;
	}

	
	output->UnlockBits(outputBD);
	return output;
}

