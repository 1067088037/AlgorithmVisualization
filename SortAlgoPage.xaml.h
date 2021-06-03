//
// SortAlgoPage.xaml.h
// SortAlgoPage 类的声明
//

#pragma once

#include "SortAlgoPage.g.h"
#include "Histogram.h"
#include <vector>
#include "ExecuteStep.h"

using namespace std;

namespace AlgorithmVisualization
{
	ref class SortExcute sealed : public ExecuteStep
	{
	public:
		// 通过 ExecuteStep 继承
		virtual SingleStep^ NavigateToStep(int index) override;
		virtual SingleStep^ NavigateToNext() override;
		virtual SingleStep^ NavigateToPrevious() override;

		SortExcute();

		property Histogram^ histogram; //柱状图
		property IVector<int>^ sortVector; //要排序的向量
	};
	
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class SortAlgoPage sealed
	{
	public:
		SortAlgoPage();
	private:
		void SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void InitAlgorithm(String^ tag);
		void InitNavViewItems(int sortAlgorithmType);

		SortExcute^ executor;
	};
}
