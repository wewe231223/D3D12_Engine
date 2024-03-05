#pragma once 
#include "ResourceFactory.h"
#include <functional>

namespace EngineFramework {
	namespace Resource {
		template<typename FactoryType>
		concept Is_Compatible_Factory = std::derived_from<FactoryType,ResourceFactory>;
		
		template<typename ContainType,typename FactoryType>
		class ResourceContainer{
		public:
			ResourceContainer(const ICommandList* pCommandList,ResourceParameter InitializeParameter) requires Is_Compatible_Factory<FactoryType>;
			~ResourceContainer();
		private:
			ContainType m_value{};
			std::unique_ptr<FactoryType> m_factory{ nullptr };
		public:
			ContainType GetValue() const noexcept;
		};

		template<typename ContainType,typename FactoryType>
		inline ResourceContainer<ContainType,FactoryType>::ResourceContainer(const ICommandList* pCommandList,ResourceParameter InitializeParameter) requires Is_Compatible_Factory<FactoryType>{
			m_factory = std::make_unique<FactoryType>();
			ResourceContainTypes Product = m_factory->Create(pCommandList,InitializeParameter);

			if (!std::holds_alternative<ContainType>(Product)) {
				throw System::Exeption(_T("[ResourceContainer] : Factory Product doesn't Match for ContainType\n"));
			}

			m_value = std::get<ContainType>(Product);
		}

		template<typename ContainType, typename FactoryType>
		inline ResourceContainer<ContainType, FactoryType>::~ResourceContainer(){

		}

		template<typename ContainType,typename FactoryType>
		inline ContainType ResourceContainer<ContainType,FactoryType>::GetValue() const noexcept {
			return m_value;
		}

		using Mesh_ = ResourceContainer<MeshValue, Resource::MeshFactory>;
	}
}