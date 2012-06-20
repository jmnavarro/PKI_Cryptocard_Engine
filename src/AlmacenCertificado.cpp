#include "../h/AlmacenCertificado.h"


AlmacenCertificado::AlmacenCertificado(const ArsCadena &nombre)
{
	setNombre(nombre);
}

AlmacenCertificado::~AlmacenCertificado()
{
}

void AlmacenCertificado::setNombre(const ArsCadena &nombre)
{
	this->nombre = nombre;
}

ArsCadena& AlmacenCertificado::getNombre()
{
	return nombre;
}

