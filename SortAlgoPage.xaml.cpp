//
// SortAlgoPage.xaml.cpp
// SortAlgoPage 类的实现
//

#include "pch.h"
#include "SortAlgoPage.xaml.h"
#include "Histogram.h"

using namespace AlgorithmVisualization;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Shapes;
using namespace Microsoft::UI::Xaml::Media;
using namespace Windows::UI;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

SortAlgoPage::SortAlgoPage()
{
	InitializeComponent();
	histogram = ref new Histogram{}; //初始化容器
	SortHistogram->Children->Append(histogram->container);

	sortVector.clear();
	auto vec = ref new Vector<int>();
	int size = 15;
	for (int i = 0; i < size; ++i)
	{
		int value = rand() % 99 + 1;
		sortVector.push_back(value);
		vec->Append(value);
	}
	histogram->load(vec);
}


void AlgorithmVisualization::SortAlgoPage::SortNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void AlgorithmVisualization::SortAlgoPage::SortNavView_ItemInvoked(Microsoft::UI::Xaml::Controls::NavigationView^ sender, Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{

}


void AlgorithmVisualization::SortAlgoPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto workTask = ref new WorkItemHandler([this](IAsyncAction^ workItem)
		{
			auto n = sortVector.size();
			for (auto i = 0; i < n; i++)
			{
				for (auto j = i + 1; j < n; j++)
				{
					histogram->compareOnUI(i, j);
					Sleep(100);
					if (sortVector.at(i) > sortVector.at(j)) {
						histogram->swapOnUI(i, j);
						int temp = sortVector[i];
						sortVector[i] = sortVector[j];
						sortVector[j] = temp;
						Sleep(100);
					}
					if (i == n - 2 && j == n - 1) histogram->setStateOnUI(n - 1, PillarState::Compared);
					else histogram->setStateOnUI(j, PillarState::Default);
				}
				histogram->setStateOnUI(i, PillarState::Completed);
			}
		});
	auto asyncAction = Windows::System::Threading::ThreadPool::RunAsync(workTask);
}


void AlgorithmVisualization::SortAlgoPage::SortHistogram_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	//Console->Text += "Grid:  " + e->NewSize.Width.ToString() + "   ";
	//Console->Text += e->NewSize.Height.ToString() + "\n";

	histogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height);
}


void AlgorithmVisualization::SortAlgoPage::SortHistogramStack_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	//Console->Text += "Stack:  " + e->NewSize.Width.ToString() + "   ";
	//Console->Text += e->NewSize.Height.ToString() + "\n";
	
	//histogram->onSizeChanged(e->NewSize.Width, e->NewSize.Height);
}
