/**
 * Se implementa una clase abstracta base para la definición de aquellos servicios
 * que permiten la verificación de la validez de un certificado, bien sea su
 * caducidad o su estado de revocación cuando la CRL está almacenada en local.
 *
 * @author JM
 *
 * @date 02/02/2004 - JM  @li Versión inicial
 * @date 20/10/2004 - JM  @li Se cambia el tipo de retorno de los métodos. Pasa de dos valores de retorno (correcto/incorrecto) a tres (correcto/incorrecto/error).
**/
#ifndef ValidacionCertificadoLocal_H
#define ValidacionCertificadoLocal_H

#include "ValidacionCertificado.h"


class ValidacionCertificadoLocal : public ValidacionCertificado
{
public:
	ValidacionCertificadoLocal();
	virtual ~ValidacionCertificadoLocal();

	/// Informa si un certificado está revocado. Para ello se comparan las fechas del certificado con la fecha local del sistema.
	virtual EstadoCaducidad estaCaducado(const Certificado *cert);
};


#endif
