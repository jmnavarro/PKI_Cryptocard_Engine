/**
 * Se implementa una clase abstracta base para la definición de aquellos servicios
 * que permiten la verificación de la validez de un certificado, bien sea su
 * caducidad o su estado de revocación.
 *
 * @author JM
 *
 * @date 02/02/2004 - JM  @li Versión inicial
 * @date 20/10/2004 - JM  @li Se cambia el tipo de retorno de los métodos. Pasa de dos valores de retorno (correcto/incorrecto) a tres (correcto/incorrecto/error).
**/
#ifndef ValidacionCertificadoCAPI_H
#define ValidacionCertificadoCAPI_H


#include "ValidacionCertificadoLocal.h"


class AlmacenCertificadoCAPI;


class ValidacionCertificadoCAPI : public ValidacionCertificadoLocal
{
public:
	ValidacionCertificadoCAPI(AlmacenCertificadoCAPI *store);
	virtual ~ValidacionCertificadoCAPI();

	/// Informa si un certificado está revocado, buscándolo en la lista de CRLs disponibles en el almacén
	virtual EstadoRevocacion estaRevocado(const Certificado *cert);

	/// Informa si un certificado está caducado, a través de las funciones de CryptoAPI
	virtual EstadoCaducidad estaCaducado(const Certificado *cert);
	
private:
	AlmacenCertificadoCAPI  *almacen;
};


#endif