/**
 * Define la clase abstracta Firmador para la generaci�n de firmas digitales
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versi�n inicial
 * @date 02/02/2004 - JM  @li Se a�ade la entidad "Certificado" como par�metro de los m�todos "firmar" y "validar"
 * @date 13/09/2004 - JM  @li Se elimina el m�todo "calcularOriginal".
 * @date 29/11/2004 - JM  @li Cambiado el sistema para verificar firma. Se introduce un m�todo abstracto que se implementar� con la validaci�n de la firma.
 *
**/
#ifndef Firmador_H
#define Firmador_H

#include "Firma.h"
#include "Firmable.h"
#include "Resumen.h"
#include "Certificado.h"


class Firmador
{
public:
	/// Tipos de firma soportados.
	typedef enum
	{
		TipoFirmaDetached	///< Por ahora s�lo se soporta el tipo de forma Detached, que es el que genera CryptoAPI.
	} TipoFirma;

public:
	Firmador();
	virtual ~Firmador();

	//@{
	/** M�todo de firma.
	 * En realidad, el m�todo de firma lo que hace es crear un resumen de los datos pasados, y despu�s genera la firma a partir de ese resumen.
	**/
	Firma* firmar(TipoFirma tipo, const ArsCadena &data, const Certificado &cert);
	Firma* firmar(TipoFirma tipo, const char *data, const Certificado &cert);
	Firma* firmar(TipoFirma tipo, const LPBYTE data, DWORD size, const Certificado &cert);
	Firma* firmar(TipoFirma tipo, const BufferDinamico &data, const Certificado &cert);
	//@}

	//@{
	// M�todos de resumen
	Resumen* resumir(const ArsCadena &data);
	Resumen* resumir(const char *data);
	Resumen* resumir(const LPBYTE data, DWORD size);
	Resumen* resumir(const BufferDinamico &data);
	//@}

	//@{
	// M�todos de validaci�n
	bool validar(const char* data, const Firma &firma, const Certificado *cert = NULL);
	bool validar(const LPBYTE data, DWORD size, const Firma &firma, const Certificado *cert = NULL);
	bool validar(const BufferDinamico &data, const Firma &firma, const Certificado *cert = NULL);
	//@}

protected:
	/// M�todo factory: encapsula la creaci�n de objetos firma. Podr�a sobrescribirse para crear objetos espec�ficos
	virtual Firma*	 crearFirma(TipoFirma tipo);

	/// M�todo factory: encapsula la creaci�n de objetos firma. Podr�a sobrescribirse para crear objetos espec�ficos
	virtual Resumen* crearResumen();

	/// Este m�todo calcula la firma de un documento y la almacena en un buffer
	virtual bool calcularFirma(	const BufferDinamico &doc,
								const Certificado 	 &cert,
								BufferDinamico		 &firma) = 0;

	/// Obtiene el resumen de un documento
	virtual bool calcularResumen(const BufferDinamico &doc, BufferDinamico &hash) = 0;

	/**
	 *  Valida una firma
	 *  Se puede validar con el certificado pasado por par�metro, o con el certificado incrustado dentro de la firma si el par�metro es NULL
	 **/
	virtual bool validarFirma(const BufferDinamico &data, const Firma &firma, const Certificado *cert = NULL) = 0;
};


#endif

