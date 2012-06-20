/**
 * Se define una interfaz que representa a todos aquellos elementos que tienen la
 * capacidad de serializarse, es decir: de representarse por caracteres alfanuméricos
 * En C++ no existe el concepto de interfaz, así que se ha codificado como una clase
 * abstracta pura.
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versión inicial
**/
#ifndef Serializable_H
#define Serializable_H

#include <pshpack8.h>

#include "../../LibUtils/h/ArsCadena.h"
//--------------------------------------------------------------------------------------


class Serializable
{

public:
	virtual ArsCadena serializar() = 0;
	virtual bool deserializar(const ArsCadena &serie) = 0;
};


#include <poppack.h>
#endif
