/**
 * Se implementa una clase abstracta base para la definición de aquellos servicios
 * que permiten la verificación de la validez de un certificado, bien sea su
 * caducidad o su estado de revocación.
 *
 * @author JM
 *
 * @date 19/01/2004 - JM  @li Versión inicial
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

	/// Informa si un certificado está revocado, buscándolo en la lista de CRLs disponibles en el almacén
	virtual EstadoRevocacion estaRevocado(const Certificado *cert);

private:
	AlmacenCertificadoCL  *almacen;
};


#endif