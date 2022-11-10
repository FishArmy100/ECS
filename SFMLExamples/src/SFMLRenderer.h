#pragma once
#include "Utils/Ref.h"
#include "EntityRegistry.h"
#include "SFML/Graphics.hpp"
#include "Components.h"

namespace Examples
{
	template<typename... TComponents>
	class SFMLRenderer
	{
	private:
		using RegistryRef = typename ECS::Utils::Ref<ECS::EntityRegistry<TComponents...>>;
		using WindowRef = typename ECS::Utils::Ref<sf::RenderWindow>;

	public:
		SFMLRenderer(WindowRef window, RegistryRef registry) : m_WindowRef(window), m_RegistryRef(registry) {}

		void Render(const sf::View& camera)
		{
			m_WindowRef->setView(camera);
			auto entities = m_RegistryRef->GetView<Transform, Renderer>();
			for (auto& [transform, renderer] : entities)
			{
				
			}
		}

	private:
		WindowRef m_WindowRef;
		RegistryRef m_RegistryRef;
	};
}

