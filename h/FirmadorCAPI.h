/**
 * Se implementa una especialización de "Firmador" que implementa toda la
 * funcionalidad utilizando el CryptoAPI de Microsoft.
 *
 * @author JM
 *
 * @date 02/02/2004 - JM  @li Versión inicial
 * @date 01/08/2004 - JM  @li Se permite crear un firmador a partir del handle de un CSP
 * @date 29/11/2004 - JM  @li Implementado el método abstracto "validarFirma"
 *
**/
#ifndef FirmadorCAPI_H
#define FirmadorCAPI_H

#include "Firmador.h"


class FirmadorCAPI : public Firmador
{
public:
	FirmadorCAPI(HCRYPTPROV  _csp = 0);
	virtual ~FirmadorCAPI();

protected:
	// este método calcula la firma de un documento y la almacena en un buffer
	virtual bool calcularFirma(	const BufferDinamico &doc,
								const Certificado 	 &cert,
								BufferDinamico		 &firma);

	// y este obtiene el resumen de un documento
	virtual bool calcularResumen(const BufferDinamico &doc, BufferDinamico &hash);

	/**
	 *  Valida una firma
	 *  Se puede validar con el certificado pasado por parámetro, o con el certificado incrustado dentro de la firma si el parámetro es NULL
	 **/
	virtual bool validarFirma(const BufferDinamico &data, const Firma &firma, const Certificado *cert = NULL);

private:
	Certificado* getCertificadoDeMensajeFirmado(HCRYPTMSG msg);

	HCRYPTPROV  csp;
};


#endif
