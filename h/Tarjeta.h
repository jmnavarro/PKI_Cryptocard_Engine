/**
 * Se implementa una clase abstracta base que representa una tarjeta criptográfica.
 *
 * @author JM
 *
 * @date 02/08/2004 - JM  @li Versión inicial
 * @date 23/09/2004 - JM  @li Se permite crear la tarjeta a partir de un certificado.
 * @date 09/05/2005 - JM  @li Se añade un método abstracto para obtener la etiqueta de la tarjeta (a implementar en los descencientes)
**/
#if !defined(_TARJETA_H_)
#define _TARJETA_H_


#include "../../LibUtils/h/ArsCadena.h"


class AlmacenCertificado;
class Firmador;
class Certificado;


class Tarjeta
{
public:
	Tarjeta(const ArsCadena &_nombre);
	virtual ~Tarjeta();

	virtual bool conectar(Certificado *cert = NULL) = 0;
	virtual bool desconectar() = 0;
	virtual bool isConectado() const = 0;

	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre) = 0;
	virtual bool cerrarAlmacen(AlmacenCertificado* almacen) = 0;

	virtual bool establecerPIN(const ArsCadena &pin, ArsCadena &msg) = 0;

	virtual Firmador* createFirmador() = 0;

	virtual ArsCadena getEtiqueta() const = 0;

protected:
	ArsCadena getNombre() const;

private:
	ArsCadena  nombre;
};


#endif
