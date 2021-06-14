//
// ContagionPage.xaml.h
// ContagionPage 类的声明
//

#pragma once

#include "Grid.h"
#include "ContagionPage.g.h"

namespace AlgorithmVisualization
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContagionPage sealed
	{
	public:
		ContagionPage();
	private:
		void ContagionGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void DebugBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		double lastGridWidth = 800.0; //上次柱状图宽度
		double lastGridHeight = 400.0; //上次柱状图高度

		Grid^ InfectiousGrid; //传染病网格
	};
}
