//+++===================================================================================
// BufferDinamico.
//
// Descripcion:
//		Se implementa una clase que representa un buffer de memoria din�mica, que en
//		realidad no es m�s que un arrays de bytes junto con su tama�o.
//		Encapsulando este concepto, conseguimos mayor seguridad de la memoria que
//		utiliza el array.
//		Adem�s, a la hora de pasar un array de bytes como par�metro, podemos pasar un
//		�nico objeto de este tipo, y no dos par�metro (el array y su tama�o)
//
// Autor   Fecha       Observaciones
// ------  ----------  -----------------------------------------------------------------
// JM      21/11/2003  Versi�n inicial
//
//======================================================================================
#ifndef BufferDinamico_H
#define BufferDinamico_H

#include <windows.h>
//--------------------------------------------------------------------------------------

/*
typedef unsigned char	BYTE;
typedef BYTE			*LPBYTE;
typedef const LPBYTE	LPCBYTE;

typedef unsigned long	DWORD;
*/

#ifndef _LPCBYTE_DEFINED
#define _LPCBYTE_DEFINED
	typedef const BYTE*  LPCBYTE;
#endif
//--------------------------------------------------------------------------------------

class BufferDinamico
{
public:
	BufferDinamico();
	BufferDinamico(const BufferDinamico &copy);	// copy constructor
	BufferDinamico(const void* buff, DWORD size);
	virtual ~BufferDinamico();

	LPCBYTE getBuffer() const;
	void setBuffer(LPCBYTE newBuffer, DWORD newSize);
	void setBuffer(const BufferDinamico &copy);

	DWORD getSize() const;
	void setSize(DWORD newSize);
	void setSizeData(DWORD newSize);

	void freeBuffer();

	bool equals(const BufferDinamico &dataComparacion) const;

	// operadores
	const BufferDinamico& operator = (const BufferDinamico &newData);
	bool operator == (const BufferDinamico &dataComparacion) const;
	bool operator != (const BufferDinamico &dataComparacion) const;

protected:
	// para manejar los atributos internos desde los descendientes
	DWORD getSizeBuffer() const;

private:
	LPBYTE 	buffer;
	DWORD	sizeBuffer;
	DWORD	sizeData;
};


#endif

