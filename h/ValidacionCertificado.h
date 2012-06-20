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
#ifndef ValidacionCertificado_H
#define ValidacionCertificado_H


#include "Certificado.h"


class ValidacionCertificado
{
public:
	/// Valores retornados por el m�todo estaRevocado
	typedef enum
	{
		EstadoRevValido,
		EstadoRevRevocado,
		EstadoRevError
	} EstadoRevocacion;

	/// Valores retornados por el m�todo estaCaducado
	typedef enum
	{
		EstadoCadValido,
		EstadoCadCaducado,
		EstadoCadError
	} EstadoCaducidad;

public:
	ValidacionCertificado();
	virtual ~ValidacionCertificado();

	/// Informa si un certificado est� revocado, seg�n el m�todo implementado en la clase concreta
	virtual EstadoRevocacion estaRevocado(const Certificado *cert) = 0;

	/// Informa si un certificado est� caducado, seg�n el m�todo implementado en la clase concreta
	virtual EstadoCaducidad  estaCaducado(const Certificado *cert) = 0;
};


#endif
