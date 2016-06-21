#ifndef CORE_GAME_OBJECT_HPP
#define CORE_GAME_OBJECT_HPP

template<class T>
std::weak_ptr<T> GameObject::GetComponent() const
{
	for (auto it : m_Components)
	{
		if (it->IsTypeOf(T::GetType()))
		{
			return std::static_pointer_cast<T>(it);
		}
	}

	return std::weak_ptr<T>();
}

#endif // CORE_GAME_OBJECT_HPP
