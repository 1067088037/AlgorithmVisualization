#include "pch.h"
#include "Histogram.h"

using namespace AlgorithmVisualization;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Shapes;
using namespace Microsoft::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI;

//寻找数组中最大的一项
int findMax(IVector<int>^ vec)
{
	int _max = INT32_MIN;
	for (int i : vec)
	{
		if (i > _max) _max = i;
	}
	return _max;
}

AlgorithmVisualization::Histogram::Histogram(Windows::UI::Xaml::Controls::Panel^ container)
{
	this->container = container;

	int arr[] = { 10,2,3,4,5,6,7,8,9,10 };
	auto vec = ref new Vector<int>(arr, arr + 10);
	load(vec);
}

void AlgorithmVisualization::Histogram::load(IVector<int>^ numbers)
{
	container->Children->Clear();
	int maxNumber = findMax(numbers);
	double margin = 4.0;
	for (int number : numbers)
	{
		auto rect = ref new Rectangle();
		rect->Width = width / numbers->Size - margin * 3;
		rect->Height = height * number / maxNumber;
		rect->Margin = Thickness(margin);
		rect->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom;
		rect->Fill = ref new SolidColorBrush(unselectedColor);
		container->Children->Append(rect);
	}
}

void AlgorithmVisualization::Histogram::swap(int left, int right)
{
	auto children = container->Children;
	auto leftRect = (Rectangle^)children->GetAt(left);
	auto rightRect = (Rectangle^)children->GetAt(right);
	children->RemoveAt(left);
	children->RemoveAt(right - 1);
	children->InsertAt(left, rightRect);
	children->InsertAt(right, leftRect);
}

void AlgorithmVisualization::Histogram::setColor(int index, Windows::UI::Color color)
{
	auto rect = (Rectangle^)container->Children->GetAt(1);
	rect->Fill = ref new SolidColorBrush(color);
}

void AlgorithmVisualization::Histogram::select(int index, bool selected)
{
	Color color;
	if (selected) color = selectedColor;
	else color = unselectedColor;
	auto rect = (Rectangle^)container->Children->GetAt(index);
	rect->Fill = ref new SolidColorBrush(color);
}
