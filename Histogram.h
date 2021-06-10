#pragma once

#include <collection.h>

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;

namespace AlgorithmVisualization {
	/// <summary>
	/// 柱体的状态
	/// </summary>
	public enum class PillarState
	{
		Default = 0, //默认状态
		Swapping = 1, //正在交换状态
		Compared = 2, //正在比较状态
		Completed = 3, //已完成
		Selected = 4, //被选中
		SetValue = 5, //设置值
	};

	/// <summary>
	/// 柱体类
	/// </summary>
	public ref class Pillar sealed
	{
	public:
		Pillar(int _number, int width, int height, Color color); //构造函数
		Pillar(int _number, int width, int height, Color color, bool temp); //构造函数
		Windows::UI::Xaml::Controls::StackPanel^ getView(); //获取视图
		void resize(int _width, int _height); //重新设置尺寸

		property int number; //代表的数字
		
	private:
		Windows::UI::Xaml::Controls::StackPanel^ outBox;//最外侧的盒子
		Windows::UI::Xaml::Shapes::Rectangle^ rect; //柱体中的矩形
		Windows::UI::Xaml::Controls::TextBlock^ bottomTextBlock; //柱体中的文字
		Windows::UI::Xaml::Controls::TextBlock^ topTextBlock; //柱体中的文字
		
		const int margin = 4; //边界
		const int textHeight = 20; //文本高度
	};

	/// <summary>
	/// 柱状图绘制类
	/// </summary>
	public ref class Histogram sealed
	{
	public:
		Histogram(float _width, float _height);
		void load(IVector<int>^ numbers); //绘制柱状图
		void load(IVector<int>^ numbers, IVector<PillarState>^ states); //绘制柱状图
		void load(IVector<int>^ numbers, IVector<PillarState>^ states, IVector<int>^ isTemp); //绘制柱状图
		void swap(int left, int right); //交换两个柱体的位置
		void swapOnUI(int left, int right); //在UI线程交换两个柱体的位置并设置状态
		void compareOnUI(int left, int right); //在UI线程展示比较两个柱体
		void setColor(int index, Windows::UI::Color color); //设置矩形的颜色
		void setState(int index, PillarState state); //设置柱体的状态
		void setStateOnUI(int index, PillarState state); //在UI线程设置单个柱体的状态
		void setStatesOnUI(IVector<int>^ indexs, IVector<PillarState>^ states); //批量设置柱体状态
		void onSizeChanged(float _width, float _height); //当界面尺寸改变时
		Color stateToColor(PillarState state); //状态转颜色

		property Windows::UI::Xaml::Controls::StackPanel^ container; //用来盛放柱状图的容器

	private:
		int maxNumber = 0;
		Vector<Pillar^>^ pillars; //柱体向量

		float width{1000.0};
		float height{500.0};
		
		Color DefaultColor = Colors::Blue; //默认颜色
		Color SwappingColor = Colors::Red; //正在交换颜色
		Color ComparedColor = Colors::Green; //比较颜色
		Color CompletedColor = Colors::Yellow; //完成颜色
		Color SelectedColor = Colors::Orange; //选中颜色
		Color SetValueColor = Colors::HotPink; //设置值颜色
	};
}