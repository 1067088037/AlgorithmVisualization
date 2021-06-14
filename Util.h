#pragma once

using namespace Windows::Foundation::Collections;
using namespace Platform;
using namespace Platform::Collections;
using namespace AlgorithmVisualization;

namespace AlgorithmVisualization {
	/// <summary>
	/// 工具类
	/// </summary>
	public ref class Util sealed
	{
	internal:
		template<class T>
		static IVector<T>^ CopyVector(IVector<T>^ from);

		template<class T>
		static IVector<IVector<T>^>^ Copy2DVector(IVector<IVector<T>^>^ from);
	};
	
	template<class T>
	inline IVector<T>^ Util::CopyVector(IVector<T>^ from)
	{
		auto To = ref new Vector<T>();
		for (auto i : from)
		{
			To->Append(i);
		}
		return To;
	}
	template<class T>
	inline IVector<IVector<T>^>^ Util::Copy2DVector(IVector<IVector<T>^>^ from)
	{
		auto To = ref new Vector<IVector<T>^>();
		for (IVector<T>^ i : from)
		{
			auto ToRow = ref new Vector<T>();
			for (T j : i)
			{
				ToRow->Append(j);
			}
			To->Append(ToRow);
		}
		return To;
	}
}