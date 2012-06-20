/**
 * Se implementa una especialización de "AlmacenCertificado" que implementa toda la
 * funcionalidad utilizando la librería cryptográfica "cryptLib"
 *
 * @author JM
 *
 * @date 04/01/2005 - JM @li Versión inicial
 * @date 24/02/2005 - JM @li Se renombra de AlmacenCL a AlmacenCertificadoCL
 * @date 11/03/2005 - JM @li Añadido el método reloadCertificado
 *
 * @test AlmacenCertificadoCLTest
**/
#if !defined(_ALMACEN_CERTIFICADO_CL_H_)
#define _ALMACEN_CERTIFICADO_CL_H_


#include "cryptLibAPI.h"
#include "AlmacenCertificado.h"
#include "../../../Base/LibDato/h/Vector.h"


class CertCL;


class AlmacenCertificadoCL : public AlmacenCertificado
{
public:
	/**
	 * Si pasa un nombre vacío, se trata del almacén de la tarjeta.
	 * En caso contrario, debe ser una ruta (absoluta o relativa) donde se almacenarán los datos
	**/
	AlmacenCertificadoCL(const ArsCadena &nombre, const CL_DEVICE &_dev = 0);
	virtual ~AlmacenCertificadoCL();

	/** Apertura del almacén.
	 *  En cryptLib no hace nada.
	**/
	virtual bool open();

	/** Cierre del almacén
	 *  @copydoc AlmacenCertificado::close
	**/
	virtual bool close();

	bool isCerrado();

	/** Busca un certificado en el almacén, crea un objeto Certificado y carga
	 *	sus datos desde el almacén.
	 *	El API cryptLib solo permite busquedas por nombre y e-mail, que en realidad
	 *	son búsquedas por la etiqueta (CKA_LABEL) del certificado dentro del
	 *	dispositivo PKCS#11.
	 *	Hemos tenido que modificar cryptLib para que permita la "búsqueda" del
	 * primer certificado de la tarjeta.
	**/
	virtual Certificado* loadCertificado(ClaveBusqueda claveBusqueda, void *data);

	/** Dado un certificado que tiene cargada la clave pública PERO NO la privada,
	 *  carga la clave privada.
	**/
	virtual bool reloadCertificado(CertCL* certificado);

	/** Método <i>builder</i> para la creación de objetos de validación del certificado. */
	virtual ValidacionCertificado*  createValidacionCertificado();

	/** Enumera los certificadosm, aunque en cryptLib esta enumeración solo
	 *  retornará un certificado, aunque haya más de uno.
	**/
	Certificado* enumCertificados(Certificado* anterior);

	/** @return Número de certificados que hay en el almacén o -1 en caso de error.
	 *  En cryptLib esta función siempre retorna 1 si hay algún certificado o 0
	 *  si no hay ninguno.
	**/
	virtual int getNumeroCertificados();

	ArsCadena getCRL(int index);
	virtual DWORD eliminarCRL(const ArsCadena &nombreFichero);
	virtual bool  almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos);

protected:
	/** Método <i>builder</i> para crear y cargar los datos de un objeto
	 *  Certificado a partir de sus claves
	 **/
	virtual CertCL* crearCertificado(const CL_CERTIFICATE clavePublica, const CL_CERTIFICATE clavePrivada);

	char* calcularParametrosBusqueda(ClaveBusqueda claveBusqueda, void *data, CL_KEYID_TYPE *keyIDType);
	void recordarParametrosBusqueda(CertCL *certificado, ClaveBusqueda claveBusqueda, char* id);

private:
	typedef Vector<ArsCadena>	ListaCRL;

private:
	bool hayCertificadosEnTarjeta();
	bool esAlmacenEnTarjeta();
	ArsCadena getRutaAlmacen();
	ArsCadena extraerNombreFichero(const ArsCadena &rutaFichero);

	int cargarCRLs();
	bool comprobarDatosCRL(const void* datos, DWORD lenDatos);
	bool copiarNuevaCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos);


	CL_DEVICE	dev;
	CL_KEYSET	handle;
	ListaCRL		listaCRL;
};


#endif
