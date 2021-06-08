#include "pch.h"
#include "ExecuteStep.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace AlgorithmVisualization;
using namespace Windows::System::Threading;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;

/// <summary>
/// 构造函数
/// </summary>
/// <param name="id"></param>
/// <param name="type"></param>
AlgorithmVisualization::SingleStep::SingleStep(int type)
{
	this->Type = type;
	HighlightLines = ref new Vector<int>();
	ThisState = ref new Vector<IVector<int>^>();
	ToNextStep = ref new Vector<ExcuteInstruct^>();
}

/// <summary>
/// 步骤计数
/// </summary>
/// <returns>步骤总数</returns>
int AlgorithmVisualization::ExecuteStep::CountStep()
{
	return StepList->Size;
}

/// <summary>
/// 加入步骤
/// </summary>
/// <param name="step">要插入的步骤</param>
/// <returns>步骤总数</returns>
int AlgorithmVisualization::ExecuteStep::AddStep(SingleStep^ step)
{
	StepList->Append(step);
	return CountStep();
}

/// <summary>
/// 删除指定步骤
/// </summary>
/// <param name="index"></param>
/// <returns></returns>
int AlgorithmVisualization::ExecuteStep::RemoveStep(int index)
{
	StepList->RemoveAt(index);
	return CountStep();
}

/// <summary>
/// 清空步骤
/// </summary>
void AlgorithmVisualization::ExecuteStep::ClearStep()
{
	StepList->Clear();
}

/// <summary>
/// 导航到第一步
/// </summary>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::ExecuteStep::NavigateToFirst()
{
	return NavigateToStep(0);
}

/// <summary>
/// 导航到最后一步
/// </summary>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::ExecuteStep::NavigateToLast()
{
	return NavigateToStep(CountStep() - 1);
}

/// <summary>
/// 是否是第一步
/// </summary>
/// <returns></returns>
bool AlgorithmVisualization::ExecuteStep::IsFirstStep()
{
	return CurrentStep == 0;
}

/// <summary>
/// 是否是最后一步
/// </summary>
/// <returns></returns>
bool AlgorithmVisualization::ExecuteStep::IsLastStep()
{
	if (StepList->Size != 0) return CurrentStep == StepList->Size - 1;
	else return true;
}

/// <summary>
/// 获取第一步
/// </summary>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::ExecuteStep::GetFirstStep()
{
	if (StepList->Size == 0) return nullptr;
	else return StepList->GetAt(0);
}

/// <summary>
/// 获取最后一步
/// </summary>
/// <returns></returns>
SingleStep^ AlgorithmVisualization::ExecuteStep::GetLastStep()
{
	if (StepList->Size == 0) return nullptr;
	else return StepList->GetAt(StepList->Size - 1);
}

void AlgorithmVisualization::ExecuteStep::DefaultInit()
{
	SpeedList = ref new Vector<int>{ 0, 20, 40, 60, 80,
		100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600, 650, 700, 750, 800, 850, 900, 950,
		1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };
	StepList = ref new Vector<SingleStep^>();
	CurrentStep = 0;
	Speed = 200;
	ThreadTimer = nullptr;
}

bool AlgorithmVisualization::ExecuteStep::TimerIsRunning()
{
	return ThreadTimer != nullptr;
}

void AlgorithmVisualization::ExecuteStep::StopTimer()
{
	if (ThreadTimer != nullptr)
	{
		ThreadTimer->Cancel();
		ThreadTimer = nullptr;
	}
}
