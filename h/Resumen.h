/**
 * Se implementa una clase descendiente de BufferDinamico para almacenar los resúmenes generados por el Firmador.
 *
 * @author JM
 *
 * @date 26/11/2003 - JM  @li Versión inicial
**/
#ifndef Resumen_H
#define Resumen_H

#include "BufferDinamico.h"
#include "Serializable.h"


class Resumen : public BufferDinamico,
				public Serializable
{
public:
	Resumen();
	virtual ~Resumen();

	virtual ArsCadena serializar();
	virtual bool deserializar(const ArsCadena &serie);
	virtual bool deserializar(const char* serie, DWORD size = 0);
};


#endif
