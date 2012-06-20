/**
 * Se implementa una clase abstracta base para la definici�n de aquellos elementos que pueden ser firmados. El �nico requisito de estos elementos, es que sean representables por un array de bytes, creado en el m�todo readData.
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versi�n inicial
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

	/** Este m�todo debe ser implementado en los descendientes para leer el contenido
	 * de alg�n sitio indicado en "param" (por ejemplo de un archivo o un buffer)
	 * Una vez le�do el contenido, debe ser almacenado a trav�s de setData();
	 * La copia al atributo "data" debe hacerse a trav�s del m�todo "setData".
	**/
	virtual bool readData(void* param = NULL) = 0;
};


#endif
