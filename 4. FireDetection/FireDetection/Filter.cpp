#include "Filter.h"
#include <vector>
#include <algorithm>

Bitmap^ Filter::LowpassFilter(int maskSize, Bitmap^ noiseImg)
{
	Bitmap^ result = gcnew Bitmap(noiseImg->Width, noiseImg->Height, noiseImg->PixelFormat);

	BitmapData^ noiseImgBD = noiseImg->LockBits(Rectangle(0, 0, noiseImg->Width, noiseImg->Height), ImageLockMode::ReadOnly, noiseImg->PixelFormat);
	BitmapData^ resultBD = result->LockBits(Rectangle(0, 0, result->Width, result->Height), ImageLockMode::ReadWrite, result->PixelFormat);

	/* Image maybe have unknown information */
	int resultByteskip = resultBD->Stride - resultBD->Width * 3;

	Byte* noiseImgPtr = (Byte*)((void*)noiseImgBD->Scan0);
	Byte* resultPtr = (Byte*)((void*)resultBD->Scan0);

	array<array<int>^>^ mask = gcnew array<array<int>^>(maskSize);
	for (int i = 0; i < maskSize; i++) mask[i] = gcnew array<int>(maskSize);

	for (int i = 0; i < maskSize; i++)
		for (int j = 0; j < maskSize; j++)
			mask[i][j] = 1;

	int blank = maskSize / 2;

	for (int y = 0; y < noiseImgBD->Height; y++)
	{
		for (int x = 0; x < noiseImgBD->Width; x++)
		{
			// avoid out of the range 
			if (x >= blank && x < noiseImgBD->Width - blank
				&& y >= blank && y < noiseImgBD->Height - blank)
			{
				int bSum = 0, gSum = 0, rSum = 0;
				for (int i = -blank, index_i = 0; i <= blank; i++)
				{
					for (int j = -blank, index_j = 0; j <= blank; j++)
					{
						Byte* ptr = noiseImgPtr + (x + j) * 3 + (y + i) * noiseImgBD->Stride;
						bSum += ptr[0] * mask[index_i][index_j];
						gSum += ptr[1] * mask[index_i][index_j];
						rSum += ptr[2] * mask[index_i][index_j];
					}
				}

				resultPtr[0] = bSum / (maskSize * maskSize);
				resultPtr[1] = gSum / (maskSize * maskSize);
				resultPtr[2] = rSum / (maskSize * maskSize);
			}
			else // if is edge
			{
				Byte* ptr = noiseImgPtr + x * 3 + y * noiseImgBD->Stride;
				resultPtr[0] = ptr[0];
				resultPtr[1] = ptr[1];
				resultPtr[2] = ptr[2];
			}

			resultPtr += 3;
		}
		resultPtr += resultByteskip;
	}

	noiseImg->UnlockBits(noiseImgBD);
	result->UnlockBits(resultBD);
	return result;
}

