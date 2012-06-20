#include "../h/Tarjeta.h"


Tarjeta::Tarjeta(const ArsCadena &_nombre)
{
	nombre = _nombre;
}


Tarjeta::~Tarjeta()
{
}


ArsCadena Tarjeta::getNombre() const
{
	return nombre;
}
