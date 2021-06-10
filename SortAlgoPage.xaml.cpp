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
		Executor = ref new SortExcute(32, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitCountingSort();
	}
	else if (tag == L"BucketSort")
	{
		AlgorithmName->Text = "桶排序";
		Executor = ref new SortExcute(32, width, height, false); //实例化排序可执行 初等排序设置小一点
		InitExecutor();
		InitBucketSort();
	}
	else if (tag == L"RadixSort")
	{
		AlgorithmName->Text = "基数排序";
		Executor = ref new SortExcute(32, width, height, false); //实例化排序可执行 初等排序设置小一点
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
		SortHistogram->Children->Append(Executor->MainHistogram->container); //向父级容器中添加柱状图的容器

		auto AssistTextBlock = ref new TextBlock();
		AssistTextBlock->Text = L"辅助柱状图";
		AssistTextBlock->FontWeight = Windows::UI::Text::FontWeights::Bold;
		AssistTextBlock->Margin = Windows::UI::Xaml::Thickness(6);
		SortHistogram->Children->Append(AssistTextBlock); //向父级容器中添加文字
		SortHistogram->Children->Append(Executor->AssistHistogram->container); //向父级容器中添加柱状图的容器
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
		//ThisState中[5]保存辅助柱状图数组的临时标签
		
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
		
		if (thisStep->ThisState->Size == 2) //没有临时变量的时候
		{
			MainHistogram->load(thisStep->ThisState->GetAt(0), mainStateList); //加载步骤的内容
			if (NeedAssistHistogram)
				AssistHistogram->load(thisStep->ThisState->GetAt(3), assistStateList); //加载步骤的内容
		}
		else //有临时变量
		{
			MainHistogram->load(thisStep->ThisState->GetAt(0), mainStateList, thisStep->ThisState->GetAt(2)); //加载步骤的内容
			if (NeedAssistHistogram)
				AssistHistogram->load(thisStep->ThisState->GetAt(3), assistStateList, thisStep->ThisState->GetAt(5)); //加载步骤的内容
		}
		
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
	AssistVector = ref new Vector<int>(n, 0);
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
/// 添加空步骤
/// </summary>
/// <param name="stateList"></param>
bool AlgorithmVisualization::SortAlgoPage::AddEmptyStep(IVector<int>^ stateList, IVector<int>^ isTemp)
{
	auto emptyStep = ref new SingleStep(0); //比较步骤
	emptyStep->HighlightLines->Append(-1);
	emptyStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	emptyStep->ThisState->Append(Util::CopyVector(stateList));
	emptyStep->ThisState->Append(isTemp);
	Executor->AddStep(emptyStep);
	return true;
}

/// <summary>
/// 添加设定步骤
/// </summary>
/// <param name="stateList"></param>
/// <param name="pos"></param>
/// <param name="highlightLine"></param>
bool AlgorithmVisualization::SortAlgoPage::AddSetStep(IVector<int>^ stateList, int pos, int highlightLine, IVector<int>^ isTemp)
{
	auto setStep = ref new SingleStep(0); //设置步骤
	setStep->HighlightLines->Append(highlightLine);
	setStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	stateList->SetAt(pos, (int)PillarState::SetValue); //设置去向
	setStep->ThisState->Append(Util::CopyVector(stateList));
	setStep->ThisState->Append(isTemp);
	Executor->AddStep(setStep);
	return true;
}

bool AlgorithmVisualization::SortAlgoPage::AddSetFromToStep(IVector<int>^ stateList, int from, int to, int highlightLine, IVector<int>^ isTemp)
{
	auto setStep = ref new SingleStep(0); //设置步骤
	setStep->HighlightLines->Append(highlightLine);
	setStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	stateList->SetAt(from, (int)PillarState::Selected); //设置来源
	stateList->SetAt(to, (int)PillarState::SetValue); //设置去向
	setStep->ThisState->Append(Util::CopyVector(stateList));
	setStep->ThisState->Append(isTemp);
	Executor->AddStep(setStep);
	return true;
}

/// <summary>
/// 添加比较步骤
/// </summary>
/// <param name="stateList">状态列表</param>
/// <param name="left">左数字</param>
/// <param name="right">右数字</param>
/// <param name="highlightLine">高亮行</param>
bool AlgorithmVisualization::SortAlgoPage::AddCompareStep(IVector<int>^ stateList, int left, int right, int highlightLine, IVector<int>^ isTemp)
{
	auto compareStep = ref new SingleStep(0); //比较步骤
	compareStep->HighlightLines->Append(highlightLine);
	compareStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//设置两个位置为比较状态
	stateList->SetAt(left, (int)PillarState::Compared);
	stateList->SetAt(right, (int)PillarState::Compared);
	compareStep->ThisState->Append(Util::CopyVector(stateList));
	compareStep->ThisState->Append(isTemp);
	Executor->AddStep(compareStep);
	return true;
}

/// <summary>
/// 添加交换步骤
/// </summary>
/// <param name="stateList">状态列表</param>
/// <param name="left">左位置</param>
/// <param name="right">右位置</param>
/// <param name="highlightLine">高亮行数</param>
bool AlgorithmVisualization::SortAlgoPage::AddSwapStep(IVector<int>^ stateList, int left, int right, int highlightLine, IVector<int>^ isTemp)
{
	auto swapStep = ref new SingleStep(1);
	swapStep->HighlightLines->Append(highlightLine);
	swapStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	//设置两个位置为交换状态
	stateList->SetAt(left, (int)PillarState::Swapping);
	stateList->SetAt(right, (int)PillarState::Swapping);
	swapStep->ThisState->Append(Util::CopyVector(stateList));
	swapStep->ThisState->Append(isTemp);
	Executor->AddStep(swapStep);
	return true;
}

/// <summary>
/// 添加恢复步骤
/// </summary>
/// <param name="stateList">状态列表</param>
/// <param name="recover">恢复位置</param>
bool AlgorithmVisualization::SortAlgoPage::AddRecoverStep(IVector<int>^ stateList, IVector<int>^ recover, IVector<int>^ isTemp)
{
	auto recoverStep = ref new SingleStep(2);
	recoverStep->HighlightLines->Append(-1);
	recoverStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (auto i : recover)
	{
		stateList->SetAt(i, (int)PillarState::Default); //遍历设为默认状态
	}
	recoverStep->ThisState->Append(Util::CopyVector(stateList));
	recoverStep->ThisState->Append(isTemp);
	Executor->AddStep(recoverStep);
	return true;
}

/// <summary>
/// 添加完成状态
/// </summary>
/// <param name="stateList">状态列表</param>
/// <param name="complete">完成的位置</param>
bool AlgorithmVisualization::SortAlgoPage::AddCompleteStep(IVector<int>^ stateList, IVector<int>^ complete, IVector<int>^ isTemp)
{
	auto endStep = ref new SingleStep(3);
	endStep->HighlightLines->Append(-1);
	endStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (auto i : complete)
	{
		stateList->SetAt(i, (int)PillarState::Completed); //遍历设为完成状态
	}
	endStep->ThisState->Append(Util::CopyVector(stateList));
	endStep->ThisState->Append(isTemp);
	Executor->AddStep(endStep);
	return true;
}

/// <summary>
/// 添加选中状态
/// </summary>
/// <param name="stateList"></param>
/// <param name="select"></param>
bool AlgorithmVisualization::SortAlgoPage::AddSelectStep(IVector<int>^ stateList, IVector<int>^ select, int highlightLine, IVector<int>^ isTemp)
{
	auto selectStep = ref new SingleStep(4);
	selectStep->HighlightLines->Append(highlightLine);
	selectStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (auto i : select)
	{
		stateList->SetAt(i, (int)PillarState::Selected); //遍历设为完成状态
	}
	selectStep->ThisState->Append(Util::CopyVector(stateList));
	selectStep->ThisState->Append(isTemp);
	Executor->AddStep(selectStep);
	return true;
}

/// <summary>
/// 全部完成
/// </summary>
/// <param name="stateList"></param>
bool AlgorithmVisualization::SortAlgoPage::AddAllCompleteStep(IVector<int>^ stateList, IVector<int>^ isTemp)
{
	auto endStep = ref new SingleStep(3);
	endStep->HighlightLines->Append(-1);
	endStep->ThisState->Append(Util::CopyVector(Executor->SortVector));
	for (unsigned int i = 0; i < stateList->Size; ++i)
	{
		stateList->SetAt(i, (int)PillarState::Completed); //遍历设为完成状态
	}
	endStep->ThisState->Append(Util::CopyVector(stateList));
	endStep->ThisState->Append(isTemp);
	Executor->AddStep(endStep);
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
	auto stateList = ref new Vector<int>(n, (int)PillarState::Default); //实例化状态列表
	AddRecoverStep(stateList, ref new Vector<int>()); //一开始加入一个空的步骤

	for (auto i = 0; i < n - 1; i++)
	{
		for (auto j = 0; j < n - i - 1; j++)
		{
			AddCompareStep(stateList, j, j + 1, 1); //加入比较步骤
			if (Executor->SortVector->GetAt(j) > Executor->SortVector->GetAt(j + 1)) { //如果后者更大
				//交换j和j+1位置上的数字
				int temp = Executor->SortVector->GetAt(j + 1);
				Executor->SortVector->SetAt(j + 1, Executor->SortVector->GetAt(j));
				Executor->SortVector->SetAt(j, temp);
				//加入交换步骤
				AddSwapStep(stateList, j, j + 1, 2);
			}
			stateList->SetAt(j, (int)PillarState::Default); //设置前者为默认状态
		}
		AddCompleteStep(stateList, ref new Vector<int>{ (int)n - i - 1 }); //设置本次的末尾完成
	}
	AddCompleteStep(stateList, ref new Vector<int>{ 0 }); //加入完成步骤
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
	auto stateList = ref new Vector<int>(n, (int)PillarState::Default); //实例化状态列表
	AddRecoverStep(stateList, ref new Vector<int>()); //一开始加入一个空的步骤

	for (int i = 0; i < n - 1; ++i) {
		int min = i; //初始化最小的下标
		for (int j = i + 1; j < n; ++j) {
			AddCompareStep(stateList, min, j, 1); //加入比较步骤
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
		AddSwapStep(stateList, i, min, 3);
		AddRecoverStep(stateList, ref new Vector<int>{ i, min });
		AddCompleteStep(stateList, ref new Vector<int>{ i }); //加入完成步骤
	}

	AddCompleteStep(stateList, ref new Vector<int>{ n - 1 }); //加入完成步骤
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
	auto stateList = ref new Vector<int>(n, (int)PillarState::Default); //实例化状态列表
	AddRecoverStep(stateList, ref new Vector<int>()); //一开始加入一个空的步骤

	for (int i = 0; i < n; i++) {
		for (int j = i; j > 0; j--) {
			AddCompareStep(stateList, j - 1, j, 2); //加入比较步骤
			//恢复默认状态
			stateList->SetAt(j, (int)PillarState::Default);
			stateList->SetAt(j - 1, (int)PillarState::Default); 
			if (Executor->SortVector->GetAt(j) < Executor->SortVector->GetAt(j - 1)) //比较
			{
				AddSwapStep(stateList, j - 1, j, 3); //添加交换步骤
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
	
	AddAllCompleteStep(stateList); //最后全部完成
	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化希尔排序算法
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitShellSort()
{
	Introduction->Text = L"时间复杂度：O(n²)\n希尔排序是直接插入排序算法的一种更高效的改进版本，是非稳定排序算法。希尔排序是把记录按下标的一定增量分组，对每组使用直接插入排序算法排序；随着增量逐渐减少，每组包含的关键词越来越多，当增量减至 1 时，整个文件恰被分成一组，算法便终止。";

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
	auto stateList = ref new Vector<int>(n, (int)PillarState::Default); //实例化状态列表
	AddRecoverStep(stateList, ref new Vector<int>()); //一开始加入一个空的步骤
	
	Executor->SortVector->Append(0); //临时变量
	stateList->Append((int)PillarState::Default); //临时变量
	auto isTemp = ref new Vector<int>{ n };
	AddEmptyStep(stateList, isTemp);

	for (int gap = n / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < n; ++i) {
			Executor->SortVector->SetAt(n, Executor->SortVector->GetAt(i));
			AddSetFromToStep(stateList, i, n, 2, isTemp);
			stateList->SetAt(i, (int)PillarState::Default);
			int j;
			for (j = i; j >= gap; j -= gap) {
				AddCompareStep(stateList, j - gap, n, 5, isTemp);
				stateList->SetAt(n, (int)PillarState::Default);
				stateList->SetAt(j - gap, (int)PillarState::Default);
				if (Executor->SortVector->GetAt(n) < Executor->SortVector->GetAt(j - gap))
				{
					Executor->SortVector->SetAt(j, Executor->SortVector->GetAt(j - gap));
					stateList->SetAt(n, (int)PillarState::Default);
					AddSetFromToStep(stateList, j - gap, j, 6, isTemp);
					stateList->SetAt(j, (int)PillarState::Default);
				}
				else break;
			}
			Executor->SortVector->SetAt(j, Executor->SortVector->GetAt(n));
			AddSetFromToStep(stateList, n, j, 9, isTemp);
			stateList->SetAt(j, (int)PillarState::Default);
		}
	}

	for (auto &i : stateList)
	{
		i = (int)PillarState::Completed;
	}
	stateList->SetAt(n, (int)PillarState::Default); //临时变量
	AddEmptyStep(stateList, isTemp);

	//移除临时变量
	Executor->SortVector->RemoveAtEnd();
	stateList->RemoveAtEnd();
	AddEmptyStep(stateList, isTemp);
	
	ProcessText->Text = "0/" + (Executor->StepList->Size - 1).ToString(); //在初始化完毕后设置文本
	ProgressSlider->Maximum = Executor->StepList->Size - 1; //设置进度滑块最大值
}

/// <summary>
/// 初始化归并排序
/// </summary>
void AlgorithmVisualization::SortAlgoPage::InitMergeSort()
{
	throw ref new Platform::NotImplementedException();
}

void AlgorithmVisualization::SortAlgoPage::InitQuickSort()
{
	throw ref new Platform::NotImplementedException();
}

void AlgorithmVisualization::SortAlgoPage::InitHeapSort()
{
	throw ref new Platform::NotImplementedException();
}

void AlgorithmVisualization::SortAlgoPage::InitCountingSort()
{
	throw ref new Platform::NotImplementedException();
}

void AlgorithmVisualization::SortAlgoPage::InitBucketSort()
{
	throw ref new Platform::NotImplementedException();
}

void AlgorithmVisualization::SortAlgoPage::InitRadixSort()
{
	throw ref new Platform::NotImplementedException();
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
