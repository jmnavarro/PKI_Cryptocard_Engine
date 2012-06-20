/**
 * Se implementa una clase abstracta base que representa un certificado. 
 * A su vez, desciende de "Firmador", ya que un certificado es un elemento con la capacidad de firmar datos.
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versión inicial
 * @date 02/02/2004 - JM  @li La clase "Certificado" deja de ser un descendiente de "Firmador".
 * @date 12/02/2004 - JM  @li Se añade el atributo NumeroSerie
 * @date 28/09/2004 - JM  @li Se añade el método "getKeyContainerName" para obtener el nombre del contenedor de claves donde reside el certificado.
 * @date 13/10/2004 - JM  @li Se implementa el interfaz ISerializable
**/
#ifndef Certificado_H
#define Certificado_H

#include "Serializable.h"

#include <windows.h>
#include "../../LibUtils/h/ArsCadena.h"


class Certificado : public Serializable
{
public:
	Certificado();
	Certificado(const ArsCadena &encoded);
	Certificado(Certificado* copy);			///< copy-constructor
	virtual ~Certificado();

	bool equals(Certificado* comparar);		///< compara un objeto con otro
	virtual Certificado* clone() = 0;		///< retorna un copia exacta objeto

	SYSTEMTIME getFechaDesde() const;
	SYSTEMTIME getFechaHasta() const;
	void setFechaDesde(const LPSYSTEMTIME value);
	void setFechaHasta(const LPSYSTEMTIME value);

	ArsCadena getNombre() const;
	ArsCadena getEmisor() const;
	ArsCadena getNombreEmisor() const;
	ArsCadena getNumeroSerie() const;
	ArsCadena getKeyContainer() const;

	ArsCadena getIdentificador() const;

	// ISerializable
	virtual ArsCadena serializar() = 0;
	virtual bool deserializar(const ArsCadena &serie) = 0;

protected:
	virtual void loadData() = 0;
	virtual void resetData();

	void setNombre(const ArsCadena &value);
	void setEmisor(const ArsCadena &value);
	void setNombreEmisor(const ArsCadena &value);
	void setNumeroSerie(const ArsCadena &value);
	void setKeyContainer(const ArsCadena &value);

private:
	SYSTEMTIME fechaDesde;
	SYSTEMTIME fechaHasta;

	ArsCadena	nombre;
	ArsCadena	emisor;
	ArsCadena	nombreEmisor;
	ArsCadena	numeroSerie;
	ArsCadena	keyContainer;
};


#endif
