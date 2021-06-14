//
// ContagionPage.xaml.cpp
// ContagionPage 类的实现
//

#include "pch.h"
#include "Grid.h"
#include "ContagionPage.xaml.h"

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

// https://go.microsoft.com/fwlink/?LinkId=234238 上介绍了“空白页”项模板

ContagionPage::ContagionPage()
{
	InitializeComponent();
	int type = ((App^)(Application::Current))->sortAlgorithmType; //传染病模型类型

	int rows = 30;
	int cols = 60;
	InfectiousGrid = ref new Grid(lastGridWidth, lastGridWidth / cols * rows, 
		rows, cols, RectState::Susceptible);
	ContagionGrid->Children->Append(InfectiousGrid->GetView());
}


void AlgorithmVisualization::ContagionPage::ContagionGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{
	lastGridWidth = e->NewSize.Width;
	lastGridHeight = e->NewSize.Height;

	InfectiousGrid->OnSizeChanged(lastGridWidth, lastGridWidth / InfectiousGrid->cols * InfectiousGrid->rows);
}


void AlgorithmVisualization::ContagionPage::DebugBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
