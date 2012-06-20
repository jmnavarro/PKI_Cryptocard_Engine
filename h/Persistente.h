/**
 * Se define una interfaz que representa a todos aquellos elementos que tienen la
 * capacidad de almacenarse en un archivo de disco.
 * En C++ no existe el concepto de interfaz, así que se ha codificado como una clase
 * abstracta pura.
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versión inicial
**/
#ifndef Persistente_H
#define Persistente_H

#include <windows.h>
//--------------------------------------------------------------------------------------

#define ERROR_SAVE_FILE	(DWORD) 0xFFFFFFFF
#define ERROR_LOAD_FILE (DWORD) 0xFFFFFFFF


class Persistente
{
public:
	// deben retornar el número de bytes guardados /leídos del fichero, o los valores
	// ERROR_SAVE_FILE o ERROR_LOAD_FILE si se ha producido un error.
	virtual DWORD saveToFile(const char* filename) = 0;
	virtual DWORD loadFromFile(const char* filename) = 0;
};


#endif
