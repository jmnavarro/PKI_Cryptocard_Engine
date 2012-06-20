/**
 * Se implementa la clase abstracta "Firma", encargada de ser la clase base de todos los tipos de firma digital a utilizar.
 *
 * @author JM
 *
 * @date 17/11/2003 - JM  @li Versión inicial
**/
#ifndef Firma_H
#define Firma_H

#include "BufferDinamico.h"

#include "Persistente.h"
#include "Serializable.h"


class Firma :	public BufferDinamico,
				public Persistente,
				public Serializable
{
public:
	Firma();
	Firma(const Firma &copy);	///< copy constructor
	Firma(const void* buff, DWORD size);
	virtual ~Firma();

	/// implementación de IPersistente
	/// Se guarda la firma en formato alfanumérico (lo que retorne "serializable")
	virtual DWORD saveToFile(const char* filename);
	virtual DWORD loadFromFile(const char* filename);

	/// también se permite guardar la firma de forma binaria (directamente lo que hay en el buffer interno)
	virtual DWORD saveToBinaryFile(const char* filename);
	virtual DWORD loadFromBinaryFile(const char* filename);

	/// ISerializable
	virtual ArsCadena serializar() = 0;
	virtual bool deserializar(const ArsCadena &serie) = 0;
	virtual bool deserializar(const char* serie, DWORD size = 0) = 0;

private:
	DWORD saveData(const char* filename, LPCBYTE data, DWORD len);
	DWORD loadData(const char* filename, BufferDinamico &datos);
};


#endif
