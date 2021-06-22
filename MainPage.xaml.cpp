//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "HomePage.xaml.h"
#include "SortAlgoPage.xaml.h"
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

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// 主页导航加载完成时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="e"></param>
void AlgorithmVisualization::MainPage::MainNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	MainNavView->SelectedItem = MainNavView->MenuItems->GetAt(0);
	ContentFrame->Navigate((Type^)HomePage::typeid);

	//TODO:方便调试
	//MainNavView->SelectedItem = MainNavView->MenuItems->GetAt(1);
	//ContentFrame->Navigate((Type^)SortAlgoPage::typeid);
	//((App^)Application::Current)->sortAlgorithmType = 0;
}

/// <summary>
/// 主页导航的Item被点击时回调
/// </summary>
/// <param name="sender"></param>
/// <param name="args"></param>
void AlgorithmVisualization::MainPage::MainNavView_ItemInvoked(Windows::UI::Xaml::Controls::NavigationView^ sender, Windows::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	((App^)Application::Current)->sortAlgorithmType = -1;
	((App^)Application::Current)->contagionAlgorithmType = -1;

	if (args->IsSettingsInvoked) //当设置被点击
	{

	}
	else
	{
		auto tag = args->InvokedItemContainer->Tag->ToString();
		Type^ navigateTarget = nullptr;
		if (tag == "Home") {
			navigateTarget = HomePage::typeid;
		}
		else if (tag == "PrimarySort")
		{
			navigateTarget = SortAlgoPage::typeid;
			((App^)Application::Current)->sortAlgorithmType = 0;
		}
		else if (tag == "AdvancedComparativeSort")
		{
			navigateTarget = SortAlgoPage::typeid;
			((App^)Application::Current)->sortAlgorithmType = 1;
		}
		else if (tag == "AdvancedNonComparativeSort")
		{
			navigateTarget = SortAlgoPage::typeid;
			((App^)Application::Current)->sortAlgorithmType = 2;
		}
		else if (tag == "SIModel")
		{
			navigateTarget = ContagionPage::typeid;
			((App^)Application::Current)->contagionAlgorithmType = (int)ContagionModelType::SIModel;
		}
		else if (tag == "SISModel")
		{
			navigateTarget = ContagionPage::typeid;
			((App^)Application::Current)->contagionAlgorithmType = (int)ContagionModelType::SISModel;
		}
		else if (tag == "SIRModel")
		{
			navigateTarget = ContagionPage::typeid;
			((App^)Application::Current)->contagionAlgorithmType = (int)ContagionModelType::SIRModel;
		}
		else if (tag == "SEIRModel")
		{
			navigateTarget = ContagionPage::typeid;
			((App^)Application::Current)->contagionAlgorithmType = (int)ContagionModelType::SEIRModel;
		}
		else if (tag == "SEIARModel")
		{
			navigateTarget = ContagionPage::typeid;
			((App^)Application::Current)->contagionAlgorithmType = (int)ContagionModelType::SEIARModel;
		}
		
		if (navigateTarget != nullptr)
			ContentFrame->Navigate(navigateTarget, nullptr, args->RecommendedNavigationTransitionInfo); //导航到相应页面
	}
}
