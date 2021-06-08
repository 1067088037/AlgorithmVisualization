#include "pch.h"
#include "CodeDrawable.h"

using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace AlgorithmVisualization;
using namespace Windows::UI::Xaml::Documents;
using namespace Windows::UI::Xaml::Media;

AlgorithmVisualization::CodeDrawable::CodeDrawable()
{
	Texts = ref new Vector<String^>();
}

IVector<Object^>^ AlgorithmVisualization::CodeDrawable::GenerateDrawable(int highLightLine)
{
	return GenerateDrawable(ref new Vector<int>{ highLightLine });
}

IVector<Object^>^ AlgorithmVisualization::CodeDrawable::GenerateDrawable(IVector<int>^ highLightLine)
{
	auto result = ref new Vector<Object^>();
	auto para = ref new Paragraph();
	auto highlighter = ref new TextHighlighter();
	int textCount = 0;
	for (int i = 0; i < Texts->Size; ++i)
	{
		unsigned int n;
		highLightLine->IndexOf(i, &n);
		Run^ run = ref new Run();
		run->Text = Texts->GetAt(i);
		int currentLength = run->Text->Length();
		auto textRange = TextRange{ textCount, currentLength };
		textCount += currentLength;
		if (n == 0)
		{
			//run->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Colors::Red);
			run->FontWeight = Windows::UI::Text::FontWeights::Bold;
			highlighter->Background = ref new SolidColorBrush(HighlighterColor);
			highlighter->Ranges->Append(textRange);
		}
		para->Inlines->Append(run);
	}
	result->Append(para);
	result->Append(highlighter);
	return result;
}
