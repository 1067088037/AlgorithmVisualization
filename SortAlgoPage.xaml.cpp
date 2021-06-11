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
	SortNavView->SelectedItem = SortNavView->MenuItems->GetAt(0); //默认选择第一个
}

/// <summary>
/// 当项目被点击时
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void AlgorithmVisualization::SortAlgoPage::SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	auto tag = args->InvokedItemContainer->Tag->ToString(); //获取点击Tag
	Executor->SortVector->Clear(); //清空向量
	Executor->StopTimer(); //关闭计时器
	InitAlgorithm(tag); //初始化对应的算法
}

/// <summary>
/// 调试按钮被点击时
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Debug_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}

/// <summary>
/// 排序柱状图监测尺寸改变
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	lastHistogramWidth = e->NewSize.Width; //记录宽度
	lastHistogramHeight = e->NewSize.Height; //记录高度
	if (Executor->NeedAssistHistogram)
	{
		Executor->MainHistogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height / 2); //自适应布局调整大小
		Executor->AssistHistogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height / 2); //自适应布局调整大小
	}
	else
	{
		Executor->MainHistogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height); //自适应布局调整大小
	}
}

/// <summary>
/// 初始化算法
/// </summary>
/// <param name="tag"></param>
void AlgorithmVisualization::SortAlgoPage::InitAlgorithm(String^ tag)
{
	float width = lastHistogramWidth;
	float height = defaultHeight; //不使用动态高度

	Introduction->TextHighlighters->Clear(); //清空高亮

	if (tag == L"BubbleSort") {
		AlgorithmName->Text = L"冒泡排序";
		Executor = ref new SortExcute(20, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitBubbleSort();
	}
	else if (tag == L"SelectionSort")
	{
		AlgorithmName->Text = L"选择排序";
		Executor = ref new SortExcute(20, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitSelectionSort();
	}
	else if (tag == L"InsertionSort")
	{
		AlgorithmName->Text = L"插入排序";
		Executor = ref new SortExcute(20, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitInsertionSort();
	}
	else if (tag == L"ShellSort")
	{
		AlgorithmName->Text = "希尔排序";
		Executor = ref new SortExcute(20, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitShellSort();
	}
	else if (tag == L"MergeSort")
	{
		AlgorithmName->Text = "归并排序";
		Executor = ref new SortExcute(32, width, height, true); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitMergeSort();
	}
	else if (tag == L"QuickSort")
	{
		AlgorithmName->Text = "快速排序";
		Executor = ref new SortExcute(32, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitQuickSort();
	}
	else if (tag == L"HeapSort")
	{
		AlgorithmName->Text = "堆排序";
		Executor = ref new SortExcute(32, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitHeapSort();
	}
	else if (tag == L"CountingSort")
	{
		AlgorithmName->Text = "计数排序";
		Executor = ref new SortExcute(32, width, height, true); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitCountingSort();
	}
	else if (tag == L"BucketSort")
	{
		AlgorithmName->Text = "桶排序";
		Executor = ref new SortExcute(32, width, height, true); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitBucketSort();
	}
	else if (tag == L"RadixSort")
	{
		AlgorithmName->Text = "基数排序";
		Executor = ref new SortExcute(32, width, height, true); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitRadixSort();
	}

	SortHistogram->Children->Clear();
	if (Executor->NeedAssistHistogram)
	{
		auto MainTextBlock = ref new TextBlock();
		MainTextBlock->Text = L"排序柱状图";
		MainTextBlock->FontWeight = Windows::UI::Text::FontWeights::Bold;
		MainTextBlock->Margin = Windows::UI::Xaml::Thickness(6);
		SortHistogram->Children->Append(MainTextBlock); //向父级容器中添加文字

		auto mainHistogramView = Executor->MainHistogram->container;
		SortHistogram->Children->Append(mainHistogramView); //向父级容器中添加柱状图的容器

		auto AssistTextBlock = ref new TextBlock();
		AssistTextBlock->Text = L"辅助柱状图";
		AssistTextBlock->FontWeight = Windows::UI::Text::FontWeights::Bold;
		AssistTextBlock->Margin = Windows::UI::Xaml::Thickness(6);
		SortHistogram->Children->Append(AssistTextBlock); //向父级容器中添加文字

		auto assistHistogramView = Executor->AssistHistogram->container;
		assistHistogramView->Height = height / 2;
		SortHistogram->Children->Append(assistHistogramView); //向父级容器中添加柱状图的容器
	}
	else
	{
		SortHistogram->Children->Append(Executor->MainHistogram->container); //向父级容器中添加柱状图的容器
	}
	Executor->NavigateToFirst();
}

/// <summary>
/// 初始化导航栏项目
/// </summary>
/// <param name="sortAlgorithmType">排序算法类型</param>
void AlgorithmVisualization::SortAlgoPage::InitNavViewItems(int sortAlgorithmType)
{
	AlgorithmType = sortAlgorithmType;
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

/// <summary>
/// 初始化执行器
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitExecutor()
{
	Executor->ProcessText = ProcessText;
	Executor->ProgressSlider = ProgressSlider;
	Executor->SpeedText = SpeedText;
	Executor->SpeedSlider = SpeedSlider;
	Executor->CodeText = CodeText;
	Executor->CodeDrawable = ref new CodeDrawable();

	SpeedSlider->Maximum = Executor->SpeedList->Size - 1; //设置滑块的最大值
	SpeedSlider->Minimum = 1; //设置滑块的最小值
}

/// <summary>
/// 显示调试信息
/// </summary>
/// <param name="message">调试信息</param>
void AlgorithmVisualization::SortAlgoPage::Debug(String^ message)
{
	Console->Text += message + "\n";
}

/// <summary>
/// 开始计时器线程
/// </summary>
void AlgorithmVisualization::SortAlgoPage::StartThreadTimer()
{
	if (!Executor->TimerIsRunning()) //没有开始运行才创建
	{
		Windows::Foundation::TimeSpan period{};
		period.Duration = Executor->Speed * 10000;  //设置时间间隔
		Executor->ThreadTimer = ThreadPoolTimer::CreatePeriodicTimer(
			ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
				{
					if (Executor->IsLastStep())
					{
						//已经到达最后一步则结束计时器
						Executor->StopTimer();
						return;
					}
					CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this]()
						{
							if (Executor->IsLastStep())
							{
								//已经到达最后一步则结束计时器
								Executor->StopTimer();
								return;
							}
							Executor->NavigateToNext(); //如果没有到最后一步则导航到下一步
						}
					));
				}), period
		);
	}
}

/// <summary>
/// 导航到步骤
/// </summary>
/// <param name="index">序号</param>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::SortExcute::NavigateToStep(int index)
{
	CurrentStep = index; //设置当前步骤
	ProcessText->Text = CurrentStep.ToString() + "/" + (StepList->Size - 1).ToString(); //设置文字
	ProgressSlider->Value = CurrentStep;
	if (index >= 0 && index < (int)StepList->Size)
	{
		//ThisState中[0]保存主柱状图数组
		//ThisState中[1]保存主柱状图数组的状态
		//ThisState中[2]保存主柱状图数组的临时标签

		//ThisState中[3]保存辅助柱状图数组
		//ThisState中[4]保存辅助柱状图数组的状态

		auto thisStep = StepList->GetAt(index);
		auto mainStateList = ref new Vector<PillarState>(); //实例化状态列表
		auto assistStateList = ref new Vector<PillarState>(); //实例化状态列表

		for (int i : thisStep->ThisState->GetAt(1))
		{
			mainStateList->Append((PillarState)i);
		}
		if (NeedAssistHistogram)
		{
			for (int i : thisStep->ThisState->GetAt(4))
			{
				assistStateList->Append((PillarState)i);
			}
		}

		int size = thisStep->ThisState->Size;
		if (thisStep->ThisState->Size == 2) //没有临时变量的时候
			MainHistogram->load(thisStep->ThisState->GetAt(0), mainStateList); //加载步骤的内容
		else //有临时变量
			MainHistogram->load(thisStep->ThisState->GetAt(0), mainStateList, thisStep->ThisState->GetAt(2)); //加载步骤的内容

		if (NeedAssistHistogram)
			AssistHistogram->load(thisStep->ThisState->GetAt(3), assistStateList); //加载步骤的内容

		int i = thisStep->ThisState->Size;
		ShowCodeChange(StepList->GetAt(index)->HighlightLines); //显示代码的变化
		return StepList->GetAt(index);
	}
	else
	{
		throw ref new InvalidArgumentException("传入的序号越界"); //抛出数组越界
	}
}

/// <summary>
/// 导航到下一步
/// </summary>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::SortExcute::NavigateToNext()
{
	CurrentStep++;
	return NavigateToStep(CurrentStep);
}

/// <summary>
/// 导航到上一步
/// </summary>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::SortExcute::NavigateToPrevious()
{
	CurrentStep--;
	return NavigateToStep(CurrentStep);
}

/// <summary>
/// 构造函数
/// </summary>
/// <param name="n">要排序的数字个数</param>
AlgorithmVisualization::SortExcute::SortExcute(int n, float width, float height, bool needAssist)
{
	NeedAssistHistogram = needAssist;
	DefaultInit(); //默认的初始化
	if (needAssist)
	{
		MainHistogram = ref new Histogram(width, height / 2); //初始化容器
		AssistHistogram = ref new Histogram(width, height / 2); //初始化容器
	}
	else
	{
		MainHistogram = ref new Histogram(width, height); //初始化容器
	}
	SortVector = ref new Vector<int>(); //初始化要排序的向量
	for (int i = 0; i < n; ++i)
	{
		int value = rand() % 99; //生成随机数用于排序
		SortVector->Append(value);
	}
	SortStates = ref new Vector<int>(n, (int)PillarState::Default);
	if (needAssist)
	{
		AssistVector = ref new Vector<int>(n, 0);
		AssistStates = ref new Vector<int>(n, (int)PillarState::Default);
	}
}

/// <summary>
/// 展示代码的修改
/// </summary>
/// <param name="highlighters"></param>
void AlgorithmVisualization::SortExcute::ShowCodeChange(IVector<int>^ highlighters)
{
	auto generateRes = CodeDrawable->GenerateDrawable(highlighters); //生成可绘制的代码
	CodeText->Blocks->Clear();
	CodeText->Blocks->Append((Paragraph^)(generateRes->GetAt(0))); //加载段落
	CodeText->TextHighlighters->Clear();
	CodeText->TextHighlighters->Append((TextHighlighter^)(generateRes->GetAt(1))); //加载高亮
}

/// <summary>
/// 上一步
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Previous_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!Executor->IsFirstStep()) Executor->NavigateToPrevious();
	//Debug("当前步骤:" + executor->CurrentStep);
}

/// <summary>
/// 开始按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	StartThreadTimer(); //启动计时器线程
}

