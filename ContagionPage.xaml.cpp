//
// ContagionPage.xaml.cpp
// ContagionPage 类的实现
//

#include "pch.h"
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
}


void AlgorithmVisualization::ContagionPage::ContagionGrid_SizeChanged(Platform::Object^ sender, Windows::UI::Xaml::SizeChangedEventArgs^ e)
{

}


void AlgorithmVisualization::ContagionPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void AlgorithmVisualization::ContagionPage::DebugBtn_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}
