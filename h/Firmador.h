/**
 * Define la clase abstracta Firmador para la generación de firmas digitales
 *
 * @author JM
 *
 * @date 20/11/2003 - JM  @li Versión inicial
 * @date 02/02/2004 - JM  @li Se añade la entidad "Certificado" como parámetro de los métodos "firmar" y "validar"
 * @date 13/09/2004 - JM  @li Se elimina el método "calcularOriginal".
 * @date 29/11/2004 - JM  @li Cambiado el sistema para verificar firma. Se introduce un método abstracto que se implementará con la validación de la firma.
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
		TipoFirmaDetached	///< Por ahora sólo se soporta el tipo de forma Detached, que es el que genera CryptoAPI.
	} TipoFirma;

public:
	Firmador();
	virtual ~Firmador();

	//@{
	/** Método de firma.
	 * En realidad, el método de firma lo que hace es crear un resumen de los datos pasados, y después genera la firma a partir de ese resumen.
	**/
	Firma* firmar(TipoFirma tipo, const ArsCadena &data, const Certificado &cert);
	Firma* firmar(TipoFirma tipo, const char *data, const Certificado &cert);
	Firma* firmar(TipoFirma tipo, const LPBYTE data, DWORD size, const Certificado &cert);
	Firma* firmar(TipoFirma tipo, const BufferDinamico &data, const Certificado &cert);
	//@}

	//@{
	// Métodos de resumen
	Resumen* resumir(const ArsCadena &data);
	Resumen* resumir(const char *data);
	Resumen* resumir(const LPBYTE data, DWORD size);
	Resumen* resumir(const BufferDinamico &data);
	//@}

	//@{
	// Métodos de validación
	bool validar(const char* data, const Firma &firma, const Certificado *cert = NULL);
	bool validar(const LPBYTE data, DWORD size, const Firma &firma, const Certificado *cert = NULL);
	bool validar(const BufferDinamico &data, const Firma &firma, const Certificado *cert = NULL);
	//@}

protected:
	/// Método factory: encapsula la creación de objetos firma. Podría sobrescribirse para crear objetos específicos
	virtual Firma*	 crearFirma(TipoFirma tipo);

	/// Método factory: encapsula la creación de objetos firma. Podría sobrescribirse para crear objetos específicos
	virtual Resumen* crearResumen();

	/// Este método calcula la firma de un documento y la almacena en un buffer
	virtual bool calcularFirma(	const BufferDinamico &doc,
								const Certificado 	 &cert,
								BufferDinamico		 &firma) = 0;

	/// Obtiene el resumen de un documento
	virtual bool calcularResumen(const BufferDinamico &doc, BufferDinamico &hash) = 0;

	/**
	 *  Valida una firma
	 *  Se puede validar con el certificado pasado por parámetro, o con el certificado incrustado dentro de la firma si el parámetro es NULL
	 **/
	virtual bool validarFirma(const BufferDinamico &data, const Firma &firma, const Certificado *cert = NULL) = 0;
};


#endif

