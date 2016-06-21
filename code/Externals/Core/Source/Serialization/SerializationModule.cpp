#include "SerializationModule.h"


IMPLEMENT_MODULE(SerializationModule)


SerializationModule::SerializationModule()
	: ModuleBase()
{
}

SerializationModule::~SerializationModule()
{

}

bool SerializationModule::Initialize()
{
	bool success = true;

	return success;
}

void SerializationModule::Shutdown()
{

}

Factory<Serializable>& SerializationModule::GetSerializableFactory()
{
	return m_SerializableFactory;
}

void SerializationModule::OnSerializableLoaded(unsigned int instanceID, std::weak_ptr<Serializable> loadedSerializable)
{
	std::unordered_map<unsigned int, std::vector<std::weak_ptr<Serializable>> >::iterator itWaitingDependencies = m_WaitingDependencies.find(instanceID);
	if (itWaitingDependencies != m_WaitingDependencies.end())
	{
		for (auto dependableObject : itWaitingDependencies->second)
		{
			std::shared_ptr<Serializable> dependendableObjectShared = dependableObject.lock();
			if (dependendableObjectShared != nullptr)
			{
				dependendableObjectShared->ResolveLink(instanceID, loadedSerializable);
			}
		}
		m_WaitingDependencies.erase(instanceID);
	}

	std::unordered_map<unsigned int, std::weak_ptr<Serializable>>::const_iterator itLoadedInstances = m_LoadedSerializableInstances.find(instanceID);
	assert(itLoadedInstances == m_LoadedSerializableInstances.end());
	if (itLoadedInstances == m_LoadedSerializableInstances.end())
	{
		m_LoadedSerializableInstances.insert(std::pair<unsigned int, std::weak_ptr<Serializable>>(instanceID, loadedSerializable));
	}
}

void SerializationModule::OnSerializableUnloaded(unsigned int instanceID)
{
	std::unordered_map<unsigned int, std::weak_ptr<Serializable>>::iterator it = m_LoadedSerializableInstances.find(instanceID);
	assert(it != m_LoadedSerializableInstances.end());
	if (it != m_LoadedSerializableInstances.end())
	{
		RemoveDependencies(it->second);
		m_LoadedSerializableInstances.erase(instanceID);
	}
}

void SerializationModule::AddDependency(unsigned int targetInstanceID, std::weak_ptr<Serializable> dependableObject)
{
	std::unordered_map<unsigned int, std::weak_ptr<Serializable>>::const_iterator it = m_LoadedSerializableInstances.find(targetInstanceID);
	if (it != m_LoadedSerializableInstances.end())
	{
		std::shared_ptr<Serializable> dependendableObjectShared = dependableObject.lock();
		if (dependendableObjectShared != nullptr)
		{
			dependendableObjectShared->ResolveLink(it->first, it->second);
		}
	}
	else
	{
		m_WaitingDependencies[targetInstanceID].push_back(dependableObject);
	}
}

void SerializationModule::RemoveDependencies(std::weak_ptr<Serializable> dependableObject)
{
	std::unordered_map<unsigned int, std::vector<std::weak_ptr<Serializable>> >::iterator itWaitingDependencies = m_WaitingDependencies.begin();
	for (; itWaitingDependencies != m_WaitingDependencies.end(); )
	{
		std::vector<std::weak_ptr<Serializable>>::iterator itWaitingDependenciesVector = itWaitingDependencies->second.begin();
		for (; itWaitingDependenciesVector != itWaitingDependencies->second.end();)
		{
			std::shared_ptr<Serializable> dependendableObjectShared = itWaitingDependenciesVector->lock();
			if (dependendableObjectShared == dependableObject.lock())
			{
				itWaitingDependenciesVector = itWaitingDependencies->second.erase(itWaitingDependenciesVector);
			}
			else
			{
				++itWaitingDependenciesVector;
			}
		}

		if (itWaitingDependencies->second.empty())
		{
			itWaitingDependencies = m_WaitingDependencies.erase(itWaitingDependencies);
		}
		else
		{
			++itWaitingDependencies;
		}
	}
}
