#include "Color.h"

#include "Serialization/SerializationModule.h"

Color::Color()
	: ISerializable()
	, m_R(0)
	, m_G(0)
	, m_B(0)
{

}

Color::Color(unsigned int r, unsigned int g, unsigned int b)
	: ISerializable()
	, m_R(r)
	, m_G(g)
	, m_B(b)
{
}

Color::~Color()
{

}

bool Color::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_R, Color);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_G, Color);
	assert(success);
	success &= DESERIALIZE_VARIABLE(m_B, Color);
	assert(success);

	return success;
}

unsigned int Color::GetPackedValue() const
{
	return m_R + (m_G << 8) + (m_B << 16);;
}

unsigned int Color::GetR() const
{
	return m_R;
}

unsigned int Color::GetG() const
{
	return m_G;
}

unsigned int Color::GetB() const
{
	return m_B;
}
