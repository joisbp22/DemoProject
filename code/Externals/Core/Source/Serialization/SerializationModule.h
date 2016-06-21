#ifndef CORE_SERIALIZATION_MODULE_H
#define CORE_SERIALIZATION_MODULE_H

#include "ModuleSystem/ModuleBase.h"

#include "Serialization/Serializable.h"
#include "Serialization/Serializers/SerializerXML.h"
#include "Serialization/Serializers/SerializerClone.h"

class SerializationModule : public ModuleBase
{
	DECLARE_MODULE(SerializationModule, ModuleBase)

public:

					SerializationModule();
	virtual			~SerializationModule() override;

	virtual bool	Initialize() override;
	virtual void	Shutdown() override;

	Factory<Serializable>& GetSerializableFactory();

	void			OnSerializableLoaded(unsigned int instanceID, std::weak_ptr<Serializable> loadedSerializable);
	void			OnSerializableUnloaded(unsigned int instanceID);

	void			AddDependency(unsigned int targetInstanceID, std::weak_ptr<Serializable> dependableObject);

private:

	void			RemoveDependencies(std::weak_ptr<Serializable> dependableObject);

	Factory<Serializable> m_SerializableFactory;

	std::unordered_map<unsigned int, std::weak_ptr<Serializable>> m_LoadedSerializableInstances;
	std::unordered_map<unsigned int, std::vector<std::weak_ptr<Serializable>> > m_WaitingDependencies;
};

#define BEGIN_DESERIALIZE_XML(XMLNode) \
	std::shared_ptr<SerializerXML> serializer = std::make_shared<SerializerXML>(XMLNode); \
	serializer->SetThis(serializer);

#define BEGIN_DESERIALIZE_CLONE(clonedObject) \
	std::shared_ptr<SerializerClone> serializer = std::make_shared<SerializerClone>(clonedObject); \
	serializer->SetThis(serializer);

#define END_DESERIALIZE() serializer.reset();

#define DESERIALIZE_VARIABLE_XML(Variable) std::static_pointer_cast<SerializerXML>(serializer)->DeserializeVariable(#Variable, Variable)

#define DESERIALIZE_VARIABLE_ARRAY_XML(Variable) std::static_pointer_cast<SerializerXML>(serializer)->DeserializeVariableArray(#Variable, Variable)

#define DESERIALIZE_VARIABLE_CLONE(Variable, ClassType) std::static_pointer_cast<SerializerClone>(serializer)->DeserializeVariable(std::static_pointer_cast<const ClassType>(std::static_pointer_cast<SerializerClone>(serializer)->GetCurrentlyClonedObject().lock())->Variable, Variable)

#define DESERIALIZE_VARIABLE_ARRAY_CLONE(Variable, ClassType) std::static_pointer_cast<SerializerClone>(serializer)->DeserializeVariableArray(std::static_pointer_cast<const ClassType>(std::static_pointer_cast<SerializerClone>(serializer)->GetCurrentlyClonedObject().lock())->Variable, Variable)

#define DESERIALIZE_VARIABLE(Variable, ClassType) \
	(serializer->GetType() == SerializerType::eXML) ? \
		DESERIALIZE_VARIABLE_XML(Variable) \
	: (serializer->GetType() == SerializerType::eClone) ? \
		DESERIALIZE_VARIABLE_CLONE(Variable, ClassType) \
	: false

#define DESERIALIZE_VARIABLE_ARRAY(Variable, ClassType) \
	(serializer->GetType() == SerializerType::eXML) ? \
		DESERIALIZE_VARIABLE_ARRAY_XML(Variable) \
	: (serializer->GetType() == SerializerType::eClone) ? \
		DESERIALIZE_VARIABLE_ARRAY_CLONE(Variable, ClassType) \
	: false

#endif // CORE_SERIALIZATION_MODULE_H