Bitmap^ Filter:: MedianFilter(int maskSize, Bitmap^ noiseImg)
{
	Bitmap^ result = gcnew Bitmap(noiseImg->Width, noiseImg->Height, noiseImg->PixelFormat);

	BitmapData^ noiseImgBD = noiseImg->LockBits(Rectangle(0, 0, noiseImg->Width, noiseImg->Height), ImageLockMode::ReadOnly, noiseImg->PixelFormat);
	BitmapData^ resultBD = result->LockBits(Rectangle(0, 0, result->Width, result->Height), ImageLockMode::ReadWrite, result->PixelFormat);

	/* Image maybe have unknown information */
	int resultByteskip = resultBD->Stride - resultBD->Width * 3;

	Byte* noiseImgPtr = (Byte*)((void*)noiseImgBD->Scan0);
	Byte* resultPtr = (Byte*)((void*)resultBD->Scan0);

	int blank = maskSize / 2;
	int median = maskSize * maskSize / 2;

	for (int y = 0; y < noiseImgBD->Height; y++)
	{
		for (int x = 0; x < noiseImgBD->Width; x++)
		{
			// avoid out of the range 
			if (x >= blank && x < noiseImgBD->Width - blank
				&& y >= blank && y < noiseImgBD->Height - blank)
			{
				// vector template needs import library "vector"
				std::vector<int> b;
				std::vector<int> g;
				std::vector<int> r;

				for (int i = -blank; i <= blank; i++)
				{
					for (int j = -blank; j <= blank; j++)
					{
						Byte* ptr = noiseImgPtr + (x + j) * 3 + (y + i) * noiseImgBD->Stride;
						b.push_back(ptr[0]);
						g.push_back(ptr[1]);
						r.push_back(ptr[2]);
					}
				}

				// sort function needs import library "Algorithm"
				std::sort(b.begin(), b.end());
				std::sort(g.begin(), g.end());
				std::sort(r.begin(), r.end());

				resultPtr[0] = b[median];
				resultPtr[1] = g[median];
				resultPtr[2] = r[median];
			}
			else // if is edge
			{
				Byte* ptr = noiseImgPtr + x * 3 + y * noiseImgBD->Stride;
				resultPtr[0] = ptr[0];
				resultPtr[1] = ptr[1];
				resultPtr[2] = ptr[2];
			}

			resultPtr += 3;
		}
		resultPtr += resultByteskip;
	}

	noiseImg->UnlockBits(noiseImgBD);
	result->UnlockBits(resultBD);
	return result;
}

Bitmap^ Filter::AlphaTrimmedMeanFilter(int maskSize, int T, Bitmap^ noiseImg)
{
	Bitmap^ result = gcnew Bitmap(noiseImg->Width, noiseImg->Height, noiseImg->PixelFormat);

	BitmapData^ noiseImgBD = noiseImg->LockBits(Rectangle(0, 0, noiseImg->Width, noiseImg->Height), ImageLockMode::ReadOnly, noiseImg->PixelFormat);
	BitmapData^ resultBD = result->LockBits(Rectangle(0, 0, result->Width, result->Height), ImageLockMode::ReadWrite, result->PixelFormat);

	/* Image maybe have unknown information */
	int resultByteskip = resultBD->Stride - resultBD->Width * 3;

	Byte* noiseImgPtr = (Byte*)((void*)noiseImgBD->Scan0);
	Byte* resultPtr = (Byte*)((void*)resultBD->Scan0);

	int blank = maskSize / 2;

	for (int y = 0; y < noiseImgBD->Height; y++)
	{
		for (int x = 0; x < noiseImgBD->Width; x++)
		{
			// avoid out of the range 
			if (x >= blank && x < noiseImgBD->Width - blank
				&& y >= blank && y < noiseImgBD->Height - blank)
			{
				// vector template needs import library "vector"
				std::vector<int> b;
				std::vector<int> g;
				std::vector<int> r;

				for (int i = -blank; i <= blank; i++)
				{
					for (int j = -blank; j <= blank; j++)
					{
						Byte* ptr = noiseImgPtr + (x + j) * 3 + (y + i) * noiseImgBD->Stride;
						b.push_back(ptr[0]);
						g.push_back(ptr[1]);
						r.push_back(ptr[2]);
					}
				}

				// sort function needs import library "Algorithm"
				std::sort(b.begin(), b.end());
				std::sort(g.begin(), g.end());
				std::sort(r.begin(), r.end());

				int bSum = 0, gSum = 0, rSum = 0;
				for (int index = T; index < maskSize * maskSize - T; index++)
				{
					bSum += b[index];
					gSum += g[index];
					rSum += r[index];
				}

				resultPtr[0] = bSum / (maskSize * maskSize - 2 * T);
				resultPtr[1] = gSum / (maskSize * maskSize - 2 * T);
				resultPtr[2] = rSum / (maskSize * maskSize - 2 * T);
			}
			else // if is edge
			{
				Byte* ptr = noiseImgPtr + x * 3 + y * noiseImgBD->Stride;
				resultPtr[0] = ptr[0];
				resultPtr[1] = ptr[1];
				resultPtr[2] = ptr[2];
			}

			resultPtr += 3;
		}
		resultPtr += resultByteskip;
	}

	noiseImg->UnlockBits(noiseImgBD);
	result->UnlockBits(resultBD);
	return result;
}