/**
 * Define la clase abstracta AlmacenCertificado para la obtención de certificados digitales.
 *
 * @author JM
 *
 * @date 10/12/2003 - JM  @li Versión inicial
 * @date 16/01/2004 - JM  @li Añadidos métodos virtuales "open" y "close" para abrir y cerrar almacén de certificados en el constructor y destructor.
 * @date 02/02/2004 - JM  @li Se mueven a los descendientes los métodos "open" y "close" debido a la limitación de C++ de no poder llamar a métodos polimórficos en el constructor/destructor.
 * @date 25/02/2004 - JM  @li Añadidos los métodos abstractos "open" y "close"
 * @date 29/09/2004 - JM  @li Añadido el método "getNumeroCertificados"
 * @date 08/09/2004 - JM  @li Soporte para gestionar (añadir y eliminar) las CRLs del almacén
 * @date 04/01/2005 - JM  @li Nuevos tipos de búsqueda soportados por cryptLib.
 * @date 25/01/2005 - JM  @li Modificado el interfaz de "almacenarCRL" para pasar el nombre del fichero CRL
 * @date 24/02/2005 - JM  @li Se cambia el nombre de Almacen a AlmacenCertificado
**/
#if !defined(_ALMACEN_CERTIFICADO_H_)
#define _ALMACEN_CERTIFICADO_H_


#include "../../LibUtils/h/ArsCadena.h"


class Certificado;
class ValidacionCertificado;


class AlmacenCertificado
{
public:
	typedef enum
	{
		ClaveBusquedaNumSerie,			// soportadas por CAPI
		ClaveBusquedaSubStrAsunto,		// soportadas por CAPI
		ClaveBusquedaNombre,			// soportadas por CL
		ClaveBusquedaEMail,				// soportadas por CL
		ClaveBusquedaPrimero			// soportadas por CL
	} ClaveBusqueda;

	typedef struct
	{
		ArsCadena  emisor;
		ArsCadena  numSerie;
	} NumeroSerie;

public:
	AlmacenCertificado(const ArsCadena &nombre);
	virtual ~AlmacenCertificado();

	void setNombre(const ArsCadena &nombre);
	ArsCadena& getNombre();

	/// apertura del almacén
	virtual bool open()  = 0;
	/// cierre del almacén
	virtual bool close() = 0;

	/// método abstracto que creará un certificado concreto
	virtual Certificado* loadCertificado(ClaveBusqueda claveBusqueda, void *data) = 0;
	/// método abstracto que creará un objeto de revocación
	virtual ValidacionCertificado*  createValidacionCertificado() = 0;

	/// enumeracion de certificados
	virtual Certificado* enumCertificados(Certificado* anterior) = 0;

	/// cuenta de los certificados que hay en el almacén
	virtual int getNumeroCertificados() = 0;

	virtual DWORD eliminarCRL(const ArsCadena &emisor) = 0;
	virtual bool  almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos) = 0;

private:
	ArsCadena	nombre;
};


#endif