/// <summary>
/// 暂停按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Pause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	Executor->StopTimer(); //结束计时器
}

/// <summary>
/// 下一步
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::Next_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!Executor->IsLastStep()) Executor->NavigateToNext();
	//Debug("当前步骤:" + executor->CurrentStep);
}

/// <summary>
/// 初始化辅助步骤
/// </summary>
/// <param name="step"></param>
void AlgorithmVisualization::SortAlgoPage::InitAssistStep(SingleStep^ step)
{
	if (Executor->NeedAssistHistogram)
	{
		step->ThisState->Append(Util::CopyVector(Executor->AssistVector));
		step->ThisState->Append(Util::CopyVector(Executor->AssistStates));
		step->ThisState->Append(Util::CopyVector(Executor->AssistVector));
	}
}

/// <summary>
/// 交换向量中数字
/// </summary>
/// <param name="arr"></param>
/// <param name="left"></param>
/// <param name="right"></param>
void AlgorithmVisualization::SortAlgoPage::SwapInVector(IVector<int>^ arr, int left, int right)
{
	int temp = arr->GetAt(left);
	arr->SetAt(left, arr->GetAt(right));
	arr->SetAt(right, temp);
}

/// <summary>
/// 添加空步骤
/// </summary>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddEmptyStep(IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(0); //比较步骤
	tempStep->HighlightLines->Append(-1);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加设置步骤
