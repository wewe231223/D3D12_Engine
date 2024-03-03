#pragma once 
#include "ResourceFactory.h"

namespace EngineFramework {
	namespace Resource {
		template<typename ContainType,typename FactoryType>
		class ResourceContainer {
		public:
			ResourceContainer(const ICommandList* pCommandList,ResourceParameter InitializeParameter);
		private:
			ContainType m_value;
			ResourceFactory* m_factory{};
		public:
			ContainType GetValue() const noexcept;
		};

		template<typename ContainType,typename FactoryType>
		inline ResourceContainer<ContainType,FactoryType>::ResourceContainer(const ICommandList* pCommandList,ResourceParameter InitializeParameter){
			m_factory = new FactoryType(); // 이건 나중에 바꿔야함 
			ResourceContainTypes Product = m_factory->Create(pCommandList,InitializeParameter);
			
			if (!std::holds_alternative<ContainType>(Product)) {
				//Exeption!
			}
		}

		template<typename ContainType,typename FactoryType>
		inline ContainType ResourceContainer<ContainType,FactoryType>::GetValue() const noexcept {
			return m_value;
		}
	}
}