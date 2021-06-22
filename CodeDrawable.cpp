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

/// <summary>
/// 构造函数
/// </summary>
AlgorithmVisualization::CodeDrawable::CodeDrawable()
{
	Texts = ref new Vector<String^>(); //实例化文本向量
}

/// <summary>
/// 生成可绘制的代码
/// </summary>
/// <param name="highLightLine">高亮行数</param>
/// <returns>[0]是段落，[1]是高亮</returns>
IVector<Object^>^ AlgorithmVisualization::CodeDrawable::GenerateDrawable(int highLightLine)
{
	return GenerateDrawable(ref new Vector<int>{ highLightLine }); //以只有1个向量的参数调用
}

/// <summary>
/// 生成可绘制的代码
/// </summary>
/// <param name="highLightLine">多个高亮行</param>
/// <returns>[0]是段落，[1]是高亮</returns>
IVector<Object^>^ AlgorithmVisualization::CodeDrawable::GenerateDrawable(IVector<int>^ highLightLine)
{
	auto result = ref new Vector<Object^>(); //结果向量
	auto para = ref new Paragraph(); //段落
	auto highlighter = ref new TextHighlighter(); //文本高亮器
	int textCount = 0; //文本计数
	for (unsigned int i = 0; i < Texts->Size; ++i)
	{
		unsigned int n;
		highLightLine->IndexOf(i, &n); //查找i的位置
		Run^ run = ref new Run();
		run->Text = Texts->GetAt(i); //获取文字
		int currentLength = run->Text->Length();
		auto textRange = TextRange{ textCount, currentLength }; //创建矩形域
		textCount += currentLength; //增加当前的长度
		if (n == 0)
		{
			//run->Foreground = ref new Windows::UI::Xaml::Media::SolidColorBrush(Colors::Red);
			run->FontWeight = Windows::UI::Text::FontWeights::Bold; //设置字重
			highlighter->Background = ref new SolidColorBrush(HighlighterColor); //设置背景
			highlighter->Ranges->Append(textRange); //加入到高亮器中
		}
		para->Inlines->Append(run); //追加进入段落
	}
	result->Append(para); //向结果中追加段落
	result->Append(highlighter); //向结果中追加高亮器
	return result;
}
