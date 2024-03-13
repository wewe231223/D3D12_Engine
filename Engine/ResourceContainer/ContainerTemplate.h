#pragma once 

namespace EngineFramework {

	template<typename T>
	concept Is_Vector = requires(T Ty_) {
		typename T::value_type;
		requires std::is_same_v<T, std::vector<typename T::value_type>>;
	};

	/*
	* 이 컨테이너에 대하여... 
	* 
	* 
	* 1. 템플릿 인자는 하나였으면 함.
	* --> 템플릿 인자를 통해 그에 맞는 팩토리도 자동 생성되게 하려면...?
	* ==> 특수화는 어디서?
	* 
	* 특수화는 외부에서 하고 와라.
	* 
	* TMP
	* 
	* A -> B -> C 
	* 
	* template<typename T,typename Ty = DoubleResourceComponent> // 
	* concept derived_from<T,Ty>
	* K = Ty();
	* 
	* ==> Ty 에 들어갈 
	*
	*/

	template<typename T>
	concept Is_Struct = std::is_class<T>::value;

	template<Is_Struct ViewType>
	class View {
	public:
		View(ViewType&& v);
		ViewType m_value{};
	};

	template<Is_Struct ViewType>
	inline View<ViewType>::View(ViewType&& v){
		m_value = v;
	}


	
	template<typename ContainType>
	class Container {
		
	public:
		/// <summary>
		/// Initialize Resource 
		/// </summary>
		/// <typeparam name="...Args">Template Argument's Initialize</typeparam>
		/// <param name="...Arguments"></param>
		template<typename... Args>
		Container(Args...);
		~Container();
	private:
		ContainType* m_data{ nullptr };
	};

	template<typename ContainType>
	template<typename ...Args>
	inline Container<ContainType>::Container(Args ...Arguments){
		m_data = new ContainType{ Arguments };
	}

	template<typename ContainType>
	inline Container<ContainType>::~Container() {
		delete m_data;

	}

}