/**
 * Se implementa la clase "Firma", encargada de realizar la firma digital sobre un buffer de bytes.
 *
 * @author JM
 *
 * @date 26/01/2004 - JM  @li Versión inicial
**/
#ifndef FirmaDetached_H
#define FirmaDetached_H

#include "Firma.h"


class FirmaDetached : public Firma
{
public:
	FirmaDetached();
	FirmaDetached(const FirmaDetached &copy);
	FirmaDetached(const void* buff, DWORD size);
	virtual ~FirmaDetached();

	// implementación de ISerializable
	ArsCadena serializar();
	bool deserializar(const ArsCadena &serie);
	bool deserializar(const char* serie, DWORD size = 0);
};


#endif
