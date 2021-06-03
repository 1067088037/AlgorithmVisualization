#pragma once

using namespace Platform;
using namespace AlgorithmVisualization;
using namespace Windows::Foundation::Collections;

namespace AlgorithmVisualization {
	/// <summary>
	/// 执行指令
	/// </summary>
	public ref struct ExcuteInstruct sealed
	{
		property int Opcode; //操作码
		property IVector<int>^ Operand; //操作数
	};
	
	/// <summary>
	/// 算法的单个步骤
	/// </summary>
	public ref class SingleStep sealed
	{
	public:
		SingleStep(int type); //构造函数
		
		property int Type; //步骤的类型
		property IVector<IVector<int>^>^ ThisState; //当前步骤的状态
		property IVector<ExcuteInstruct^>^ ToNextStep; //运转到下一步的过程
	};
	
	/// <summary>
	/// 算法执行的步骤
	/// </summary>
	ref class ExecuteStep abstract
	{
	public:
		int CountStep(); //计算有多少步
		int AddStep(SingleStep^ step); //添加步骤
		int RemoveStep(int index); //删除步骤
		void ClearStep(); //清空步骤
		SingleStep^ NavigateToFirst(); //导航到第一步
		SingleStep^ NavigateToLast(); //导航到最后一步

		property IVector<SingleStep^>^ StepList; //步骤列表

	protected private:
		virtual SingleStep^ NavigateToStep(int index) = 0; //导航到指定的步骤
		virtual SingleStep^ NavigateToNext() = 0; //导航到下一步
		virtual SingleStep^ NavigateToPrevious() = 0; //导航到上一步
	};
}
