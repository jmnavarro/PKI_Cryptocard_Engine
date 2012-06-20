/**
 * Se implementa una especializaci�n de "AlmacenCertificado" que implementa toda la
 * funcionalidad utilizando la librer�a cryptogr�fica "cryptLib"
 *
 * @author JM
 *
 * @date 04/01/2005 - JM @li Versi�n inicial
 * @date 24/02/2005 - JM @li Se renombra de AlmacenCL a AlmacenCertificadoCL
 * @date 11/03/2005 - JM @li A�adido el m�todo reloadCertificado
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
	 * Si pasa un nombre vac�o, se trata del almac�n de la tarjeta.
	 * En caso contrario, debe ser una ruta (absoluta o relativa) donde se almacenar�n los datos
	**/
	AlmacenCertificadoCL(const ArsCadena &nombre, const CL_DEVICE &_dev = 0);
	virtual ~AlmacenCertificadoCL();

	/** Apertura del almac�n.
	 *  En cryptLib no hace nada.
	**/
	virtual bool open();

	/** Cierre del almac�n
	 *  @copydoc AlmacenCertificado::close
	**/
	virtual bool close();

	bool isCerrado();

	/** Busca un certificado en el almac�n, crea un objeto Certificado y carga
	 *	sus datos desde el almac�n.
	 *	El API cryptLib solo permite busquedas por nombre y e-mail, que en realidad
	 *	son b�squedas por la etiqueta (CKA_LABEL) del certificado dentro del
	 *	dispositivo PKCS#11.
	 *	Hemos tenido que modificar cryptLib para que permita la "b�squeda" del
	 * primer certificado de la tarjeta.
	**/
	virtual Certificado* loadCertificado(ClaveBusqueda claveBusqueda, void *data);

	/** Dado un certificado que tiene cargada la clave p�blica PERO NO la privada,
	 *  carga la clave privada.
	**/
	virtual bool reloadCertificado(CertCL* certificado);

	/** M�todo <i>builder</i> para la creaci�n de objetos de validaci�n del certificado. */
	virtual ValidacionCertificado*  createValidacionCertificado();

	/** Enumera los certificadosm, aunque en cryptLib esta enumeraci�n solo
	 *  retornar� un certificado, aunque haya m�s de uno.
	**/
	Certificado* enumCertificados(Certificado* anterior);

	/** @return N�mero de certificados que hay en el almac�n o -1 en caso de error.
	 *  En cryptLib esta funci�n siempre retorna 1 si hay alg�n certificado o 0
	 *  si no hay ninguno.
	**/
	virtual int getNumeroCertificados();

	ArsCadena getCRL(int index);
	virtual DWORD eliminarCRL(const ArsCadena &nombreFichero);
	virtual bool  almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos);

protected:
	/** M�todo <i>builder</i> para crear y cargar los datos de un objeto
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
