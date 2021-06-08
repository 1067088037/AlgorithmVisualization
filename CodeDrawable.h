#pragma once

using namespace std;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace AlgorithmVisualization;
using namespace Windows::UI::Xaml::Documents;

namespace AlgorithmVisualization {
	/// <summary>
	/// 可绘制的代码
	/// </summary>
	ref class CodeDrawable sealed
	{
	public:
		CodeDrawable();
		IVector<Object^>^ GenerateDrawable(int highLightLine); //生成可绘制的代码
		IVector<Object^>^ GenerateDrawable(IVector<int>^ highLightLines); //生成可绘制的代码

		property IVector<String^>^ Texts;
	
	private:
		Color HighlighterColor = Colors::LightGoldenrodYellow;
	};
}
