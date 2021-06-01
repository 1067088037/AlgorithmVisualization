//
// SortAlgoPage.xaml.h
// SortAlgoPage 类的声明
//

#pragma once

#include "SortAlgoPage.g.h"
#include "Histogram.h"
#include <vector>

using namespace std;

namespace AlgorithmVisualization
{
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SortAlgoPage sealed
	{
	public:
		SortAlgoPage();
	private:
		Histogram^ histogram; //柱状图
		vector<int> sortVector; //要排序的向量

		void SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void SortHistogramStack_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
	};
}
