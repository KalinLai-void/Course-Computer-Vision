using namespace System;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Drawing::Design;

// Origin I want to use these filter (write myself)
// but its efficiency is to BAD

class Filter
{
public:
	static Bitmap^ LowpassFilter(int maskSize, Bitmap^ noiseImg);
	static Bitmap^ MedianFilter(int maskSize, Bitmap^ noiseImg);
	static Bitmap^ AlphaTrimmedMeanFilter(int maskSize, int T, Bitmap^ noiseImg);
};