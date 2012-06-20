/**
 * Se implementa una clase abstracta base para la definici�n de aquellos servicios
 * que permiten la verificaci�n de la validez de un certificado, bien sea su
 * caducidad o su estado de revocaci�n cuando la CRL est� almacenada en local.
 *
 * @author JM
 *
 * @date 02/02/2004 - JM  @li Versi�n inicial
 * @date 20/10/2004 - JM  @li Se cambia el tipo de retorno de los m�todos. Pasa de dos valores de retorno (correcto/incorrecto) a tres (correcto/incorrecto/error).
**/
#ifndef ValidacionCertificadoLocal_H
#define ValidacionCertificadoLocal_H

#include "ValidacionCertificado.h"


class ValidacionCertificadoLocal : public ValidacionCertificado
{
public:
	ValidacionCertificadoLocal();
	virtual ~ValidacionCertificadoLocal();

	/// Informa si un certificado est� revocado. Para ello se comparan las fechas del certificado con la fecha local del sistema.
	virtual EstadoCaducidad estaCaducado(const Certificado *cert);
};


#endif
