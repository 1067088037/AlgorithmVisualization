//
// ContagionPage.xaml.h
// ContagionPage 类的声明
//

#pragma once

#include <vector>
#include <random>
#include "Grid.h"
#include "ContagionPage.g.h"

namespace AlgorithmVisualization
{
	/// <summary>
	/// 点
	/// </summary>
	public ref struct Point sealed
	{
		Point(int _x, int _y)
		{
			x = _x;
			y = _y;
		}

		property int x;
		property int y;
	};

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

	protected:
		virtual void OnNavigatedFrom(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	private:
		void ContagionGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e);
		void DebugBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void InitAlgorithm(ContagionModelType type); //初始化算法
		void GetThisState(); //获取当前步骤
		void LoadNextState(); //加载下一个状态
		void Next_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void TimeElapseSpeedSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void ContactPeopleCountSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void InfectiousRateSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void StartOrPause_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void RecoveryRateSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void Reset_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void ExposedToInfectiousSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void ExposedToAsymptomaticSlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);
		void AsymptomaticRecoverySlider_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e);

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

		RectState GetState(int x, int y, bool next = false); //获取指定坐标的状态
		void SetState(int x, int y, RectState newState, bool next = true); //设置状态
		void ChangeNear(int srcX, int srcY, int nearCount, double probability, RectState from, RectState to); //感染邻近的人
		void InfectNear(int srcX, int srcY, int nearCount, double probability); //感染附近
		void ChangeToExposed(int srcX, int srcY, int nearCount, double probability); //变为潜伏者
		bool CheckPointValid(int x, int y); //检查点的合法性

		bool IsTimerRunning(); //计时器是否在运行
		void StartTimer(); //启动计时器
		void StopTimer(); //停止计时器

		ContagionModelType currentType;
		double lastGridWidth = 800.0; //上次柱状图宽度
		double lastGridHeight = 400.0; //上次柱状图高度
		int64 Speed = 200; //可视化速度
		int ContactPeopleCount = 20; //接触到的人数

		double infectiousRate = 0.1; //感染率
		property double InfectiousRate //感染率
		{
			double get()
			{
				return infectiousRate;
			}

			void set(double value)
			{
				infectiousRate = value;
				if (InfectiousRateText != nullptr)
				{
					InfectiousRateText->Text = (int)(100 * value) + L"%";
					InfectiousRateSlider->Value = 100 * value;
				}
			}
		}

		double recoveryRate = 0.1;
		property double RecoveryRate //康复率
		{
			double get()
			{
				return recoveryRate;
			}

			void set(double value)
			{
				recoveryRate = value;
				if (RecoveryRateText != nullptr)
				{
					RecoveryRateText->Text = (int)(100 * value) + L"%";
					RecoveryRateSlider->Value = 100 * value;
				}
			}
		}

		double exposedToInfectiousRate = 0.1;
		property double ExposedToInfectiousRate //潜伏者转显性感染者概率
		{
			double get()
			{
				return exposedToInfectiousRate;
			}

			void set(double value)
			{
				exposedToInfectiousRate = value;
				if (ExposedToInfectiousText != nullptr)
					ExposedToInfectiousText->Text = (int)(100 * value) + L"%";
			}
		}

		double exposedToAsymptomaticRate = 0.1; //潜伏者转隐形感染者概率
		property double ExposedToAsymptomaticRate //潜伏者转隐形感染者概率
		{
			double get()
			{
				return exposedToAsymptomaticRate;
			}

			void set(double value)
			{
				exposedToAsymptomaticRate = value;
				if (ExposedToAsymptomaticText != nullptr)
					ExposedToAsymptomaticText->Text = (int)(100 * value) + L"%";
			}
		}

		double AsymptomaticRecoveryRate = 0.1; //隐性感染者康复率

		Windows::System::Threading::ThreadPoolTimer^ ThreadTimer; //计时器线程
		Grid^ InfectiousGrid; //传染病网格
		property int xMax //最大x坐标
		{
			int get()
			{
				return InfectiousGrid->cols;
			}
		}
		property int yMax //最大y坐标
		{
			int get()
			{
				return InfectiousGrid->rows;
			}
		}

		int DirectionX[8] = { -1, 0, 1, 1, 1, 0, -1, -1 }; //X的方向
		int DirectionY[8] = { 1, 1, 1, 0, -1, -1, -1, 0 }; //Y的方向

		NavToNextStepFun NavToNextStep; //导航到下一步
		std::vector<std::vector<RectState>> ThisStateVector; //当前状态向量
		std::vector<std::vector<RectState>> NextStateVector; //下一个状态向量

		std::default_random_engine e{ GetTickCount() }; //随机数引擎
		std::uniform_real_distribution<double> random{ 0, 1 };
	};
}
