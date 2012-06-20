/**
 * Se implementa una clase abstracta base para la definici�n de aquellos servicios
 * que permiten la verificaci�n de la validez de un certificado, bien sea su
 * caducidad o su estado de revocaci�n.
 *
 * @author JM
 *
 * @date 02/02/2004 - JM  @li Versi�n inicial
 * @date 20/10/2004 - JM  @li Se cambia el tipo de retorno de los m�todos. Pasa de dos valores de retorno (correcto/incorrecto) a tres (correcto/incorrecto/error).
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

	/// Informa si un certificado est� revocado, busc�ndolo en la lista de CRLs disponibles en el almac�n
	virtual EstadoRevocacion estaRevocado(const Certificado *cert);

	/// Informa si un certificado est� caducado, a trav�s de las funciones de CryptoAPI
	virtual EstadoCaducidad estaCaducado(const Certificado *cert);
	
private:
	AlmacenCertificadoCAPI  *almacen;
};


#endif