//
// SortAlgoPage.xaml.cpp
// SortAlgoPage 类的实现
//

#include "pch.h"
#include "SortAlgoPage.xaml.h"
#include "Histogram.h"
#include <vector>
#include "ExecuteStep.h"
#include "Util.h"
#include "CodeDrawable.h"

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
using namespace Windows::UI::Xaml::Documents;

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

/// <summary>
/// 排序算法页面构造函数
/// </summary>
SortAlgoPage::SortAlgoPage()
{
	InitializeComponent();
	InitNavViewItems(((App^)(Application::Current))->sortAlgorithmType); //初始化导航栏
}

/// <summary>
/// 导航栏的加载
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	SortNavView->SelectedItem = SortNavView->MenuItems->GetAt(0);
}

/// <summary>
/// 当项目被点击时
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void AlgorithmVisualization::SortAlgoPage::SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	auto tag = args->InvokedItemContainer->Tag->ToString();
	InitAlgorithm(tag);
}

/// <summary>
/// 调试按钮被点击时
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Debug_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//auto para = ref new Paragraph();
	//auto run = ref new Run();
	//run->Text = "1222222";
	//run->Foreground = ref new SolidColorBrush(Colors::CadetBlue);
	//para->Inlines->Append(run);
	//Code->Blocks->Append(para);

	auto code = ref new CodeDrawable();
	code->Texts->Append("111111111111\n");
	code->Texts->Append("222222222222\n");
	code->Texts->Append("3333333333\n");
	code->Texts->Append("444444444\n");
	Code->Blocks->Clear();
	Code->Blocks->Append(code->GenerateDrawable(2));
}

/// <summary>
/// 排序柱状图监测尺寸改变
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	executor->histogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height);
}

/// <summary>
/// 初始化算法
/// </summary>
/// <param name="tag"></param>
void AlgorithmVisualization::SortAlgoPage::InitAlgorithm(String^ tag)
{
	executor = ref new SortExcute(); //实例化排序可执行
	executor->ProcessText = ProcessText;
	executor->ProgressSlider = ProgressSlider;
	executor->SpeedText = SpeedText;
	executor->SpeedSlider = SpeedSlider;

	SpeedSlider->Maximum = executor->SpeedList->Size - 1;
	SpeedSlider->Minimum = 1;

	if (tag == L"BubbleSort") {
		AlgorithmName->Text = L"冒泡排序";
		SpeedSlider->Value = 13;
		auto n = executor->sortVector->Size; //数字总数
		Debug("数字总数 = " + n);
		auto stateList = ref new Vector<int>(n, (int)PillarState::Default);
		AddRecoverStep(stateList, ref new Vector<int>());
		for (auto i = 0; i < n - 1; i++)
		{
			for (auto j = 0; j < n - i - 1; j++)
			{
				//Debug("添加比较");
				AddCompareStep(stateList, j, j + 1);
				//executor->histogram->compareOnUI(j, j + 1); //比较j和j+1大小
				if (executor->sortVector->GetAt(j) > executor->sortVector->GetAt(j + 1)) { //如果后者更大
					//Debug("添加交换");
					//executor->histogram->swapOnUI(j, j + 1); //交换二者
					int temp = executor->sortVector->GetAt(j + 1);
					executor->sortVector->SetAt(j + 1, executor->sortVector->GetAt(j));
					executor->sortVector->SetAt(j, temp);

					AddSwapStep(stateList, j, j + 1);
				}
				stateList->SetAt(j, (int)PillarState::Default);
				//executor->histogram->setStateOnUI(j, PillarState::Default); //让前者恢复默认状态
			}
			AddCompleteStep(stateList, ref new Vector<int>{ (int)n - i - 1 });
			//executor->histogram->setStateOnUI((int)(n - i - 1), PillarState::Completed); //最后一个设置为已完成状态
		}
		AddCompleteStep(stateList, ref new Vector<int>{ 0 });
		//executor->histogram->setStateOnUI(0, PillarState::Completed); //第一个也设为已完成
		Debug("总步骤数 = " + executor->CountStep());

		ProcessText->Text = "0/" + (executor->StepList->Size - 1).ToString();
		ProgressSlider->Maximum = executor->StepList->Size - 1;
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

	SortHistogram->Children->Append(executor->histogram->container); //向父级容器中添加柱状图的容器
	executor->NavigateToFirst();
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
		auto menuItem = ref new Microsoft::UI::Xaml::Controls::NavigationViewItem(); //创建菜单栏目
		menuItem->Content = i.Content; //设置内容
		menuItem->Tag = i.Tag; //设置标签
		SortNavView->MenuItems->Append(menuItem); //追加进入菜单栏
	}
}

void AlgorithmVisualization::SortAlgoPage::Debug(String^ message)
{
	Console->Text += message + "\n";
}

