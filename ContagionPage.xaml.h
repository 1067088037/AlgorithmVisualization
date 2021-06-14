//
// ContagionPage.xaml.h
// ContagionPage 类的声明
//

#pragma once

#include "Grid.h"
#include "ContagionPage.g.h"

namespace AlgorithmVisualization
{
	/// <summary>
	/// 传染病类型
	/// </summary>
	public enum class ContagionModelType
	{
		SIModel = 0,
		SISModel = 1,
		SIRModel = 2,
		SEIRModel = 3,
		SEIARModel = 4
	};
	
	/// <summary>
	/// 可用于自身或导航至 Frame 内部的空白页。
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ContagionPage sealed
	{
	public:
		typedef void (ContagionPage::* NavToNextStepFun)(void);
		
		ContagionPage();
	
	private:
		void ContagionGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void DebugBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void InitAlgorithm(ContagionModelType type); //初始化算法
		void Start_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Pause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		void InitSIModel(); //初始化SI模型
		void InitSISModel(); //初始化SIS模型
		void InitSIRModel(); //初始化SIR模型
		void InitSEIRModel(); //初始化SEIR模型
		void InitSEIARModel(); //初始化SEIAR模型

		void SINextStep(); //SI下一步
		void SISNextStep(); //SIS下一步
		void SIRNextStep(); //SIR下一步
		void SEIRNextStep(); //SEIR下一步
		void SEIARNextStep(); //SEIAR下一步

		bool IsTimerRunning(); //计时器是否在运行
		void StopTimer(); //停止计时器
		
		double lastGridWidth = 800.0; //上次柱状图宽度
		double lastGridHeight = 400.0; //上次柱状图高度
		int64 Speed = 100; //可视化速度

		Windows::System::Threading::ThreadPoolTimer^ ThreadTimer; //计时器线程
		Grid^ InfectiousGrid; //传染病网格

		NavToNextStepFun NavToNextStep;
	};
}
