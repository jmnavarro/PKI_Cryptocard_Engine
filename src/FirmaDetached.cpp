//--------------------------------------------------------------------------------------

#include "../h/FirmaDetached.h"
#include "../h/ConversorDatos.h"

//--------------------------------------------------------------------------------------


FirmaDetached::FirmaDetached()
	: Firma()
{
}

FirmaDetached::FirmaDetached(const FirmaDetached &copy)
	: Firma(copy)
{
}

FirmaDetached::FirmaDetached(const void* buff, DWORD size)
	: Firma(buff, size)
{
}


FirmaDetached::~FirmaDetached()
{
}



ArsCadena FirmaDetached::serializar()
{
	return ConversorDatos::ByteToStr(this->getBuffer(), this->getSize());
}


bool FirmaDetached::deserializar(const ArsCadena &serie)
{
	return this->deserializar(serie.cadena(), serie.longitud());
}


bool FirmaDetached::deserializar(const char* serie, DWORD size /*= 0*/)
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
