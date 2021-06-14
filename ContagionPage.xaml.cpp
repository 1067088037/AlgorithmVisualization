//
// ContagionPage.xaml.cpp
// ContagionPage 类的实现
//

#include "pch.h"
#include "Grid.h"
#include "Util.h"
#include <random>
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
	srand(GetTickCount());
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
	GetThisState();
	
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
/// 获取当前步骤
/// </summary>
/// <returns></returns>
void AlgorithmVisualization::ContagionPage::GetThisState()
{
	auto res = ref new Vector<IVector<RectState>^>(InfectiousGrid->cols);
	for (int i = 0; i < InfectiousGrid->cols; i++)
	{
		auto resRow = ref new Vector<RectState>(InfectiousGrid->rows);
		for (int j = 0; j < InfectiousGrid->rows; j++)
		{
			resRow->SetAt(j, InfectiousGrid->Get(i, j)->CurrentState);
		}
		res->SetAt(i, resRow);
	}
	NextStateVector = res;
	ThisStateVector = Util::Copy2DVector(res);
}

/// <summary>
/// 加载下一步状态
/// </summary>
void AlgorithmVisualization::ContagionPage::LoadNextState()
{
	for (int i = 0; i < InfectiousGrid->cols; i++)
	{
		for (int j = 0; j < InfectiousGrid->rows; j++)
		{
			InfectiousGrid->SetState(i, j, NextStateVector->GetAt(i)->GetAt(j));
		}
	}
}

/// <summary>
/// 初始化SI模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSIModel()
{
	NavToNextStep = &ContagionPage::SINextStep;
	Introduction->Text = L"SI传播模型是最简单的疾病传播模型，模型中的所有个体都只可能处于两个状态中的一个：即易感(S)状态或感染(I)状态。\nSI模型中的个体一旦被感染后就永远处于感染状态。";
	FitInfectiousDisease->Text = L"适用于：艾滋病";
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
	for (int i = 0; i < xMax; i++)
	{
		for (int j = 0; j < yMax; j++)
		{
			if (GetState(i, j, false) == RectState::Infectious)
				InfectNear(i, j, ContactPeopleCount / 10, InfectiousRate);
		}
	}	
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
/// 获取状态
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
RectState AlgorithmVisualization::ContagionPage::GetState(int x, int y, bool next)
{
	if (next)
		return NextStateVector->GetAt(x)->GetAt(y);
	else
		return ThisStateVector->GetAt(x)->GetAt(y);
}

/// <summary>
/// 设置状态
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="newState"></param>
void AlgorithmVisualization::ContagionPage::SetState(int x, int y, RectState newState, bool next)
{
	if (next)
		NextStateVector->GetAt(x)->SetAt(y, newState);
	else
		ThisStateVector->GetAt(x)->SetAt(y, newState);
}

/// <summary>
/// 感染临近的人
/// </summary>
/// <param name="srcX"></param>
/// <param name="srcY"></param>
/// <param name="nearCount"></param>
/// <param name="probability"></param>
void AlgorithmVisualization::ContagionPage::InfectNear(int srcX, int srcY, int contactCount, double probability)
{
	if (contactCount < 0 || contactCount > 8) 
		throw ref new InvalidArgumentException(L"临近的人必须是0~8的整数");

	//获取邻近的人的状态
	auto nearPeople = ref new Vector<Point^>();
	for (int i = 0; i < 8; i++)
	{
		int newX = srcX + DirectionX[i];
		int newY = srcY + DirectionY[i];
		if (CheckPointValid(newX, newY))
		{
			nearPeople->Append(ref new Point{ newX, newY });
		}
	}

	while ((int)nearPeople->Size > contactCount) //当目前的人数大于接触到的人数时
	{
		double rand1 = random(e);
		int removeAt = (int)(rand1 * 100000000) % nearPeople->Size;
		nearPeople->RemoveAt(removeAt); //随机删除其中一个元素
	}

	for (unsigned int i = 0; i < nearPeople->Size; ++i)
	{
		auto people = nearPeople->GetAt(i);
		if (random(e) <= probability)
		{
			SetState(people->x, people->y, RectState::Infectious, true);
		}
	}
}

/// <summary>
/// 检查点位的合法性
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool AlgorithmVisualization::ContagionPage::CheckPointValid(int x, int y)
{
	if (x < 0 || y < 0) return false;
	if (x >= InfectiousGrid->cols || y >= InfectiousGrid->rows) return false;
	return true;
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
/// 启动计时器
/// </summary>
void AlgorithmVisualization::ContagionPage::StartTimer()
{
	if (!IsTimerRunning())
	{
		Windows::Foundation::TimeSpan period{};
		period.Duration = Speed * 10000;  //设置时间间隔
		ThreadTimer = ThreadPoolTimer::CreatePeriodicTimer(
			ref new TimerElapsedHandler([this](ThreadPoolTimer^ source)
				{
					GetThisState();
					(this->*NavToNextStep)(); //计算出下一步的情况

					if (IsTimerRunning())
					{
						CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High,
							ref new DispatchedHandler([this]()
								{
									LoadNextState(); //将下一步加载到UI上
								}
						));
					}
				}), period
		);
	}
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
/// 下一步
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::Next_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	GetThisState();
	(this->*NavToNextStep)(); //计算出下一步的情况
	LoadNextState(); //将下一步加载到UI上
}

/// <summary>
/// 时间流逝速度改变
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::TimeElapseSpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	Speed = (int64)(e->NewValue * 1000);
	TimeElapseSpeedText->Text = e->NewValue + L"s";
	if (IsTimerRunning())
	{
		StopTimer();
		StartTimer();
	}
}

/// <summary>
/// 接触到的人数回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::ContactPeopleCountSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	ContactPeopleCountText->Text = e->NewValue + L"人";
	ContactPeopleCount = (int)e->NewValue;
}

/// <summary>
/// 感染率改变时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::InfectiousRateSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	InfectiousRateText->Text = (int)e->NewValue + L"%";
	InfectiousRate = e->NewValue / 100;
}


void AlgorithmVisualization::ContagionPage::StartOrPause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (IsTimerRunning())
	{
		StopTimer();
		Next->IsEnabled = true;
		StartOrPause->Content = L"开始";
	}
	else
	{
		StartTimer();
		Next->IsEnabled = false;
		StartOrPause->Content = L"暂停";
	}
}
