#pragma once

#include <collection.h>

using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;

namespace AlgorithmVisualization {
	/// <summary>
	/// 柱状图绘制类
	/// </summary>
	public ref class Histogram sealed
	{
	public:
		Histogram(Windows::UI::Xaml::Controls::Panel^ container);
		void load(IVector<int>^ numbers); //绘制柱状图
		void swap(int left, int right); //交换两个柱体的位置
		void setColor(int index, Windows::UI::Color color); //设置矩形的颜色
		void select(int index, bool selected);
	
	private:
		Windows::UI::Xaml::Controls::Panel^ container; //用来盛放柱状图的容器
		double width = 1000.0;
		double height = 500.0;
		Color selectedColor = Windows::UI::Colors::OrangeRed;
		Color unselectedColor = Windows::UI::Colors::Blue;
	};
}