/**
 * Define la clase abstracta AlmacenCertificado para la obtenci�n de certificados digitales.
 *
 * @author JM
 *
 * @date 10/12/2003 - JM  @li Versi�n inicial
 * @date 16/01/2004 - JM  @li A�adidos m�todos virtuales "open" y "close" para abrir y cerrar almac�n de certificados en el constructor y destructor.
 * @date 02/02/2004 - JM  @li Se mueven a los descendientes los m�todos "open" y "close" debido a la limitaci�n de C++ de no poder llamar a m�todos polim�rficos en el constructor/destructor.
 * @date 25/02/2004 - JM  @li A�adidos los m�todos abstractos "open" y "close"
 * @date 29/09/2004 - JM  @li A�adido el m�todo "getNumeroCertificados"
 * @date 08/09/2004 - JM  @li Soporte para gestionar (a�adir y eliminar) las CRLs del almac�n
 * @date 04/01/2005 - JM  @li Nuevos tipos de b�squeda soportados por cryptLib.
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

	/// apertura del almac�n
	virtual bool open()  = 0;
	/// cierre del almac�n
	virtual bool close() = 0;

	/// m�todo abstracto que crear� un certificado concreto
	virtual Certificado* loadCertificado(ClaveBusqueda claveBusqueda, void *data) = 0;
	/// m�todo abstracto que crear� un objeto de revocaci�n
	virtual ValidacionCertificado*  createValidacionCertificado() = 0;

	/// enumeracion de certificados
	virtual Certificado* enumCertificados(Certificado* anterior) = 0;

	/// cuenta de los certificados que hay en el almac�n
	virtual int getNumeroCertificados() = 0;

	virtual DWORD eliminarCRL(const ArsCadena &emisor) = 0;
	virtual bool  almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos) = 0;

private:
	ArsCadena	nombre;
};


#endif
