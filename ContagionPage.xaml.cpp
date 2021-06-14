//
// ContagionPage.xaml.cpp
// ContagionPage 类的实现
//

#include "pch.h"
#include "Grid.h"
#include "ContagionPage.xaml.h"

using namespace AlgorithmVisualization;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

/// <summary>
/// 构造函数
/// </summary>
ContagionPage::ContagionPage()
{
	InitializeComponent();
	ContagionModelType type = (ContagionModelType)((App^)(Application::Current))->contagionAlgorithmType; //传染病模型类型

	int rows = 30;
	int cols = 60;
	InfectiousGrid = ref new Grid(lastGridWidth, lastGridWidth / cols * rows, 
		rows, cols, RectState::Susceptible);
	ContagionGrid->Children->Append(InfectiousGrid->GetView());

	InitAlgorithm(type);
}

/// <summary>
/// 尺寸改变时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::ContagionGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	lastGridWidth = e->NewSize.Width;
	lastGridHeight = e->NewSize.Height;

	InfectiousGrid->OnSizeChanged(lastGridWidth, lastGridWidth / InfectiousGrid->cols * InfectiousGrid->rows);
}

/// <summary>
/// 调试按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::DebugBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
}

/// <summary>
/// 初始化算法
/// </summary>
/// <param name="type"></param>
void AlgorithmVisualization::ContagionPage::InitAlgorithm(ContagionModelType type)
{
	InfectiousGrid->GetCenter()->ChangeState(RectState::Infectious);
	
	switch (type)
	{
	case ContagionModelType::SIModel:
		AlgorithmName->Text = L"SI模型";
		InitSIModel();
		break;
	case ContagionModelType::SISModel:
		AlgorithmName->Text = L"SIS模型";
		InitSISModel();
		break;
	case ContagionModelType::SIRModel:
		AlgorithmName->Text = L"SIR模型";
		InitSIRModel();
		break;
	case ContagionModelType::SEIRModel:
		AlgorithmName->Text = L"SEIR模型";
		InitSEIRModel();
		break;
	case ContagionModelType::SEIARModel:
		AlgorithmName->Text = L"SEIAR模型";
		InitSEIARModel();
		break;
	default:
		break;
	}
}

/// <summary>
/// 初始化SI模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSIModel()
{
	NavToNextStep = &ContagionPage::SINextStep;
	
}

/// <summary>
/// 初始化SIS模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSISModel()
{
	NavToNextStep = &ContagionPage::SISNextStep;
	
}

/// <summary>
/// 初始化SIR模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSIRModel()
{
	NavToNextStep = &ContagionPage::SIRNextStep;

}

/// <summary>
/// 初始化SEIR模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSEIRModel()
{
	NavToNextStep = &ContagionPage::SEIRNextStep;

}

/// <summary>
/// 初始化SEIAR模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSEIARModel()
{
	NavToNextStep = &ContagionPage::SEIARNextStep;

}

/// <summary>
/// SI下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SINextStep()
{
	
}

/// <summary>
/// SIS下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SISNextStep()
{

}

/// <summary>
/// SIR下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SIRNextStep()
{

}

/// <summary>
/// SEIR下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SEIRNextStep()
{

}

/// <summary>
/// SEIAR下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SEIARNextStep()
{

}

/// <summary>
/// 计时器是否在运行
/// </summary>
/// <returns></returns>
bool AlgorithmVisualization::ContagionPage::IsTimerRunning()
{
	return ThreadTimer != nullptr;
}

/// <summary>
/// 结束计时器
/// </summary>
void AlgorithmVisualization::ContagionPage::StopTimer()
{
	if (ThreadTimer != nullptr)
	{
		ThreadTimer->Cancel();
		ThreadTimer = nullptr;
	}
}

/// <summary>
/// 开始执行
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!IsTimerRunning())
	{
		Windows::Foundation::TimeSpan period{};
		period.Duration = Speed * 10000;  //设置时间间隔
		ThreadTimer = ThreadPoolTimer::CreatePeriodicTimer(
			ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
				{
					CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, 
						ref new DispatchedHandler([this]()
						{
							(this->*NavToNextStep)();
						}
					));
				}), period
		);
	}
}

/// <summary>
/// 暂停执行
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::Pause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	StopTimer();
}
