#include "Serializable.h"

#include "SerializationModule.h"

IMPLEMENT_FACTORY_INTERFACE(Serializable)

Serializable::Serializable()
{
}

Serializable::~Serializable()
{
	std::shared_ptr<SerializationModule> serializationModule = GET_MODULE(SerializationModule).lock();
	if (serializationModule != nullptr)
	{
		serializationModule->OnSerializableUnloaded(m_InstanceID);
	}
}

bool Serializable::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	return true;
}

void Serializable::ResolveLink(unsigned int targetInstanceID, std::weak_ptr<Serializable> targetInstance)
{
	std::unordered_map<unsigned int, std::weak_ptr<Serializable>*>::iterator it = m_Dependencies.find(targetInstanceID);
	assert(it != m_Dependencies.end());
	if (it != m_Dependencies.end())
	{
		*(it->second) = targetInstance;

		m_Dependencies.erase(targetInstanceID);
	}
}
