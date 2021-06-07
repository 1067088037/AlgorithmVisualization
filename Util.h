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
}