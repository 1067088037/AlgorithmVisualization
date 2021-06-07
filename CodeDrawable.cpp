#include "pch.h"
#include "CodeDrawable.h"

using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace AlgorithmVisualization;
using namespace Windows::UI::Xaml::Documents;

AlgorithmVisualization::CodeDrawable::CodeDrawable()
{
	Texts = ref new Vector<String^>();
}

Paragraph^ AlgorithmVisualization::CodeDrawable::GenerateDrawable(int highLightLine)
{
	return GenerateDrawable(ref new Vector<int>{ highLightLine });
}

Paragraph^ AlgorithmVisualization::CodeDrawable::GenerateDrawable(IVector<int>^ highLightLine)
{
	auto para = ref new Paragraph();
	for (int i = 0; i < Texts->Size; ++i)
	{
		unsigned int n;
		highLightLine->IndexOf(i, &n);
		Run^ run = ref new Run();
		run->Text = Texts->GetAt(i);
		if (n == 0)
		{
			run->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Colors::Red);
			run->FontWeight = Windows::UI::Text::FontWeights::Bold;
		}
		para->Inlines->Append(run);
	}
	return para;
}
