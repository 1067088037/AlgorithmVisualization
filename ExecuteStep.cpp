#include "pch.h"
#include "ExecuteStep.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace AlgorithmVisualization;

/// <summary>
/// 构造函数
/// </summary>
/// <param name="id"></param>
/// <param name="type"></param>
AlgorithmVisualization::SingleStep::SingleStep(int id, int type)
{
	this->Index = id;
	this->Type = type;
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
