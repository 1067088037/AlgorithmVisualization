//
// SortAlgoPage.xaml.cpp
// SortAlgoPage 类的实现
//

#include "pch.h"
#include "SortAlgoPage.xaml.h"
#include "Histogram.h"
#include <vector>

using namespace AlgorithmVisualization;

using namespace Platform;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Shapes;
using namespace Microsoft::UI::Xaml::Media;
using namespace Windows::UI;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

/// <summary>
/// 导航栏项目
/// </summary>
struct NavigateItem
{
	String^ Tag;
	String^ Content;
};
const vector<NavigateItem> PrimarySortNavItems = {
	NavigateItem{L"BubbleSort", L"冒泡排序"},
	NavigateItem{L"SelectionSort", L"选择排序"},
	NavigateItem{L"InsertionSort", L"插入排序"},
	NavigateItem{L"ShellSort", L"希尔排序"}
}; //初等排序导航项目
const vector<NavigateItem> AdvancedComparativeSortNavItems = {
	NavigateItem{L"MergeSort", L"归并排序"},
	NavigateItem{L"QuickSort", L"快速排序"},
	NavigateItem{L"HeapSort", L"堆排序"}
}; //高等比较排序导航项目
const vector<NavigateItem> AdvancedNonComparativeSortNavItems = {
	NavigateItem{L"CountingSort", L"计数排序"},
	NavigateItem{L"BucketSort", L"桶排序"},
	NavigateItem{L"RadixSort", L"基数排序"}
}; //高等非比较排序导航项目

SortAlgoPage::SortAlgoPage()
{
	InitializeComponent();
	histogram = ref new Histogram{}; //初始化容器
	SortHistogram->Children->Append(histogram->container); //向父级容器中添加柱状图的容器
	InitNavViewItems(((App^)(Application::Current))->sortAlgorithmType); //初始化导航栏

	sortVector.clear();
	auto vec = ref new Vector<int>();
	int size = 12;
	for (int i = 0; i < size; ++i)
	{
		int value = rand() % 99 + 1;
		sortVector.push_back(value);
		vec->Append(value);
	}
	histogram->load(vec);
}


void AlgorithmVisualization::SortAlgoPage::SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SortNavView->SelectedItem = SortNavView->MenuItems->GetAt(0);
}


void AlgorithmVisualization::SortAlgoPage::SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	auto tag = args->InvokedItemContainer->Tag->ToString();
	if (tag == "BubbleSort") {
		AlgorithmName->Text = "冒泡排序";
	}
	else if (tag == "SelectionSort")
	{
		AlgorithmName->Text = "选择排序";
	}
	else if (tag == "InsertionSort")
	{
		AlgorithmName->Text = "插入排序";
	}
	else if (tag == "ShellSort")
	{
		AlgorithmName->Text = "希尔排序";
	}
	else if (tag == "MergeSort")
	{

	}
	else if (tag == "QuickSort")
	{

	}
	else if (tag == "HeapSort")
	{

	}
	else if (tag == "CountingSort")
	{

	}
	else if (tag == "BucketSort")
	{

	}
	else if (tag == "RadixSort")
	{

	}
}


void AlgorithmVisualization::SortAlgoPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto workTask = ref new WorkItemHandler([this](IAsyncAction^ workItem)
		{
			auto n = sortVector.size();
			for (auto i = 0; i < n - 1; i++)
			{
				auto j = 0;
				for (j = 0; j < n - i - 1; j++)
				{
					histogram->compareOnUI(j, j + 1);
					Sleep(200);
					if (sortVector[j] > sortVector[j + 1]) {
						histogram->swapOnUI(j, j + 1);
						int temp = sortVector[j + 1];
						sortVector[j + 1] = sortVector[j];
						sortVector[j] = temp;
						Sleep(200);
					}
					histogram->setStateOnUI(j, PillarState::Default);
				}
				histogram->setStateOnUI((int)(n - i - 1), PillarState::Completed);
			}
			histogram->setStateOnUI(0, PillarState::Completed);
		});
	auto asyncAction = Windows::System::Threading::ThreadPool::RunAsync(workTask);
}


void AlgorithmVisualization::SortAlgoPage::SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	histogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height);
}


/// <summary>
/// 初始化导航栏项目
/// </summary>
/// <param name="sortAlgorithmType">排序算法类型</param>
void AlgorithmVisualization::SortAlgoPage::InitNavViewItems(int sortAlgorithmType)
{
	SortNavView->MenuItems->Clear();
	vector<NavigateItem> NavigateItems;
	switch (sortAlgorithmType) //选择对应的排序算法项目向量
	{
	case 0:
		NavigateItems = PrimarySortNavItems; break;
	case 1:
		NavigateItems = AdvancedComparativeSortNavItems; break;
	case 2:
		NavigateItems = AdvancedNonComparativeSortNavItems; break;
	default:
		break;
	}
	for (NavigateItem i : NavigateItems)
	{
		auto menuItem = ref new Microsoft::UI::Xaml::Controls::NavigationViewItem();
		menuItem->Content = i.Content; //设置内容
		menuItem->Tag = i.Tag; //设置标签
		SortNavView->MenuItems->Append(menuItem); //追加进入菜单栏
	}
}
