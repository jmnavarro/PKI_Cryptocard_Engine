/**
 * Se implementa una clase abstracta base para la definición de aquellos elementos que pueden ser firmados. El único requisito de estos elementos, es que sean representables por un array de bytes, creado en el método readData.
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versión inicial
**/
#ifndef Firmable_H
#define Firmable_H

#include "BufferDinamico.h"
//--------------------------------------------------------------------------------------

class Firmable : public BufferDinamico
{
public:
	Firmable();
	virtual ~Firmable();

	/** Este método debe ser implementado en los descendientes para leer el contenido
	 * de algún sitio indicado en "param" (por ejemplo de un archivo o un buffer)
	 * Una vez leído el contenido, debe ser almacenado a través de setData();
	 * La copia al atributo "data" debe hacerse a través del método "setData".
	**/
	virtual bool readData(void* param = NULL) = 0;
};


#endif
