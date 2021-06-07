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
using namespace Windows::ApplicationModel::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;

/// <summary>
/// 获取向量中最大的一项
/// </summary>
/// <param name="vec">要查找的向量</param>
/// <returns></returns>
int findMax(IVector<int>^ vec)
{
	int _max = INT32_MIN;
	for (int i : vec)
	{
		if (i > _max) _max = i;
	}
	return _max;
}

/// <summary>
/// 构造函数
/// </summary>
AlgorithmVisualization::Histogram::Histogram()
{
	this->container = ref new StackPanel(); //创建堆栈面板
	this->container->Orientation = Windows::UI::Xaml::Controls::Orientation::Horizontal; //设置堆栈面板为水平方向
	pillars = ref new Vector<Pillar^>(); //清空柱体向量
}

/// <summary>
/// 加载柱状图的每一条柱体
/// </summary>
/// <param name="numbers">要展示的数字向量</param> 
void AlgorithmVisualization::Histogram::load(IVector<int>^ numbers)
{
	load(numbers, ref new Vector<PillarState>(numbers->Size, PillarState::Default));
}

void AlgorithmVisualization::Histogram::load(IVector<int>^ numbers, IVector<PillarState>^ states)
{
	container->Children->Clear(); //先清空原有的容器
	pillars->Clear();
	float64 margin = 4.0; //边界大小
	int _maxNumber = findMax(numbers); //找到最高的，以它为基准作图
	maxNumber = _maxNumber;
	int pillarWidth = (int)(width / numbers->Size); //计算宽度
	for (int i = 0; i < numbers->Size; ++i)
	{
		int number = numbers->GetAt(i);
		auto rect = ref new Rectangle(); //创建矩形
		int pillarHeight = (int)(height * number / maxNumber); //计算高度
		auto pillar = ref new Pillar{ number, pillarWidth, pillarHeight, stateToColor(states->GetAt(i)) }; //实例化柱体，设置合理的高度和宽度
		container->Children->Append(pillar->getView()); //追加进容器
		pillars->Append(pillar); //追加到向量末尾
	}
}

/// <summary>
/// 交换两个柱体
/// </summary>
/// <param name="left">左边要交换的</param>
/// <param name="right">右边要交换的</param>
void AlgorithmVisualization::Histogram::swap(int left, int right)
{
	if (right < left) swap(left, right); //如果左右颠倒则保证左小右大
	if (left == right) throw ref new InvalidArgumentException(L"传入的参数必须是两个不同的位置"); //如果二者标号一样则爆出异常
	auto children = container->Children;
	auto leftRect = (StackPanel^)children->GetAt(left); //获取左侧元素
	auto rightRect = (StackPanel^)children->GetAt(right); //获取右侧元素
	children->RemoveAt(right); //删除右侧元素
	children->RemoveAt(left); //删除左侧元素
	children->InsertAt(left, rightRect); //在左侧的位置插入右侧元素
	children->InsertAt(right, leftRect); //在右侧的位置插入左侧元素
	setState(left, PillarState::Swapping);
	setState(right, PillarState::Swapping);
}

/// <summary>
/// 在UI线程交换
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void AlgorithmVisualization::Histogram::swapOnUI(int left, int right)
{
	CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, left, right]()
		{
			swap(left, right);
		}
	));
}

/// <summary>
/// 在UI线程显示比较
/// </summary>
/// <param name="left"></param>
/// <param name="right"></param>
void AlgorithmVisualization::Histogram::compareOnUI(int left, int right)
{
	CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, left, right]()
		{
			setState(left, PillarState::Compared);
			setState(right, PillarState::Compared);
		}
	));
}

/// <summary>
/// 设置柱体的颜色
/// </summary>
/// <param name="index">要设置的索引</param>
/// <param name="color">要设置的颜色</param>
void AlgorithmVisualization::Histogram::setColor(int index, Windows::UI::Color color)
{
	auto rect = (StackPanel^)container->Children->GetAt(index); //获取对应编号的元素
	((Rectangle^)(rect->Children->GetAt(0)))->Fill = ref new SolidColorBrush(color); //对该位置的元素设置颜色
}