/// </summary>
/// <param name="pos"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddSetStep(int pos, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(0); //设置步骤
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	Executor->SortStates->SetAt(pos, (int)PillarState::SetValue); //设置去向
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加有来源的设置步骤
/// </summary>
/// <param name="from"></param>
/// <param name="to"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddSetFromToStep(int from, int to, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(0); //设置步骤
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	Executor->SortStates->SetAt(from, (int)PillarState::Selected); //设置来源
	Executor->SortStates->SetAt(to, (int)PillarState::SetValue); //设置去向
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 新建跨柱状图的有来源步骤
/// </summary>
/// <param name="fromHis"></param>
/// <param name="fromPos"></param>
/// <param name="toHis"></param>
/// <param name="toPos"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddSetFromToStep(int fromHis, int fromPos, int toHis, int toPos, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(0); //设置步骤
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//获取合适的状态向量
	IVector<int>^ leftHisStates = fromHis == 0 ? Executor->SortStates : Executor->AssistStates;
	IVector<int>^ rightHisStates = toHis == 0 ? Executor->SortStates : Executor->AssistStates;
	//设置来源和去向
	leftHisStates->SetAt(fromPos, (int)PillarState::Selected); //设置来源
	rightHisStates->SetAt(toPos, (int)PillarState::SetValue); //设置去向
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加比较步骤
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddCompareStep(int left, int right, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(0); //比较步骤
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//设置两个位置为比较状态
	Executor->SortStates->SetAt(left, (int)PillarState::Compared);
	Executor->SortStates->SetAt(right, (int)PillarState::Compared);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加跨柱状图的比较
/// </summary>
/// <param name="leftHisIndex"></param>
/// <param name="leftPos"></param>
/// <param name="rightHisIndex"></param>
/// <param name="rightPos"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddCompareStep(int leftHisIndex, int leftPos, int rightHisIndex, int rightPos, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(0); //比较步骤
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//获取合适的状态向量
	IVector<int>^ leftHisStates = leftHisIndex == 0 ? Executor->SortStates : Executor->AssistStates;
	IVector<int>^ rightHisStates = rightHisIndex == 0 ? Executor->SortStates : Executor->AssistStates;
	//设置两个位置为比较状态
	leftHisStates->SetAt(leftPos, (int)PillarState::Compared);
	rightHisStates->SetAt(rightPos, (int)PillarState::Compared);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加交换步骤
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddSwapStep(int left, int right, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(1);
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//设置两个位置为交换状态
	Executor->SortStates->SetAt(left, (int)PillarState::Swapping);
	Executor->SortStates->SetAt(right, (int)PillarState::Swapping);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加跨柱状图的交换
/// </summary>
/// <param name="leftHisIndex"></param>
/// <param name="leftPos"></param>
/// <param name="rightHisIndex"></param>
/// <param name="rightPos"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddSwapStep(int leftHisIndex, int leftPos, int rightHisIndex, int rightPos, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(1);
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//获取合适的状态向量
	IVector<int>^ leftHisStates = leftHisIndex == 0 ? Executor->SortStates : Executor->AssistStates;
	IVector<int>^ rightHisStates = rightHisIndex == 0 ? Executor->SortStates : Executor->AssistStates;
	//设置两个位置为交换状态
	leftHisStates->SetAt(leftPos, (int)PillarState::Swapping);
	rightHisStates->SetAt(rightPos, (int)PillarState::Swapping);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

bool AlgorithmVisualization::SortAlgoPage::SetToDefault(int his0, int index0, int his1, int index1)
{
	//获取合适的状态向量
	IVector<int>^ leftHisStates = his0 == 0 ? Executor->SortStates : Executor->AssistStates;
	IVector<int>^ rightHisStates = his1 == 0 ? Executor->SortStates : Executor->AssistStates;
	leftHisStates->SetAt(index0, (int)PillarState::Default);
	rightHisStates->SetAt(index1, (int)PillarState::Default);
	return true;
}

/// <summary>
/// 添加恢复步骤
/// </summary>
/// <param name="recover"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddRecoverStep(IVector<int>^ recover, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(2);
	tempStep->HighlightLines->Append(-1);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (auto i : recover)
	{
		Executor->SortStates->SetAt(i, (int)PillarState::Default); //遍历设为默认状态
	}
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加完成步骤
/// </summary>
/// <param name="complete"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddCompleteStep(IVector<int>^ complete, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(3);
	tempStep->HighlightLines->Append(-1);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (auto i : complete)
	{
		Executor->SortStates->SetAt(i, (int)PillarState::Completed); //遍历设为完成状态
	}
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加选中步骤
/// </summary>
/// <param name="select"></param>
/// <param name="highlightLine"></param>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddSelectStep(IVector<int>^ select, int highlightLine, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(4);
	tempStep->HighlightLines->Append(highlightLine);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (auto i : select)
	{
		Executor->SortStates->SetAt(i, (int)PillarState::Selected); //遍历设为完成状态
	}
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 添加全部完成步骤
/// </summary>
/// <param name="isTemp"></param>
/// <param name="histogramIndex"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddAllCompleteStep(IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(3);
	tempStep->HighlightLines->Append(-1);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (unsigned int i = 0; i < Executor->SortStates->Size; ++i)
	{
		Executor->SortStates->SetAt(i, (int)PillarState::Completed); //遍历设为完成状态
	}
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 设置跨柱状图的全部完成
/// </summary>
/// <param name="hisIndex"></param>
/// <param name="isTemp"></param>
/// <returns></returns>
bool AlgorithmVisualization::SortAlgoPage::AddAllCompleteStep(int hisIndex, IVector<int>^ isTemp)
{
	auto tempStep = ref new SingleStep(3);
	tempStep->HighlightLines->Append(-1);
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	IVector<int>^ hisStates = hisIndex == 0 ? Executor->SortStates : Executor->AssistStates;
	for (unsigned int i = 0; i < Executor->SortStates->Size; ++i)
	{
		hisStates->SetAt(i, (int)PillarState::Completed); //遍历设为完成状态
	}
	tempStep->ThisState->Append(Util::CopyVector(Executor->SortStates));
	tempStep->ThisState->Append(isTemp);
	InitAssistStep(tempStep);
	Executor->AddStep(tempStep);
	return true;
}

/// <summary>
/// 初始化冒泡排序算法
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitBubbleSort()
{
	Introduction->Text = L"时间复杂度：O(n²)\n它重复地走访过要排序的元素列，依次比较两个相邻的元素，如果顺序（如从大到小、首字母从Z到A）错误就把他们交换过来。走访元素的工作是重复地进行直到没有相邻元素需要交换，也就是说该元素列已经排序完成。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("for (int i = 0; i < n - 1; i++)\n" +
		"{\n" +
		"    for (int j = 0; j < n - i - 1; j++)\n" +
		"    {\n");
	codeDrawable->Texts->Append("        if (arr[j] > arr[j + 1])    \n");
	codeDrawable->Texts->Append("            std::swap(arr[j], arr[j + 1]);    \n");
	codeDrawable->Texts->Append("    }\n" +
		"}");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto stateList = Executor->SortStates; //获取状态列表
	AddRecoverStep(ref new Vector<int>()); //一开始加入一个空的步骤

	for (auto i = 0; i < n - 1; i++)
	{
		for (auto j = 0; j < n - i - 1; j++)
		{
			AddCompareStep(j, j + 1, 1); //加入比较步骤
			if (Executor->SortVector->GetAt(j) > Executor->SortVector->GetAt(j + 1)) { //如果后者更大
				//交换j和j+1位置上的数字
				int temp = Executor->SortVector->GetAt(j + 1);
				Executor->SortVector->SetAt(j + 1, Executor->SortVector->GetAt(j));
				Executor->SortVector->SetAt(j, temp);
				//加入交换步骤
				AddSwapStep(j, j + 1, 2);
			}
			stateList->SetAt(j, (int)PillarState::Default); //设置前者为默认状态
		}
		AddCompleteStep(ref new Vector<int>{ (int)n - i - 1 }); //设置本次的末尾完成
	}
	AddCompleteStep(ref new Vector<int>{ 0 }); //加入完成步骤
	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化选择排序算法
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitSelectionSort()
{
	Introduction->Text = L"时间复杂度：O(n²)\n它的工作原理是：第一次从待排序的数据元素中选出最小（或最大）的一个元素，存放在序列的起始位置，然后再从剩余的未排序元素中寻找到最小（大）元素，然后放到已排序的序列的末尾。以此类推，直到全部待排序的数据元素的个数为零。选择排序是不稳定的排序方法。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("for (int i = 0; i < n - 1; ++i) {\n    int min = i;\n    for (int j = i + 1; j < n; ++j) {\n");
	codeDrawable->Texts->Append("        if (arr[j] < arr[min]) min = j;    \n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("    swap(arr[i], arr[min]);    \n");
	codeDrawable->Texts->Append("}");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto stateList = Executor->SortStates; //获取状态列表
	AddRecoverStep(ref new Vector<int>()); //一开始加入一个空的步骤

	for (int i = 0; i < n - 1; ++i) {
		int min = i; //初始化最小的下标
		for (int j = i + 1; j < n; ++j) {
			AddCompareStep(min, j, 1); //加入比较步骤
			if (Executor->SortVector->GetAt(j) < Executor->SortVector->GetAt(min)) { //如果新的更小
				stateList->SetAt(min, (int)PillarState::Default); //取消之前的最小值
				min = j;
			}
			stateList->SetAt(j, (int)PillarState::Default); //设置前者为默认状态
		}
		//交换i和min位置上的数字
		int temp = Executor->SortVector->GetAt(i);
		Executor->SortVector->SetAt(i, Executor->SortVector->GetAt(min));
		Executor->SortVector->SetAt(min, temp);
		//加入交换步骤
		AddSwapStep(i, min, 3);
		AddRecoverStep(ref new Vector<int>{ i, min });
		AddCompleteStep(ref new Vector<int>{ i }); //加入完成步骤
	}

	AddCompleteStep(ref new Vector<int>{ n - 1 }); //加入完成步骤
	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化插入排序算法
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitInsertionSort()
{
	Introduction->Text = L"时间复杂度：O(n²)\n它的基本思想是将一个记录插入到已经排好序的有序表中，从而一个新的、记录数增1的有序表。在其实现过程使用双层循环，外层循环对除了第一个元素之外的所有元素，内层循环对当前元素前面有序表进行待插入位置查找，并进行移动。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("for (int i = 0; i < n; i++) {\n");
	codeDrawable->Texts->Append("    for (int j = i; j > 0; j--) {\n");
	codeDrawable->Texts->Append("        if (arr[j] < arr[j - 1])    \n");
	codeDrawable->Texts->Append("            std::swap(arr[j], arr[j - 1]);    \n");
	codeDrawable->Texts->Append("        else break;\n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("}\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto stateList = Executor->SortStates; //获取状态列表
	AddRecoverStep(ref new Vector<int>()); //一开始加入一个空的步骤

	for (int i = 0; i < n; i++) {
		for (int j = i; j > 0; j--) {
			AddCompareStep(j - 1, j, 2); //加入比较步骤
			//恢复默认状态
			stateList->SetAt(j, (int)PillarState::Default);
			stateList->SetAt(j - 1, (int)PillarState::Default);
			if (Executor->SortVector->GetAt(j) < Executor->SortVector->GetAt(j - 1)) //比较
			{
				AddSwapStep(j - 1, j, 3); //添加交换步骤
				//交换数据
				int temp = Executor->SortVector->GetAt(j);
				Executor->SortVector->SetAt(j, Executor->SortVector->GetAt(j - 1));
				Executor->SortVector->SetAt(j - 1, temp);
			}
			else break;
			//恢复默认状态
			stateList->SetAt(j - 1, (int)PillarState::Default);
			stateList->SetAt(j, (int)PillarState::Default);
		}
	}

	AddAllCompleteStep(); //最后全部完成
	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化希尔排序算法
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitShellSort()
{
	Introduction->Text = L"时间复杂度：O(n^(1.3~2))\n希尔排序是直接插入排序算法的一种更高效的改进版本，是非稳定排序算法。希尔排序是把记录按下标的一定增量分组，对每组使用直接插入排序算法排序；随着增量逐渐减少，每组包含的关键词越来越多，当增量减至 1 时，整个文件恰被分成一组，算法便终止。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("for (int gap = n / 2; gap > 0; gap /= 2) {\n");
	codeDrawable->Texts->Append("    for (int i = gap; i < n; ++i) {\n");
	codeDrawable->Texts->Append("        int temp = arr[i];    \n");
	codeDrawable->Texts->Append("        int j;\n");
	codeDrawable->Texts->Append("        for (j = i; j >= gap; j -= gap) {\n");
	codeDrawable->Texts->Append("            if (temp < arr[j - gap])    \n");
	codeDrawable->Texts->Append("                arr[j] = arr[j - gap];    \n");
	codeDrawable->Texts->Append("            else break;    \n");
	codeDrawable->Texts->Append("        }\n");
	codeDrawable->Texts->Append("        arr[j] = temp;    \n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("}\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto stateList = Executor->SortStates; //获取状态列表
	AddRecoverStep(ref new Vector<int>()); //一开始加入一个空的步骤

	Executor->SortVector->Append(0); //临时变量
	stateList->Append((int)PillarState::Default); //临时变量
	auto isTemp = ref new Vector<int>{ n };
	AddEmptyStep(isTemp);

	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; ++i) {
			Executor->SortVector->SetAt(n, Executor->SortVector->GetAt(i));
			AddSetFromToStep(i, n, 2, isTemp);
			stateList->SetAt(i, (int)PillarState::Default);
			int j;
			for (j = i; j >= gap; j -= gap) {
				AddCompareStep(j - gap, n, 5, isTemp);
				stateList->SetAt(n, (int)PillarState::Default);
				stateList->SetAt(j - gap, (int)PillarState::Default);
				if (Executor->SortVector->GetAt(n) < Executor->SortVector->GetAt(j - gap))
				{
					Executor->SortVector->SetAt(j, Executor->SortVector->GetAt(j - gap));
					stateList->SetAt(n, (int)PillarState::Default);
					AddSetFromToStep(j - gap, j, 6, isTemp);
					stateList->SetAt(j, (int)PillarState::Default);
				}
				else break;
			}
			Executor->SortVector->SetAt(j, Executor->SortVector->GetAt(n));
			AddSetFromToStep(n, j, 9, isTemp);
			stateList->SetAt(j, (int)PillarState::Default);
		}
	}

	for (auto& i : stateList)
	{
		i = (int)PillarState::Completed;
	}
	stateList->SetAt(n, (int)PillarState::Default); //临时变量
	AddEmptyStep(isTemp);

	//移除临时变量
	Executor->SortVector->RemoveAtEnd();
	stateList->RemoveAtEnd();
	AddEmptyStep(isTemp);

	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化归并排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitMergeSort()
{
	Introduction->Text = L"时间复杂度：O(nlog₂n)\n归并排序是建立在归并操作上的一种有效，稳定的排序算法，该算法是采用分治法的一个非常典型的应用。将已有序的子序列合并，得到完全有序的序列；即先使每个子序列有序，再使子序列段间有序。若将两个有序表合并成一个有序表，称为二路归并。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("void merge(int *data, int start, int end, int *result) {\n    int left_length = (end - start + 1) / 2 + 1;\n    int left_index = start;\n    int right_index = start + left_length;\n    int result_index = start;\n    while (left_index < start + left_length && right_index < end + 1) {\n");
	codeDrawable->Texts->Append("        if (data[left_index] <= data[right_index])    \n");
	codeDrawable->Texts->Append("            result[result_index++] = data[left_index++];    \n");
	codeDrawable->Texts->Append("        else\n");
	codeDrawable->Texts->Append("            result[result_index++] = data[right_index++];    \n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("    while (left_index < start + left_length)\n        result[result_index++] = data[left_index++];    \n");
	codeDrawable->Texts->Append("    while (right_index < end + 1)\n        result[result_index++] = data[right_index++];    \n");
	codeDrawable->Texts->Append("}\n\n");
	codeDrawable->Texts->Append("void merge_sort(int *data, int start, int end, int *result) {\n    if (1 == end - start) {\n");
	codeDrawable->Texts->Append("        if (data[start] > data[end])    \n");
	codeDrawable->Texts->Append("            std::swap(data[start], data[end]);    \n");
	codeDrawable->Texts->Append("        return;\n");
	codeDrawable->Texts->Append("    } else if (0 == end - start) return;\n");
	codeDrawable->Texts->Append("    else {\n");
	codeDrawable->Texts->Append("        merge_sort(data, start, (end - start + 1) / 2 + start, result);\n");
	codeDrawable->Texts->Append("        merge_sort(data, (end - start + 1) / 2 + start + 1, end, result);\n");
	codeDrawable->Texts->Append("        merge(data, start, end, result);\n");
	codeDrawable->Texts->Append("        for (int i = start; i <= end; ++i)\n            data[i] = result[i];    \n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("}\n\n");
	codeDrawable->Texts->Append("int main() {\n    merge_sort(arr, 0, n - 1, res);\n}\n");
	codeDrawable->Texts->Append("\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector; //主向量
	auto mainState = Executor->SortStates; //获取状态列表
	auto assistVector = Executor->AssistVector; //辅助向量
	auto assistState = Executor->AssistStates; //辅助状态
	auto emptyVector = ref new Vector<int>(); //空向量
	AddRecoverStep(emptyVector); //一开始加入一个空的步骤

	MergeSort(mainVector, 0, n - 1, assistVector); //调用归并排序

	AddAllCompleteStep(0);
	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 分割
/// </summary>
/// <param name="data"></param>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="result"></param>
void AlgorithmVisualization::SortAlgoPage::Merge(IVector<int>^ data, int start, int end, IVector<int>^ result)
{
	auto n = (int)Executor->SortVector->Size; //数字总数

	int left_length = (end - start + 1) / 2 + 1;//左部分区间的数据元素的个数
	int left_index = start;
	int right_index = start + left_length;
	int result_index = start;
	bool lastCycle = end - start + 1 == n;
	while (left_index < start + left_length && right_index < end + 1) {
		AddCompareStep(0, left_index, 0, right_index, 1);
		SetToDefault(0, left_index, 0, right_index);
		if (data->GetAt(left_index) <= data->GetAt(right_index))
		{
			result->SetAt(result_index, data->GetAt(left_index));
			AddSetFromToStep(0, left_index, 1, result_index, 2);
			SetToDefault(1, result_index, 0, left_index);
			result_index++;
			left_index++;
		}
		else
		{
			result->SetAt(result_index, data->GetAt(right_index));
			AddSetFromToStep(0, right_index, 1, result_index, 4);
			SetToDefault(1, result_index, 0, right_index);
			result_index++;
			right_index++;
		}
		if (lastCycle)
		{
			for (int i = 0; i < result_index; ++i)
			{
				Executor->AssistStates->SetAt(i, (int)PillarState::Completed);
			}
		}
	}
	while (left_index < start + left_length)
	{
		result->SetAt(result_index, data->GetAt(left_index));
		AddSetFromToStep(0, left_index, 1, result_index, 6);
		SetToDefault(1, result_index, 0, left_index);
		result_index++;
		left_index++;
	}
	while (right_index < end + 1)
	{
		result->SetAt(result_index, data->GetAt(right_index));
		AddSetFromToStep(0, right_index, 1, result_index, 7);
		SetToDefault(1, result_index, 0, right_index);
		result_index++;
		right_index++;
	}
}

/// <summary>
/// 归并排序
/// </summary>
/// <param name="data"></param>
/// <param name="start"></param>
/// <param name="end"></param>
/// <param name="result"></param>
void AlgorithmVisualization::SortAlgoPage::MergeSort(IVector<int>^ data, int start, int end, IVector<int>^ result)
{
	auto n = (int)Executor->SortVector->Size; //数字总数

	if (end - start == 1) {
		AddCompareStep(0, start, 0, end, 10);
		SetToDefault(0, start, 0, end);
		if (data->GetAt(start) > data->GetAt(end)) {
			int temp = data->GetAt(start);
			data->SetAt(start, data->GetAt(end));
			data->SetAt(end, temp);
			AddSwapStep(0, start, 0, end, 11);
			SetToDefault(0, start, 0, end);
		}
		return;
	}
	else if (0 == end - start)
		return;
	else {
		MergeSort(data, start, (end - start + 1) / 2 + start, result);
		MergeSort(data, (end - start + 1) / 2 + start + 1, end, result);
		Merge(data, start, end, result);
		bool lastCycle = end - start + 1 == n;
		if (lastCycle)
		{
			for (int i = 0; i < n; ++i)
			{
				Executor->AssistStates->SetAt(i, (int)PillarState::Completed);
			}
		}
		for (int i = start; i <= end; ++i)
		{
			data->SetAt(i, result->GetAt(i));
			AddSetFromToStep(1, i, 0, i, 18);
			SetToDefault(0, i, 1, i);
			if (lastCycle)
			{
				Executor->SortStates->SetAt(i, (int)PillarState::Completed);
				Executor->AssistStates->SetAt(i, (int)PillarState::Completed);
			}
		}
	}
}

/// <summary>
/// 初始化快速排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitQuickSort()
{
	Introduction->Text = L"时间复杂度：O(nlog₂n)\n通过一趟排序将要排序的数据分割成独立的两部分，其中一部分的所有数据都比另外一部分的所有数据都要小，然后再按此方法对这两部分数据分别进行快速排序，整个排序过程可以递归进行，以此达到整个数据变成有序序列。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("int partition(int arr[], int left, int right) {\n");
	codeDrawable->Texts->Append("    int i = left + 1;\n    int j = right;\n    int temp = arr[left];    \n");
	codeDrawable->Texts->Append("    while (i <= j) {\n");
	codeDrawable->Texts->Append("        while (arr[i] < temp) i++;    \n");
	codeDrawable->Texts->Append("        while (arr[j] > temp) j--;    \n");
	codeDrawable->Texts->Append("        if (i < j) {\n");
	codeDrawable->Texts->Append("            swap(arr[i], arr[j]);    \n            i++;    \n            j--;    \n");
	codeDrawable->Texts->Append("        } else i++;\n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("    swap(arr[j], arr[left]);    \n");
	codeDrawable->Texts->Append("    return j;\n");
	codeDrawable->Texts->Append("}\n\n");
	codeDrawable->Texts->Append("void quick_sort(int arr[], int left, int right) {\n");
	codeDrawable->Texts->Append("    if (left > right) return;\n");
	codeDrawable->Texts->Append("    int j = partition(arr, left, right);\n");
	codeDrawable->Texts->Append("    quick_sort(arr, left, j - 1);\n");
	codeDrawable->Texts->Append("    quick_sort(arr, j + 1, right);\n");
	codeDrawable->Texts->Append("}\n\n");
	codeDrawable->Texts->Append("int main() {\n");
	codeDrawable->Texts->Append("    quick_sort(arr, 0, n - 1);\n");
	codeDrawable->Texts->Append("}\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector; //主向量
	auto mainState = Executor->SortStates; //获取状态列表
	auto emptyVector = ref new Vector<int>(); //空向量
	AddRecoverStep(emptyVector); //一开始加入一个空的步骤

	Executor->SortVector->Append(0); //临时变量
	mainState->Append((int)PillarState::Default); //临时变量
	auto isTemp = ref new Vector<int>{ n };
	AddEmptyStep(isTemp);

	QuickSort(mainVector, 0, n - 1);

	for (auto& i : mainState)
	{
		i = (int)PillarState::Completed;
	}
	mainState->SetAt(n, (int)PillarState::Default); //临时变量
	AddEmptyStep(isTemp);

	//移除临时变量
	Executor->SortVector->RemoveAtEnd();
	mainState->RemoveAtEnd();
	AddEmptyStep(isTemp);

	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 分治法
/// </summary>
/// <param name="arr"></param>
/// <param name="left"></param>
/// <param name="right"></param>
/// <returns></returns>
int AlgorithmVisualization::SortAlgoPage::Partition(IVector<int>^ arr, int left, int right)
{
	auto n = (int)Executor->SortVector->Size - 1; //数字总数
	auto mainState = Executor->SortStates; //获取状态列表
	auto isTemp = ref new Vector<int>{ n };

	int i = left + 1;
	int j = right;
	int temp = arr->GetAt(left);
	arr->SetAt(n, temp);
	AddSetFromToStep(left, n, 1, isTemp);
	SetToDefault(0, left, 0, n);
	while (i <= j) {
		while (true)
		{
			AddCompareStep(i, n, 3, isTemp);
			SetToDefault(0, i, 0, n);
			if (arr->GetAt(i) >= temp) break;
			i++;
			mainState->SetAt(i, (int)PillarState::Selected);
		}
		while (true)
		{
			AddCompareStep(j, n, 4, isTemp);
			SetToDefault(0, j, 0, n);
			if (arr->GetAt(j) <= temp) break;
			j--;
			mainState->SetAt(j, (int)PillarState::Selected);
		}
		if (i < j)
		{
			int tmp1 = arr->GetAt(i);
			arr->SetAt(i, arr->GetAt(j));
			arr->SetAt(j, tmp1);
			AddSwapStep(i, j, 6, isTemp);
			SetToDefault(0, i, 0, j);
			i++;
			j--;
		}
		else i++;
	}
	int tmp2 = arr->GetAt(left);
	arr->SetAt(left, arr->GetAt(j));
	arr->SetAt(j, tmp2);
	AddSwapStep(left, j, 9, isTemp);
	SetToDefault(0, left, 0, j);
	return j;
}

/// <summary>
/// 快速排序
/// </summary>
/// <param name="arr"></param>
/// <param name="left"></param>
/// <param name="right"></param>
void AlgorithmVisualization::SortAlgoPage::QuickSort(IVector<int>^ arr, int left, int right)
{
	if (left > right) return;
	int j = Partition(arr, left, right);
	QuickSort(arr, left, j - 1);
	for (int i = 0; i < j; i++)
	{
		Executor->SortStates->SetAt(i, (int)PillarState::Completed);
	}
	QuickSort(arr, j + 1, right);
}

/// <summary>
/// 初始化堆排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitHeapSort()
{
	Introduction->Text = L"时间复杂度：O(nlog₂n)\n堆排序是指利用堆这种数据结构所设计的一种排序算法。堆积是一个近似完全二叉树的结构，并同时满足堆积的性质：即子结点的键值或索引总是小于（或者大于）它的父节点。堆排序可以说是一种利用堆的概念来排序的选择排序。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("void max_heapify(int pInt[], int start, int end) {\n    int dad = start;\n    int son = dad * 2 + 1;\n");
	codeDrawable->Texts->Append("    while (son <= end) {\n");
	codeDrawable->Texts->Append("        if (son + 1 <= end && pInt[son] < pInt[son + 1])    \n");
	codeDrawable->Texts->Append("            son++;\n");
	codeDrawable->Texts->Append("        if (pInt[dad] > pInt[son]) return;\n");
	codeDrawable->Texts->Append("        else {\n");
	codeDrawable->Texts->Append("            swap(pInt[dad], pInt[son]);    \n            dad = son;    \n            son = dad * 2 + 1;    \n");
	codeDrawable->Texts->Append("        }\n    }\n}\n\n");
	codeDrawable->Texts->Append("void heap_sort(int pInt[], int len) {\n");
	codeDrawable->Texts->Append("    for (int i = len / 2 - 1; i >= 0; i--)\n");
	codeDrawable->Texts->Append("        max_heapify(pInt, i, len - 1);\n");
	codeDrawable->Texts->Append("    for (int i = len - 1; i > 0; i--) {\n");
	codeDrawable->Texts->Append("        swap(pInt[0], pInt[i]);    \n");
	codeDrawable->Texts->Append("        max_heapify(pInt, 0, i - 1);\n");
	codeDrawable->Texts->Append("    }\n}\n\n");
	codeDrawable->Texts->Append("int main() {\n");
	codeDrawable->Texts->Append("    heap_sort(arr, n);\n");
	codeDrawable->Texts->Append("}\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector; //主向量
	auto mainState = Executor->SortStates; //获取状态列表
	auto emptyVector = ref new Vector<int>(); //空向量
	AddRecoverStep(emptyVector); //一开始加入一个空的步骤

	HeapSort(mainVector);

	AddAllCompleteStep();

	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 最大堆化
/// </summary>
/// <param name="arr"></param>
/// <param name="start"></param>
/// <param name="end"></param>
void AlgorithmVisualization::SortAlgoPage::MaxHeapify(IVector<int>^ arr, int start, int end)
{
	int dad = start;
	int son = dad * 2 + 1;
	while (son <= end) {

		if (son + 1 <= end)
		{
			AddCompareStep(son, son + 1, 2);
			SetToDefault(0, son, 0, son + 1);
			if (arr->GetAt(son) < arr->GetAt(son + 1))
				son++;
		}
		AddCompareStep(dad, son, 4);
		SetToDefault(0, dad, 0, son);
		if (arr->GetAt(dad) > arr->GetAt(son))
			return;
		else {
			SwapInVector(arr, dad, son);
			AddSwapStep(dad, son, 6);
			SetToDefault(0, dad, 0, son);
			dad = son;
			son = dad * 2 + 1;
		}
	}
}

/// <summary>
/// 堆排序
/// </summary>
/// <param name="arr"></param>
void AlgorithmVisualization::SortAlgoPage::HeapSort(IVector<int>^ arr)
{
	auto n = (int)Executor->SortVector->Size; //数字总数
	for (int i = n / 2 - 1; i >= 0; i--)
		MaxHeapify(arr, i, n - 1);
	for (int i = n - 1; i > 0; i--) {
		SwapInVector(arr, 0, i);
		AddSwapStep(0, i, 12);
		SetToDefault(0, 0, 0, i);
		MaxHeapify(arr, 0, i - 1);
		Executor->SortStates->SetAt(i, (int)PillarState::Completed);
	}
}

/// <summary>
/// 初始化计数排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitCountingSort()
{
	Introduction->Text = L"时间复杂度：O(n)\n计数排序是一个非基于比较的排序算法，该算法于1954年由 Harold H. Seward 提出。它的优势在于在对一定范围内的整数排序时，它的复杂度为Ο(n+k)（其中k是整数的范围），快于任何比较排序算法。";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("int max = arr[0];\n");
	codeDrawable->Texts->Append("for (int i = 1; i < n; ++i) {\n");
	codeDrawable->Texts->Append("    if (arr[i] > max)\n");
	codeDrawable->Texts->Append("        max = arr[i];\n");
	codeDrawable->Texts->Append("}\n");
	codeDrawable->Texts->Append("int *countData = new int[max + 1];\n");
	codeDrawable->Texts->Append("for (int i = 0; i <= max; ++i)\n        countData[i] = 0;\n");
	codeDrawable->Texts->Append("for (int i : arr)    \n        ++countData[i];    \n");
	codeDrawable->Texts->Append("int index = 0;\n");
	codeDrawable->Texts->Append("for (int i = 0; i <= max; ++i) {\n");
	codeDrawable->Texts->Append("    for (int j = 0; j < countData[i]; ++j) {    \n");
	codeDrawable->Texts->Append("        arr[index++] = i;    \n");
	codeDrawable->Texts->Append("    }\n");
	codeDrawable->Texts->Append("}\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector; //主向量
	for (unsigned int i = 0; i < mainVector->Size; i++)
	{
		mainVector->SetAt(i, mainVector->GetAt(i) % 41);
	}
	auto mainState = Executor->SortStates; //获取状态列表
	auto emptyVector = ref new Vector<int>(); //空向量
	Executor->AssistVector->Clear();
	AddRecoverStep(emptyVector); //一开始加入一个空的步骤

	Executor->SortVector->Append(0); //临时变量
	mainState->Append((int)PillarState::Default); //临时变量
	auto isTemp = ref new Vector<int>{ n };
	AddEmptyStep(isTemp);

	mainVector->SetAt(n, mainVector->GetAt(0));
	AddSetFromToStep(0, 0, 0, n, 0, isTemp);
	SetToDefault(0, 0, 0, n);
	for (int i = 1; i < n; ++i) {
		AddCompareStep(0, i, 0, n, 2, isTemp);
		SetToDefault(0, i, 0, n);
		if (mainVector->GetAt(i) > mainVector->GetAt(n))
		{
			mainVector->SetAt(n, mainVector->GetAt(i));
			AddSetFromToStep(0, i, 0, n, 3, isTemp);
			SetToDefault(0, i, 0, n);
		}
	}
	int max = mainVector->GetAt(n);

	//移除临时变量
	Executor->SortVector->RemoveAtEnd();
	mainState->RemoveAtEnd();
	AddEmptyStep(isTemp);
	
	Executor->AssistVector = ref new Vector<int>(max + 1, 0);
	Executor->AssistStates = ref new Vector<int>(max + 1);
	auto assistVector = Executor->AssistVector;
	AddEmptyStep(isTemp);
	
	for (unsigned int index = 0; index < mainVector->Size; ++index)
	{
		int i = mainVector->GetAt(index);
		assistVector->SetAt(i, assistVector->GetAt(i) + 1);
		AddSetFromToStep(0, index, 1, i, 7);
		SetToDefault(0, index, 1, i);
	}
	int index = 0;
	for (int i = 0; i <= max; ++i) {
		for (int j = 0; j < assistVector->GetAt(i); ++j) {
			mainVector->SetAt(index, i);
			AddSetFromToStep(1, i, 0, index, 11);
			SetToDefault(1, i, 1, i);
			mainState->SetAt(index, (int)PillarState::Completed);
			index++;
		}
	}

	AddAllCompleteStep(0);

	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化桶排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitBucketSort()
{
	InitCountingSort();
	String^ Text1 = L"时间复杂度：O(n)\n桶排序的工作的原理是将数组分到有限数量的桶子里。每个桶子再个别排序（有可能再使用别的排序算法或是以递归方式继续使用桶排序进行排序）。\n";
	Introduction->Text = Text1 + L"\n桶排序的可视化较为困难，因此这里采用桶排序的特殊情况单桶排序进行代替。";
	auto highlighter = ref new TextHighlighter();
	highlighter->Background = ref new SolidColorBrush(Colors::Yellow);
	highlighter->Ranges->Append(TextRange{ (int)Text1->Length(), (int)Introduction->Text->Length() });
	Introduction->TextHighlighters->Append(highlighter);
}

/// <summary>
/// 初始化基数排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitRadixSort()
{
	Introduction->Text = L"时间复杂度：O(nmlog₂r)\n基数排序属于“分配式排序”，又称“桶子法”，顾名思义，它是透过键值的部份资讯，将要排序的元素分配至某些“桶”中，藉以达到排序的作用，基数排序法是属于稳定性的排序\n";

	auto codeDrawable = Executor->CodeDrawable;
	codeDrawable->Texts->Clear();
	codeDrawable->Texts->Append("int maxbit(const int data[]) {\n    int d = 1;\n    int p = 10;\n");
	codeDrawable->Texts->Append("    for (int i = 0; i < n; ++i) {\n");
	codeDrawable->Texts->Append("        while (data[i] >= p) {    \n            p *= 10;    \n            ++d;    \n        }    \n");
	codeDrawable->Texts->Append("    }\n    return d;\n}\n\n");
	codeDrawable->Texts->Append("void radixsort(int data[]) {\n    int d = maxbit(data);\n    int tmp[n];\n    int count[10];\n    int i, j, k;\n    int radix = 1;\n    for (i = 1; i <= d; i++) {\n");
	codeDrawable->Texts->Append("        for (j = 0; j < 10; j++)\n            count[j] = 0;\n");
	codeDrawable->Texts->Append("        for (j = 0; j < n; j++) {    \n            k = (data[j] / radix) % 10;    \n            count[k]++;    \n        }\n");
	codeDrawable->Texts->Append("        for (j = 1; j < 10; j++)    \n            count[j] = count[j - 1] + count[j];    \n");
	codeDrawable->Texts->Append("        for (j = n - 1; j >= 0; j--) {\n            k = (data[j] / radix) % 10;\n");
	codeDrawable->Texts->Append("            tmp[count[k] - 1] = data[j];    \n");
	codeDrawable->Texts->Append("            count[k]--;    \n");
	codeDrawable->Texts->Append("        }\n");
	codeDrawable->Texts->Append("        for (j = 0; j < n; j++)    \n            data[j] = tmp[j];    \n");
	codeDrawable->Texts->Append("        radix = radix * 10;\n    }\n}\n\n");
	codeDrawable->Texts->Append("int main() {\n    radixsort(arr);\n}\n");

	SpeedSlider->Value = 13; //默认滑块速度
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector; //主向量
	auto mainState = Executor->SortStates; //获取状态列表
	auto emptyVector = ref new Vector<int>(); //空向量
	AddRecoverStep(emptyVector); //一开始加入一个空的步骤

	auto isTemp = ref new Vector<int>();
	for (int i = 0; i < 10; ++i) isTemp->Append(n + i);

	RadixSort(mainVector);

	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 计算最大位数
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
int AlgorithmVisualization::SortAlgoPage::Maxbit(IVector<int>^ data)
{
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector;
	auto mainState = Executor->SortStates; //获取状态列表
	Executor->SortVector->Append(1); //临时变量
	mainState->Append((int)PillarState::Default); //临时变量
	Executor->SortVector->Append(10); //临时变量
	mainState->Append((int)PillarState::Default); //临时变量

	auto isTemp = ref new Vector<int>{ n, n + 1 };;
	AddEmptyStep(isTemp);

	for (int i = 0; i < n; ++i) {
		AddCompareStep(i, n + 1, 1, isTemp);
		SetToDefault(0, i, 0, i);
		while (data->GetAt(i) >= mainVector->GetAt(n + 1)) {
			mainVector->SetAt(n + 1, mainVector->GetAt(n + 1) * 10);
			mainVector->SetAt(n, mainVector->GetAt(n) + 1);
		}
	}
	SetToDefault(0, n, 0, n + 1);
	int d = mainVector->GetAt(n);

	Executor->SortVector->RemoveAtEnd();
	mainState->RemoveAtEnd();
	Executor->SortVector->RemoveAtEnd();
	mainState->RemoveAtEnd();
	AddEmptyStep(isTemp);

	return d;
}

/// <summary>
/// 基数排序
/// </summary>
/// <param name="data"></param>
void AlgorithmVisualization::SortAlgoPage::RadixSort(IVector<int>^ data)
{
	auto n = (int)Executor->SortVector->Size; //数字总数
	auto mainVector = Executor->SortVector;
	auto mainState = Executor->SortStates; //获取状态列表
	auto assistVector = Executor->AssistVector;
	int d = Maxbit(data);

	//添加临时变量
	auto isTemp = ref new Vector<int>();
	for (int i = 0; i < 10; ++i)
	{
		Executor->SortVector->Append(0); //临时变量
		mainState->Append((int)PillarState::Default); //临时变量
		isTemp->Append(n + i);
	}
	AddEmptyStep(isTemp);

	int i, j, k;
	int radix = 1;
	for (i = 1; i <= d; i++) {
		for (j = 0; j < 10; j++)
		{
			AddSetStep(n + j, 5, isTemp);
			mainVector->SetAt(n + j, 0);
		}
		for (j = 0; j < n; j++) {
			k = (data->GetAt(j) / radix) % 10;
			mainVector->SetAt(n + k, mainVector->GetAt(n + k) + 1);
			AddSetFromToStep(0, n + k, 0, n + k, 6, isTemp);
			SetToDefault(0, n + k, 0, n + k);
		}
		for (j = 1; j < 10; j++)
		{
			mainVector->SetAt(n + j, mainVector->GetAt(n + j - 1) + mainVector->GetAt(n + j));
			AddSetFromToStep(0, n + j - 1, 0, n + j, 7, isTemp);
			SetToDefault(0, n + j - 1, 0, n + j);
		}
		for (j = n - 1; j >= 0; j--) {
			k = (data->GetAt(j) / radix) % 10;
			assistVector->SetAt(mainVector->GetAt(n + k) - 1, mainVector->GetAt(j));
			AddSetFromToStep(0, j, 1, mainVector->GetAt(n + k) - 1, 9, isTemp);
			SetToDefault(0, j, 1, mainVector->GetAt(n + k) - 1);
			if (i == d) Executor->AssistStates->SetAt(mainVector->GetAt(n + k) - 1, (int)PillarState::Completed);
			mainVector->SetAt(n + k, mainVector->GetAt(n + k) - 1);
			AddSetFromToStep(0, n + k, 0, n + k, 10, isTemp);
			SetToDefault(0, n + k, 0, n + k);
		}
		if (i == d)
		{
			//移除临时变量
			for (int i = 0; i < 10; ++i)
			{
				Executor->SortVector->RemoveAtEnd();
				mainState->RemoveAtEnd();
			}
			AddEmptyStep(isTemp);
		}
		for (j = 0; j < n; j++)
		{
			mainVector->SetAt(j, assistVector->GetAt(j));
			AddSetFromToStep(1, j, 0, j, 12, isTemp);
			SetToDefault(1, j, 0, j);
			if (i == d)
			{
				mainState->SetAt(j, (int)PillarState::Completed);
				Executor->AssistStates->SetAt(j, (int)PillarState::Completed);
			}
		}
		radix = radix * 10;
	}

	AddEmptyStep(isTemp);
}

/// <summary>
/// 进度滑块的值改变时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::ProgressSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	Executor->NavigateToStep((int)e->NewValue);
}

/// <summary>
/// 监听速度滑块值的改变
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::SortAlgoPage::SpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	Executor->Speed = Executor->SpeedList->GetAt((unsigned int)e->NewValue); //从向量中获取对应的值
	SpeedText->Text = Executor->Speed / 1000.0 + "秒/步"; //将毫秒转换成秒
	if (Executor->TimerIsRunning())
	{
		//如果正在运行则以新的时间重新运行计时器
		Executor->StopTimer();
		StartThreadTimer();
	}
}
