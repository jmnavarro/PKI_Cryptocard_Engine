/**
 * Se implementa una especialización de "AlmacenCertificado" que implementa toda la
 * funcionalidad utilizando el CryptoAPI de Microsoft.
 *
 * @author JM
 *
 * @date 10/12/2003 - JM @li Versión inicial
 * @date 25/01/2004 - JM @li Se implementa el método de enumeración de certificados
 * @date 12/02/2004 - JM @li El método "loadCertificado" ahora busca por número de serie
 * @date 25/02/2004 - ARS y JM @li Los métodos "open" y "close" pasar a sobrescribir los del padre. En el constructor no se abre el almacén.
 * @date 01/08/2004 - JM  @li Se permite crear el almacén a partir del handle de un CSP.
 * @date 08/09/2004 - JM  @li Soporte para gestionar (añadir y eliminar) las CRLs del almacén.
 * @date 25/01/2005 - JM  @li Modificado el interfaz de "almacenarCRL" para pasar el nombre del fichero CRL
 * @date 24/02/2005 - JM  @li Se renombra de AlmacenCAPI a AlmacenCertificadoCAPI
 *
 * @test AlmacenCAPITest
**/
#if !defined(_ALMACEN_CERTIFICADO_CAPI_H_)
#define _ALMACEN_CERTIFICADO_CAPI_H_

#include "AlmacenCertificado.h"

#include <wincrypt.h>



class AlmacenCertificadoCAPI : public AlmacenCertificado
{
public:
	AlmacenCertificadoCAPI(const ArsCadena &nombre, HCRYPTPROV  _csp = 0);
	virtual ~AlmacenCertificadoCAPI();

	/** Apertura del almacén.
	 *  @test AlmacenCertificadoCAPITest::testOpenAlmacen()
	**/
	virtual bool open();

	/** Cierre del almacén */
	virtual bool close();

	/** Busca un certificado en el almacén, crea un objeto Certificado y carga
	 *  sus datos desde el almacén.
	 *  @test AlmacenCertificadoCAPITest::testLoadCertificadoFromAsunto
	 *  @test AlmacenCertificadoCAPITest::testLoadCertificadoFromNumSerie
	**/
	virtual Certificado* loadCertificado(ClaveBusqueda claveBusqueda, void *data);

	/** Método <i>builder</i> para la creación de objetos de validación del certificado. */
	virtual ValidacionCertificado*  createValidacionCertificado();

	/** Método <i>builder</i> para crear y cargar los datos de un objeto
	 *  Certificado a partir de un handle.
	 **/
	virtual Certificado* crearCertificado(PCCERT_CONTEXT  hCert);

	/** Enumera los certificados.
	 *  La primera llamada debe hacerse con NULL, para retornar el 1er certificado.
	 *  Las siguientes llamadas con el anterior retornado por esta función.
	 *  La función crea un nuevo certificado para retornarlo, y el "anterior" siempre
	 *  se liberará de memoria.
	 *  @test AlmacenCAPITest::testEnumerarCertificados
	**/
	Certificado* enumCertificados(Certificado* anterior);

	/** @return Número de certificados que hay en el almacén o -1 en caso de error */
	virtual int getNumeroCertificados();

	virtual DWORD eliminarCRL(const ArsCadena &emisor);
	virtual bool  almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos);

	HCERTSTORE getHandle() const;
	void setCSP(HCRYPTPROV csp);

protected:
	static bool EsNombreAlmacenSistema(const ArsCadena &nombre);
	bool CorrespondeCertificadoConCSP(PCCERT_CONTEXT cert) const;

private:
	void* calcularParametrosBusqueda(ClaveBusqueda claveBusqueda, void* datos, DWORD &tipo);
	void liberarParametrosBusqueda(ClaveBusqueda claveBusqueda, void* parametros);

	HCRYPTPROV  csp;
	HCERTSTORE  handle;
};


#endif


