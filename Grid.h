#pragma once

using namespace Platform;
using namespace AlgorithmVisualization;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;

namespace AlgorithmVisualization {
	/// <summary>
	/// 矩形状态
	/// </summary>
	public enum class RectState
	{
		Susceptible = 0, //易感者
		Infectious = 1, //感染者
		Recovered = 2, //康复者
		Exposed = 3, //潜伏者
		Asymptomatic = 4, //无症状感染者
	};

	/// <summary>
	/// 矩形类
	/// </summary>
	public ref class Rect sealed
	{
	public:
		Rect(double width, double height, RectState state); //构造函数

		void Resize(int width, int height); //修改尺寸
		void ChangeState(RectState newState); //修改状态
		Color StateToColor(RectState state); //状态转颜色

		property Controls::StackPanel^ Container; //容器
		property Shapes::Rectangle^ InternalRect; //内部的矩形
		property RectState CurrentState; //当前状态
		
		static property int Margin
		{
			int get()
			{
				return 1;
			}
		}
	
	private:
		Color SusceptibleColor = Colors::Blue; //易感者颜色
		Color InfectiousColor = Colors::Red; //感染者颜色
		Color RecoveredColor = Colors::Green; //康复者颜色
		Color ExposedColor = Colors::Orange; //潜伏者颜色
		Color AsymptomaticColor = Colors::Purple; //无症状感染者颜色
	};
	
	/// <summary>
	/// 网格图
	/// </summary>
	public ref class Grid sealed
	{
	public:
		Grid(double _width, double _height, int _rows, int _cols, RectState defaultState); //构造函数
		Windows::UI::Xaml::Controls::StackPanel^ GetView(); //获取视图

		void OnSizeChanged(double _width, double _height); //尺寸改变时回调
		Rect^ Get(int x, int y); //获取矩形
		void Set(int x, int y, Rect^ newRect); //设置矩形
		void SetState(int x, int y, RectState newState); //设置矩形状态

		property int rows; //行数
		property int cols; //列数

	private:
		Windows::UI::Xaml::Controls::StackPanel^ ThisView; //视图
		IVector<IVector<Rect^>^>^ Data; //网格的数据

		double width{ 800.0 }; //宽度
		double height{ 800.0 }; //高度
		
	};
}