void AlgorithmVisualization::SortAlgoPage::StartThreadTimer()
{
	if (!executor->TimerIsRunning())
	{
		Windows::Foundation::TimeSpan period{};
		period.Duration = executor->Speed * 10000;
		executor->ThreadTimer = ThreadPoolTimer::CreatePeriodicTimer(
			ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
				{
					if (executor->IsLastStep())
					{
						executor->StopTimer();
						return;
					}
					CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]()
						{
							if (executor->IsLastStep())
							{
								executor->StopTimer();
								return;
							}
							executor->NavigateToNext();
						}
					));
				}), period
		);
	}
}

SingleStep^ AlgorithmVisualization::SortExcute::NavigateToStep(int index)
{
	CurrentStep = index;
	ProcessText->Text = CurrentStep.ToString() + "/" + (StepList->Size - 1).ToString();
	ProgressSlider->Value = CurrentStep;
	if (index >= 0 && index < StepList->Size)
	{
		auto stateList = ref new Vector<PillarState>();
		for (int i : StepList->GetAt(index)->ThisState->GetAt(1))
		{
			stateList->Append((PillarState)i);
		}
		histogram->load(StepList->GetAt(index)->ThisState->GetAt(0), stateList);
		return StepList->GetAt(index);
	}
	else
	{
		throw ref new InvalidArgumentException("传入的序号越界");
	}
}

SingleStep^ AlgorithmVisualization::SortExcute::NavigateToNext()
{
	CurrentStep++;
	return NavigateToStep(CurrentStep);
}

SingleStep^ AlgorithmVisualization::SortExcute::NavigateToPrevious()
{
	CurrentStep--;
	return NavigateToStep(CurrentStep);
}

/// <summary>
/// 算法可执行类的构造函数
/// </summary>
AlgorithmVisualization::SortExcute::SortExcute()
{
	DefaultInit();
	histogram = ref new Histogram(); //初始化容器
	sortVector = ref new Vector<int>(); //初始化要排序的向量
	int size = 25;
	for (int i = 0; i < size; ++i)
	{
		int value = rand() % 99 + 1;
		sortVector->Append(value);
	}
}

/// <summary>
/// 上一步
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Previous_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!executor->IsFirstStep()) executor->NavigateToPrevious();
	//Debug("当前步骤:" + executor->CurrentStep);
}

/// <summary>
/// 开始按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	StartThreadTimer();
}

/// <summary>
/// 暂停按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Pause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	executor->StopTimer();
}

/// <summary>
/// 下一步
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Next_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!executor->IsLastStep()) executor->NavigateToNext();
	//Debug("当前步骤:" + executor->CurrentStep);
}

void AlgorithmVisualization::SortAlgoPage::AddCompareStep(IVector<int>^ stateList, int left, int right)
{
	auto compareStep = ref new SingleStep(0); //比较步骤
	compareStep->ThisState->Append(Util::CopyVector(executor->sortVector));
	stateList->SetAt(left, (int)PillarState::Compared);
	stateList->SetAt(right, (int)PillarState::Compared);
	compareStep->ThisState->Append(Util::CopyVector(stateList));
	executor->AddStep(compareStep);
}

void AlgorithmVisualization::SortAlgoPage::AddSwapStep(IVector<int>^ stateList, int left, int right)
{
	auto swapStep = ref new SingleStep(1);
	swapStep->ThisState->Append(Util::CopyVector(executor->sortVector));
	stateList->SetAt(left, (int)PillarState::Swapping);
	stateList->SetAt(right, (int)PillarState::Swapping);
	swapStep->ThisState->Append(Util::CopyVector(stateList));
	executor->AddStep(swapStep);
}

void AlgorithmVisualization::SortAlgoPage::AddRecoverStep(IVector<int>^ stateList, IVector<int>^ recover)
{
	auto recoverStep = ref new SingleStep(2);
	recoverStep->ThisState->Append(Util::CopyVector(executor->sortVector));
	for (auto i : recover)
	{
		stateList->SetAt(i, (int)PillarState::Swapping);
	}
	recoverStep->ThisState->Append(Util::CopyVector(stateList));
	executor->AddStep(recoverStep);
}

void AlgorithmVisualization::SortAlgoPage::AddCompleteStep(IVector<int>^ stateList, IVector<int>^ complete)
{
	auto endStep = ref new SingleStep(3);
	endStep->ThisState->Append(Util::CopyVector(executor->sortVector));
	for (auto i : complete)
	{
		stateList->SetAt(i, (int)PillarState::Completed);
	}
	endStep->ThisState->Append(Util::CopyVector(stateList));
	executor->AddStep(endStep);
}


void AlgorithmVisualization::SortAlgoPage::ProgressSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	executor->NavigateToStep(e->NewValue);
}


void AlgorithmVisualization::SortAlgoPage::SpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	executor->Speed = executor->SpeedList->GetAt(e->NewValue);
	SpeedText->Text = executor->Speed / 1000.0 + "秒/步";
	if (executor->TimerIsRunning())
	{
		executor->StopTimer();
		StartThreadTimer();
	}
}
