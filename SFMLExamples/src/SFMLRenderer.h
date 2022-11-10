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
			m_WindowRef->clear();
			auto entities = m_RegistryRef->GetView<Transform, Renderer>();
			for (auto[transform, renderer] : entities)
			{
				switch (renderer->Shape)
				{
				case ShapeType::Circle:
				{
					sf::CircleShape circle{ transform->Scale };
					circle.setFillColor(renderer->Color);
					circle.setPosition(transform->Pos);
					m_WindowRef->draw(circle);
					break;
				}
				case ShapeType::Square:
				{
					sf::RectangleShape square{ {transform->Scale, transform->Scale} };
					square.setFillColor(renderer->Color);
					square.setPosition(transform->Pos);
					m_WindowRef->draw(square);
					break;
				}
				default:
					break;
				}
			}
			m_WindowRef->display();
		}

	private:
		WindowRef m_WindowRef;
		RegistryRef m_RegistryRef;
	};
}
