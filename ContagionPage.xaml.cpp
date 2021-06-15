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
	currentType = type;
	InitAlgorithm(type);
}

AlgorithmVisualization::ContagionPage::~ContagionPage()
{
	StopTimer(); //关闭计时器
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
	StopTimer();
	Next->IsEnabled = true;
	StartOrPause->Content = L"开始";

	ContagionGrid->Children->Clear();
	int rows = 30;
	int cols = 60;
	InfectiousGrid = ref new Grid(lastGridWidth, lastGridWidth / cols * rows,
		rows, cols, RectState::Susceptible);
	ContagionGrid->Children->Append(InfectiousGrid->GetView());
	
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
			if (this == nullptr || InfectiousGrid == nullptr) return;
			if (((App^)Application::Current)->contagionAlgorithmType == -1) return;
			auto state = NextStateVector->GetAt(i)->GetAt(j);
			InfectiousGrid->SetState(i, j, state);
		}
	}
}

/// <summary>
/// 初始化SI模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSIModel()
{
	NavToNextStep = &ContagionPage::SINextStep;
	Introduction->Text = L"SI传播模型是最简单的疾病传播模型，模型中的所有个体都只可能处于两个状态中的一个：即易感(S)状态或感染(I)状态。SI模型中的个体一旦被感染后就永远处于感染状态。";
	FitInfectiousDisease->Text = L"适用于：艾滋病";

	RecoveryRateBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	ExposedToInfectiousBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	ExposedToAsymptomaticBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	AsymptomaticRecoveryBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

/// <summary>
/// 初始化SIS模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSISModel()
{
	NavToNextStep = &ContagionPage::SISNextStep;
	Introduction->Text = L"SIS传播模型在SI模型基础上加入康复的概率，即治愈率。该模型适用于只有易感者和患病者两类人群，但会反复发作的疾病。易感者与患病者有效接触即被感染，变为患病者，可被治愈再次变为易感者，无潜伏期、无免疫力。";
	FitInfectiousDisease->Text = L"适用于：细菌性痢疾";

	ExposedToInfectiousBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	ExposedToAsymptomaticBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	AsymptomaticRecoveryBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

/// <summary>
/// 初始化SIR模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSIRModel()
{
	NavToNextStep = &ContagionPage::SIRNextStep;
	Introduction->Text = L"适用于有易感者、患病者和康复者三类人群，治愈后不会再发的疾病。康复者具有很强免疫力，不会被再次感染。对于致死性的传染病也可以使用这个模型，死亡的病人也可以归入康复者。此时的康复者可以理解为退出了传染系统。";
	FitInfectiousDisease->Text = L"适用于：水痘";

	ExposedToInfectiousBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	ExposedToAsymptomaticBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	AsymptomaticRecoveryBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

/// <summary>
/// 初始化SEIR模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSEIRModel()
{
	NavToNextStep = &ContagionPage::SEIRNextStep;
	Introduction->Text = L"许多疾病存在潜伏期，在此期间个人不能感染其它人并有一定概率转化为感染者。在此类模型中，个体经历了较长的潜伏期，因此个体已被感染但尚未具有传染力。该模型在SIR模型中增加了E（暴露者）用于表示处于潜伏期的个体。";
	FitInfectiousDisease->Text = L"适用于：登革热";

	ExposedToAsymptomaticBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	AsymptomaticRecoveryBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

/// <summary>
/// 初始化SEIAR模型
/// </summary>
void AlgorithmVisualization::ContagionPage::InitSEIARModel()
{
	NavToNextStep = &ContagionPage::SEIARNextStep;
	Introduction->Text = L"部分疾病存在具有传染性的无症状感染者，无症状意味着这类人群可能无法察觉自己患病进而无法及时就医，造成更大的人口流动性和更低的康复率。在SEIAR模型中，引入了无症状感染者A，把SEIR模型中的I进一步分为I和A。";
	FitInfectiousDisease->Text = L"适用于：2019-nCov 新型冠状病毒肺炎";
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
			if (GetState(i, j, false) == RectState::Infectious) //如果感染则可以传染给别人
				InfectNear(i, j, ContactPeopleCount / 10, InfectiousRate);
		}
	}	
}

/// <summary>
/// SIS下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SISNextStep()
{
	for (int i = 0; i < xMax; i++)
	{
		for (int j = 0; j < yMax; j++)
		{
			if (GetState(i, j, false) == RectState::Infectious)
			{
				if (random(e) <= RecoveryRate)
				{
					SetState(i, j, RectState::Susceptible, true); //在一定概率下转化为易感者
				}
				else
				{
					InfectNear(i, j, ContactPeopleCount / 10, InfectiousRate); //如果没有痊愈则继续传染
				}
			}
		}
	}
}

/// <summary>
/// SIR下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SIRNextStep()
{
	for (int i = 0; i < xMax; i++)
	{
		for (int j = 0; j < yMax; j++)
		{
			if (GetState(i, j, false) == RectState::Infectious)
			{
				if (random(e) <= RecoveryRate)
				{
					SetState(i, j, RectState::Recovered, true); //在一定概率下转化为康复者
				}
				else
				{
					InfectNear(i, j, ContactPeopleCount / 10, InfectiousRate); //如果没有痊愈则继续传染
				}
			}
		}
	}
}

/// <summary>
/// SEIR下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SEIRNextStep()
{
	for (int i = 0; i < xMax; i++)
	{
		for (int j = 0; j < yMax; j++)
		{
			if (GetState(i, j, false) == RectState::Infectious)
			{
				if (random(e) <= RecoveryRate)
				{
					SetState(i, j, RectState::Recovered, true); //在一定概率下转化为康复者
				}
				else
				{
					ChangeToExposed(i, j, ContactPeopleCount / 10, InfectiousRate); //如果没有痊愈则继续传染
				}
			}
			else if (GetState(i, j, false) == RectState::Exposed)
			{
				if (random(e) <= ExposedToInfectiousRate)
				{
					SetState(i, j, RectState::Infectious, true); //在一定概率下转化为感染者
				}
			}
		}
	}
}

/// <summary>
/// SEIAR下一步
/// </summary>
void AlgorithmVisualization::ContagionPage::SEIARNextStep()
{
	for (int i = 0; i < xMax; i++)
	{
		for (int j = 0; j < yMax; j++)
		{
			auto rectState = GetState(i, j, false);
			if (rectState == RectState::Infectious)
			{
				if (random(e) <= RecoveryRate)
				{
					SetState(i, j, RectState::Recovered, true); //在一定概率下转化为康复者
				}
				else
				{
					ChangeToExposed(i, j, ContactPeopleCount / 10, InfectiousRate); //如果没有痊愈则继续传染
				}
			}
			else if (rectState == RectState::Asymptomatic)
			{
				if (random(e) <= AsymptomaticRecoveryRate)
				{
					SetState(i, j, RectState::Recovered, true); //在一定概率下转化为康复者
				}
				else
				{
					ChangeToExposed(i, j, ContactPeopleCount / 10, InfectiousRate); //如果没有痊愈则继续传染
				}
			}
			else if (rectState == RectState::Exposed)
			{
				if (random(e) <= ExposedToInfectiousRate)
				{
					SetState(i, j, RectState::Infectious, true); //在一定概率下转化为感染者
				}
				else if (random(e) <= ExposedToAsymptomaticRate)
				{
					SetState(i, j, RectState::Asymptomatic, true); //在一定概率下转化为隐性感染者
				}
			}
		}
	}
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
/// 改变周围的人
/// </summary>
/// <param name="srcX"></param>
/// <param name="srcY"></param>
/// <param name="nearCount"></param>
/// <param name="probability"></param>
void AlgorithmVisualization::ContagionPage::ChangeNear(int srcX, int srcY, int contactCount, double probability, RectState from, RectState to)
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
		if (GetState(people->x, people->y) == from && random(e) <= probability)
		{
			SetState(people->x, people->y, to, true);
		}
	}
}

/// <summary>
/// 感染周围人
/// </summary>
/// <param name="srcX"></param>
/// <param name="srcY"></param>
/// <param name="nearCount"></param>
/// <param name="probability"></param>
void AlgorithmVisualization::ContagionPage::InfectNear(int srcX, int srcY, int nearCount, double probability)
{
	ChangeNear(srcX, srcY, nearCount, probability, RectState::Susceptible, RectState::Infectious);
}

/// <summary>
/// 使周围人变为潜伏者
/// </summary>
/// <param name="srcX"></param>
/// <param name="srcY"></param>
/// <param name="nearCount"></param>
/// <param name="probability"></param>
void AlgorithmVisualization::ContagionPage::ChangeToExposed(int srcX, int srcY, int nearCount, double probability)
{
	ChangeNear(srcX, srcY, nearCount, probability, RectState::Susceptible, RectState::Exposed);
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

					if (IsTimerRunning() && this != nullptr)
					{
						CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::Normal,
							ref new DispatchedHandler([this]()
								{
									if (IsTimerRunning() && this != nullptr)
									{
										LoadNextState(); //将下一步加载到UI上
									}
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

/// <summary>
/// 开始和暂停按钮
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
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

/// <summary>
/// 恢复率改变时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::RecoveryRateSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	RecoveryRateText->Text = (int)e->NewValue + L"%";
	RecoveryRate = e->NewValue / 100;
}

/// <summary>
/// 重置
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::Reset_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	InitAlgorithm(currentType);
}

/// <summary>
/// 潜伏者转正概率改变回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::ExposedToInfectiousSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	ExposedToInfectiousRate = e->NewValue / 100;
	if (ExposedToInfectiousRate + ExposedToAsymptomaticRate >= 1.0)
	{
		ExposedToAsymptomaticRate = 1 - ExposedToInfectiousRate;
		ExposedToAsymptomaticSlider->Value = ExposedToAsymptomaticRate * 100;
	}
}

/// <summary>
/// 潜伏者转隐性感染者概率改变时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::ExposedToAsymptomaticSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	ExposedToAsymptomaticRate = e->NewValue / 100;
	if (ExposedToInfectiousRate + ExposedToAsymptomaticRate >= 1.0)
	{
		ExposedToInfectiousRate = 1 - ExposedToAsymptomaticRate;
		ExposedToInfectiousSlider->Value = ExposedToInfectiousRate * 100;
	}
}

/// <summary>
/// 隐性感染者康复率改变回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::ContagionPage::AsymptomaticRecoverySlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	AsymptomaticRecoveryText->Text = (int)e->NewValue + L"%";
	AsymptomaticRecoveryRate = e->NewValue / 100;
}
