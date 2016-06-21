#ifndef CORE_COLOR_H
#define CORE_COLOR_H

#include "Serialization/ISerializable.h"

class Color : public ISerializable
{
public:
	Color();
	Color(unsigned int r, unsigned int g, unsigned int b);
	virtual ~Color() override;

	virtual bool Deserialize(std::shared_ptr<SerializerBase> serializer) override;

	unsigned int GetPackedValue() const;

	unsigned int GetR() const;
	unsigned int GetG() const;
	unsigned int GetB() const;

private:
	unsigned int m_R;
	unsigned int m_G;
	unsigned int m_B;
};

#endif // CORE_COLOR_H
