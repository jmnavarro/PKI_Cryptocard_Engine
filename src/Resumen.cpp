//--------------------------------------------------------------------------------------

#include "../h/Resumen.h"
#include "../h/ConversorDatos.h"

//--------------------------------------------------------------------------------------

Resumen::Resumen()
	: BufferDinamico()
{
}

Resumen::~Resumen()
{
}


ArsCadena Resumen::serializar()
{
	return ConversorDatos::ByteToStr(this->getBuffer(), this->getSize());
}


bool Resumen::deserializar(const ArsCadena &serie)
{
	return this->deserializar(serie.cadena(), serie.longitud());
}

bool Resumen::deserializar(const char* serie, DWORD size /*= 0*/)
{
	LPBYTE	datos;

	// si no se tiene la longitud, se puede obtener
	if (size == 0)
		size = strlen(serie);

	size /= 2;

	datos = new BYTE[size];

	ConversorDatos::StrToByte(serie, datos, size);
	this->setBuffer(datos, size);

	delete [] datos;

	return true;
}
