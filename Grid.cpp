#include "pch.h"
#include "Grid.h"

using namespace AlgorithmVisualization;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Shapes;
using namespace Microsoft::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;

/// <summary>
/// 矩形构造函数
/// </summary>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
/// <param name="state">状态</param>
AlgorithmVisualization::Rect::Rect(double width, double height, RectState state)
{
	Container = ref new StackPanel(); //创建堆栈界面
	Container->Children->Clear(); //清空
	Container->Margin = Thickness(Margin); //设置边缘

	CurrentState = state; //保存状态
	InternalRect = ref new Rectangle(); //创建矩形
	InternalRect->Width = width; //设置宽度
	InternalRect->Height = height; //设置高度
	InternalRect->Fill = ref new SolidColorBrush(StateToColor(state)); //设置颜色

	Container->Children->Append(InternalRect); //追加到末尾
}

/// <summary>
/// 设置尺寸
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
void AlgorithmVisualization::Rect::Resize(int width, int height)
{
	InternalRect->Width = width; //设置宽度
	InternalRect->Height = height; //设置高度
}

/// <summary>
/// 设置状态
/// </summary>
/// <param name="newState">新状态</param>
void AlgorithmVisualization::Rect::ChangeState(RectState newState)
{
	CurrentState = newState; //保存状态
	InternalRect->Fill = ref new SolidColorBrush(StateToColor(newState)); //设置颜色
}

/// <summary>
/// 状态转颜色
/// </summary>
/// <param name="state"></param>
/// <returns></returns>
Color AlgorithmVisualization::Rect::StateToColor(RectState state)
{
	//选择颜色
	switch (state)
	{
	case RectState::Susceptible:
		return SusceptibleColor;
	case RectState::Infectious:
		return InfectiousColor;
	case RectState::Recovered:
		return RecoveredColor;
	case RectState::Exposed:
		return ExposedColor;
	case RectState::Asymptomatic:
		return AsymptomaticColor;
	default:
		throw ref new InvalidArgumentException(L"没有匹配的状态");
	}
}

/// <summary>
/// 网格实例化
/// </summary>
/// <param name="_width"></param>
/// <param name="_height"></param>
/// <param name="rows"></param>
/// <param name="cols"></param>
/// <param name="defaultState"></param>
AlgorithmVisualization::Grid::Grid(double _width, double _height, int _rows, int _cols, RectState defaultState)
{
	//记录宽高和行列
	width = _width;
	height = _height;
	rows = _rows;
	cols = _cols;

	double rectWidth = (width - 2.0 * Rect::Margin * cols) / cols - 1; //矩形宽度
	double rectHeight = (height - 2.0 * Rect::Margin * rows) / rows - 1; //矩形高度

	//初始化视图
	ThisView = ref new StackPanel();
	ThisView->Orientation = Orientation::Vertical;
	ThisView->HorizontalAlignment = HorizontalAlignment::Center;
	ThisView->VerticalAlignment = VerticalAlignment::Center;
	ThisView->Children->Clear();

	Data = ref new Vector<IVector<Rect^>^>(); //初始化数据

	for (size_t i = 0; i < rows; i++)
	{
		auto rectRow = ref new Vector<Rect^>(); //矩形行
		auto stackRow = ref new Controls::StackPanel(); //矩形堆栈界面
		stackRow->Orientation = Orientation::Horizontal; //设置方向为横向
		for (size_t j = 0; j < cols; j++)
		{
			auto rect = ref new Rect(rectWidth, rectHeight, defaultState); //单个矩形
			rectRow->Append(rect); //向矩形行追加
			stackRow->Children->Append(rect->Container); //追加进入堆栈视图
		}
		Data->Append(rectRow); //向矩形矩阵追加
		ThisView->Children->Append(stackRow); //追加行进入视图
	}
}

/// <summary>
/// 获取视图
/// </summary>
/// <returns></returns>
Windows::UI::Xaml::Controls::StackPanel^ AlgorithmVisualization::Grid::GetView()
{
	return ThisView;
}

/// <summary>
/// 尺寸改变时回调
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
void AlgorithmVisualization::Grid::OnSizeChanged(double _width, double _height)
{
	width = _width;
	height = _height;
	
	double rectWidth = (width - 2.0 * Rect::Margin * cols) / cols - 1; //矩形宽度
	double rectHeight = (height - 2.0 * Rect::Margin * rows) / rows - 1; //矩形高度

	for (IVector<Rect^>^ i : Data)
	{
		for (Rect^ j : i)
		{
			//重新修改宽高
			j->InternalRect->Width = rectWidth;
			j->InternalRect->Height = rectHeight;
		}
	}
}

/// <summary>
/// 获取矩形
/// </summary>
/// <param name="x">X坐标</param>
/// <param name="y">Y坐标</param>
/// <returns></returns>
Rect^ AlgorithmVisualization::Grid::Get(int x, int y)
{
	return Data->GetAt(y)->GetAt(x);
}

/// <summary>
/// 获取中心矩形
/// </summary>
/// <returns></returns>
Rect^ AlgorithmVisualization::Grid::GetCenter()
{
	return Get(cols / 2, rows / 2);
}

/// <summary>
/// 设置矩形
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="newRect"></param>
void AlgorithmVisualization::Grid::Set(int x, int y, Rect^ newRect)
{
	Data->GetAt(y)->SetAt(x, newRect); //设置矩形
}

/// <summary>
/// 设置矩形状态
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="newState"></param>
void AlgorithmVisualization::Grid::SetState(int x, int y, RectState newState)
{
	Get(x, y)->ChangeState(newState); //修改状态
}
