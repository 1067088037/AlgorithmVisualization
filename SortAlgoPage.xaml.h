//
// SortAlgoPage.xaml.h
// SortAlgoPage 类的声明
//

#pragma once

#include "SortAlgoPage.g.h"
#include "Histogram.h"
#include <vector>
#include "ExecuteStep.h"
#include "CodeDrawable.h"

using namespace std;
using namespace Windows::UI::Xaml::Controls;

namespace AlgorithmVisualization
{
	/// <summary>
	/// 执行步骤的派生类：排序算法可执行
	/// </summary>
	ref class SortExcute sealed : public ExecuteStep
	{
	public:
		// 通过 ExecuteStep 继承
		virtual SingleStep^ NavigateToStep(int index) override; //导航到步骤
		virtual SingleStep^ NavigateToNext() override; //导航到下一步
		virtual SingleStep^ NavigateToPrevious() override; //导航到上一步

		SortExcute(int n, float width, float height, bool needAssist = true); //构造函数
		void ShowCodeChange(IVector<int>^ highlighters); //展示代码的改变

		property Histogram^ MainHistogram; //柱状图
		property Histogram^ AssistHistogram; //辅助柱状图
		
		property bool NeedAssistHistogram; //是否需要辅助柱状图
		
		property IVector<int>^ SortVector; //要排序的向量
		property IVector<int>^ AssistVector; //辅助向量
		property IVector<int>^ SortStates; //要排序的状态
		property IVector<int>^ AssistStates; //辅助的状态
		
		property TextBlock^ ProcessText; //过程文本
		property Slider^ ProgressSlider; //过程滑块
		property TextBlock^ SpeedText; //速度文本
		property Slider^ SpeedSlider; //速度滑块
		property RichTextBlock^ CodeText; //代码文本
		property CodeDrawable^ CodeDrawable; //可绘制的代码
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
		SortExcute^ Executor; //算法执行器
		int AlgorithmType = -1; //排序算法类型
		const float defaultHeight = 410.0; //默认高度
		float lastHistogramWidth = 980.0; //上次柱状图宽度
		float lastHistogramHeight = defaultHeight; //上次柱状图高度

		void InitAlgorithm(String^ tag); //初始化算法
		void InitNavViewItems(int sortAlgorithmType); //初始化导航栏
		void InitExecutor(); //初始化执行器
		void Debug(String^ message); //显示调试信息
		void StartThreadTimer(); //开始计时器线程

		void InitAssistStep(SingleStep^ step); //初始化辅助步骤
		void SwapInVector(IVector<int>^ arr, int left, int right); //交换向量中数字
		bool AddEmptyStep(IVector<int>^ isTemp = ref new Vector<int>()); //添加空步骤
		bool AddSetStep(int pos, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //添加设置步骤
		bool AddSetFromToStep(int from, int to, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //添加有来源的步骤
		bool AddSetFromToStep(int fromHis, int fromPos, int toHis, int toPos, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //新建跨柱状图的有来源步骤
		bool AddCompareStep(int left, int right, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //新建比较步骤
		bool AddCompareStep(int leftHisIndex, int leftPos, int rightHisIndex, int rightPos, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //新建跨柱状图的比较
		bool AddSwapStep(int left, int right, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //新建交换步骤
		bool AddSwapStep(int leftHisIndex, int leftPos, int rightHisIndex, int rightPos, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //新建跨柱状图的交换
		bool SetToDefault(int his0, int index0, int his1, int index1); //设置为默认状态
		bool AddRecoverStep(IVector<int>^ recover, IVector<int>^ isTemp = ref new Vector<int>()); //新建恢复成默认状态的步骤
		bool AddCompleteStep(IVector<int>^ complete, IVector<int>^ isTemp = ref new Vector<int>()); //新建完成的步骤
		bool AddSelectStep(IVector<int>^ select, int highlightLine, IVector<int>^ isTemp = ref new Vector<int>()); //添加选中的步骤
		bool AddAllCompleteStep(IVector<int>^ isTemp = ref new Vector<int>()); //全部完成
		bool AddAllCompleteStep(int hisIndex, IVector<int>^ isTemp = ref new Vector<int>()); //全部完成
		
		void InitBubbleSort(); //初始化冒泡排序
		void InitSelectionSort(); //初始化选择排序
		void InitInsertionSort(); //初始化插入排序
		void InitShellSort(); //初始化希尔排序
		void InitMergeSort(); //初始化归并排序
		void Merge(IVector<int>^ data, int start, int end, IVector<int>^ result); //分割
		void MergeSort(IVector<int>^ data, int start, int end, IVector<int>^ result); //归并排序
		void InitQuickSort(); //初始化快速排序
		int Partition(IVector<int>^ arr, int left, int right); //分割
		void QuickSort(IVector<int>^ arr, int left, int right); //快速排序
		void InitHeapSort(); //初始化堆排序
		void MaxHeapify(IVector<int>^ arr, int start, int end); //最大堆化
		void HeapSort(IVector<int>^ arr); //堆排序
		void InitCountingSort(); //初始化计数排序
		void InitBucketSort(); //初始化桶排序
		void InitRadixSort(); //初始化基数排序
		int Maxbit(IVector<int>^ data); //最大位数
		void RadixSort(IVector<int>^ data); //基数排序
		
		void Previous_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Pause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Next_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args);
		void Debug_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void ProgressSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void SpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
	};
}