/// <summary>
/// 设置柱体的状态
/// </summary>
/// <param name="index">索引</param>
/// <param name="state">状态</param>
void AlgorithmVisualization::Histogram::setState(int index, PillarState state)
{
	Color color = stateToColor(state);
	setColor(index, color);
}

/// <summary>
/// 在UI线程设置状态
/// </summary>
/// <param name="index"></param>
/// <param name="state"></param>
void AlgorithmVisualization::Histogram::setStateOnUI(int index, PillarState state)
{
	CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, index, state]()
		{
			setState(index, state);
		}
	));
}

/// <summary>
/// 在UI线程设置多个状态
/// </summary>
/// <param name="indexs"></param>
/// <param name="states"></param>
void AlgorithmVisualization::Histogram::setStatesOnUI(IVector<int>^ indexs, IVector<PillarState>^ states)
{
	CoreApplication::MainView->CoreWindow->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, indexs, states]()
		{
			for (unsigned int i = 0; i < indexs->Size; i++)
			{
				setState(indexs->GetAt(i), states->GetAt(i));
			}
		}
	));
}

void AlgorithmVisualization::Histogram::onSizeChanged(float _width, float _height)
{
	width = _width;
	//height = _height;
	float64 margin = 4.0; //边界大小
	int pillarWidth = (int)(width / pillars->Size); //计算宽度
	for (Pillar^ i : pillars)
	{
		int pillarHeight = (int)(height * i->number / maxNumber); //计算高度
		i->resize(pillarWidth, pillarHeight);
	}
}

Color AlgorithmVisualization::Histogram::stateToColor(PillarState state)
{
	Color color;
	switch (state)
	{
	case PillarState::Default:
		color = DefaultColor;
		break;
	case PillarState::Compared:
		color = ComparedColor;
		break;
	case PillarState::Swapping:
		color = SwappingColor;
		break;
	case PillarState::Completed:
		color = CompletedColor;
		break;
	default:
		break;
	}
	return color;
}

/// <summary>
/// 构造函数
/// </summary>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
/// <param name="color">颜色</param>
/// <param name="text">文字</param>
AlgorithmVisualization::Pillar::Pillar(int _number, int width, int height, Color color)
{
	number = _number;
	
	outBox = ref new StackPanel(); //创建外侧盒子
	outBox->Orientation = Orientation::Vertical;
	outBox->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom; //设置对齐模式

	rect = ref new Rectangle(); //创建矩形
	rect->Width = width - margin * 2; //设置宽度
	rect->Height = height; //设置高度
	rect->Margin = Thickness(margin); //设置边界
	rect->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Bottom; //设置对齐方向为向底部对齐
	rect->Fill = ref new SolidColorBrush(color); //上色

	textBlock = ref new TextBlock(); //创建文本
	textBlock->Width = width - margin * 2; //设置宽度
	textBlock->Height = textHeight; //设置高度
	textBlock->Text = number.ToString(); //写入文本
	textBlock->HorizontalAlignment = HorizontalAlignment::Center; //设置对齐
	textBlock->TextAlignment = TextAlignment::Center; //设置文本对齐

	outBox->Children->Append(rect); //向堆栈面板中追加矩形
	outBox->Children->Append(textBlock); //向堆栈面板中追加文字
}

/// <summary>
/// 获取视图
/// </summary>
/// <returns></returns>
Windows::UI::Xaml::Controls::StackPanel^ AlgorithmVisualization::Pillar::getView()
{
	return outBox;
}

void AlgorithmVisualization::Pillar::resize(int _width, int _height)
{
	rect->Width = _width - margin * 2; //设置宽度
	rect->Height = _height; //设置高度
	textBlock->Width = _width - margin * 2; //设置宽度
}
