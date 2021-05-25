//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "HomePage.xaml.h"
#include "SortAlgoPage.xaml.h"

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

// 主页导航加载完成时回调
void AlgorithmVisualization::MainPage::MainNavView_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	MainNavView->SelectedItem = MainNavView->MenuItems->GetAt(0);
	ContentFrame->Navigate((Type^)HomePage::typeid);
}

// 主页导航的Item被点击时回调
void AlgorithmVisualization::MainPage::MainNavView_ItemInvoked(Windows::UI::Xaml::Controls::NavigationView^ sender, Windows::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs^ args)
{
	if (args->IsSettingsInvoked) //当设置被点击
	{

	}
	else
	{
		auto tag = args->InvokedItemContainer->Tag->ToString();
		Type^ navigateTarget;
		if (tag == "home") {
			navigateTarget = HomePage::typeid;
		}
		else if (tag == "sort")
		{
			navigateTarget = SortAlgoPage::typeid;
		}
		ContentFrame->Navigate(navigateTarget, nullptr, args->RecommendedNavigationTransitionInfo); //导航到相应页面
	}
}
