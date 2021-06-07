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
using namespace Windows::UI::Xaml::Controls;

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
		property TextBlock^ ProcessText; //过程文本
		property Slider^ ProgressSlider; //过程滑块
		property TextBlock^ SpeedText; //速度文本
		property Slider^ SpeedSlider; //速度滑块
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
		void Debug_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void InitAlgorithm(String^ tag);
		void InitNavViewItems(int sortAlgorithmType);
		void Debug(String^ message);

		SortExcute^ executor;
		bool LooperIsRunning = false;
		
		void Previous_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Pause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Next_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void AddCompareStep(IVector<int>^ stateList, int left, int right); //新建比较步骤
		void AddSwapStep(IVector<int>^ stateList, int left, int right); //新建交换步骤
		void AddRecoverStep(IVector<int>^ stateList, IVector<int>^ recover); //新建恢复成默认状态的步骤
		void AddCompleteStep(IVector<int>^ stateList, IVector<int>^ complete); //新建完成的步骤
		void ProgressSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void SpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
	};
}
