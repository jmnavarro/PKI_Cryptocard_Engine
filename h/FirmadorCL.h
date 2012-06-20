/**
 * Se implementa una especializaci�n de "Firmador" que implementa toda la
 * funcionalidad utilizando la librer�a cryptLib.
 *
 * @author JM
 *
 * @date 04/01/2005 - JM  @li Versi�n inicial
 *
**/
#if !defined(_FIRMADIR_CL_H_)
#define _FIRMADIR_CL_H_


#include "cryptLibAPI.h"
#include "Firmador.h"


class FirmadorCL : public Firmador
{
public:
	FirmadorCL(CL_DEVICE _dev);
	virtual ~FirmadorCL();

protected:
	// este m�todo calcula la firma de un documento y la almacena en un buffer
	virtual bool calcularFirma(	const BufferDinamico &doc,
								const Certificado 	 &cert,
								BufferDinamico		 &firma);

	// y este obtiene el resumen de un documento
	virtual bool calcularResumen(const BufferDinamico &doc, BufferDinamico &hash);

	/**
	 *  Valida una firma
	 *  Se debe validar con el certificado pasado por par�metro
	 **/
	virtual bool validarFirma(const BufferDinamico &data, const Firma &firma, const Certificado *cert = NULL);

private:
	CL_DEVICE dev;
};


#endif
