/**
 * Se implementa una clase abstracta base para la definici�n de aquellos servicios
 * que permiten la verificaci�n de la validez de un certificado, bien sea su
 * caducidad o su estado de revocaci�n.
 *
 * @author JM
 *
 * @date 19/01/2004 - JM  @li Versi�n inicial
**/
#ifndef ValidacionCertificadoCL_H
#define ValidacionCertificadoCL_H


#include "cryptLibAPI.h"
#include "ValidacionCertificadoLocal.h"


class AlmacenCertificadoCL;


class ValidacionCertificadoCL : public ValidacionCertificadoLocal
{
public:
	ValidacionCertificadoCL(AlmacenCertificadoCL *store);
	virtual ~ValidacionCertificadoCL();

	/// Informa si un certificado est� revocado, busc�ndolo en la lista de CRLs disponibles en el almac�n
	virtual EstadoRevocacion estaRevocado(const Certificado *cert);

private:
	AlmacenCertificadoCL  *almacen;
};


#endif