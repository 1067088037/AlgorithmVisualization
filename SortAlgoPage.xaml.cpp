//
// SortAlgoPage.xaml.cpp
// SortAlgoPage 类的实现
//

#include "pch.h"
#include "SortAlgoPage.xaml.h"
#include "Histogram.h"
#include <vector>
#include "ExecuteStep.h"

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
	executor = ref new SortExcute(); //实例化排序可执行
	
	SortHistogram->Children->Append(executor->histogram->container); //向父级容器中添加柱状图的容器
	InitNavViewItems(((App^)(Application::Current))->sortAlgorithmType); //初始化导航栏
}

void AlgorithmVisualization::SortAlgoPage::SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SortNavView->SelectedItem = SortNavView->MenuItems->GetAt(0);
}

void AlgorithmVisualization::SortAlgoPage::SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	auto tag = args->InvokedItemContainer->Tag->ToString();
	InitAlgorithm(tag);
}

void AlgorithmVisualization::SortAlgoPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto workTask = ref new WorkItemHandler([this](IAsyncAction^ workItem)
		{
			auto n = executor->sortVector->Size;
			for (auto i = 0; i < n - 1; i++)
			{
				auto j = 0;
				for (j = 0; j < n - i - 1; j++)
				{
					executor->histogram->compareOnUI(j, j + 1);
					Sleep(200);
					if (executor->sortVector->GetAt(j) > executor->sortVector->GetAt(j+1)) {
						executor->histogram->swapOnUI(j, j + 1);
						int temp = executor->sortVector->GetAt(j + 1);
						executor->sortVector->SetAt(j + 1, executor->sortVector->GetAt(j));
						executor->sortVector->SetAt(j, temp);
						Sleep(200);
					}
					executor->histogram->setStateOnUI(j, PillarState::Default);
				}
				executor->histogram->setStateOnUI((int)(n - i - 1), PillarState::Completed);
			}
			executor->histogram->setStateOnUI(0, PillarState::Completed);
		});
	auto asyncAction = Windows::System::Threading::ThreadPool::RunAsync(workTask);
}

void AlgorithmVisualization::SortAlgoPage::SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	executor->histogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height);
}

void AlgorithmVisualization::SortAlgoPage::InitAlgorithm(String^ tag)
{
	executor->NavigateToFirst();
	
	if (tag == L"BubbleSort") {
		AlgorithmName->Text = L"冒泡排序";
	}
	else if (tag == L"SelectionSort")
	{
		AlgorithmName->Text = L"选择排序";
	}
	else if (tag == L"InsertionSort")
	{
		AlgorithmName->Text = L"插入排序";
	}
	else if (tag == L"ShellSort")
	{
		AlgorithmName->Text = "希尔排序";
	}
	else if (tag == L"MergeSort")
	{

	}
	else if (tag == L"QuickSort")
	{

	}
	else if (tag == L"HeapSort")
	{

	}
	else if (tag == L"CountingSort")
	{

	}
	else if (tag == L"BucketSort")
	{

	}
	else if (tag == L"RadixSort")
	{

	}
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
		NavigateItems = PrimarySortNavItems;
		InitAlgorithm(L"BubbleSort");
		break;
	case 1:
		NavigateItems = AdvancedComparativeSortNavItems;
		InitAlgorithm(L"MergeSort");
		break;
	case 2:
		NavigateItems = AdvancedNonComparativeSortNavItems;
		InitAlgorithm(L"CountingSort");
		break;
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

SingleStep^ AlgorithmVisualization::SortExcute::NavigateToStep(int index)
{
	sortVector->Clear();
	auto vec = ref new Vector<int>();
	int size = 20;
	for (int i = 0; i < size; ++i)
	{
		int value = rand() % 99 + 1;
		sortVector->Append(value);
		vec->Append(value);
	}
	histogram->load(vec);

	return nullptr;
}

SingleStep^ AlgorithmVisualization::SortExcute::NavigateToNext()
{
	throw ref new Platform::NotImplementedException();
	// TODO: 在此处插入 return 语句
}

SingleStep^ AlgorithmVisualization::SortExcute::NavigateToPrevious()
{
	throw ref new Platform::NotImplementedException();
	// TODO: 在此处插入 return 语句
}

AlgorithmVisualization::SortExcute::SortExcute()
{
	histogram = ref new Histogram(); //初始化容器
	sortVector = ref new Vector<int>();
}
