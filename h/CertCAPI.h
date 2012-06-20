/**
 * Se implementa una especialización de "Certificado" que implementa toda la funcionalidad utilizando el CryptoAPI de Microsoft.
 *
 * @author JM
 *
 * @date 10/12/2003 - JM  @li Versión inicial
 * @date 12/02/2004 - JM  @li Añadidos los método de carga de  datos (loadXXX)
 * @date 29/09/2004 - JM  @li Añadido el método para averiguar el nombre del contenedor de claves
 * @date 06/05/2005 - JM  @li Añadido constructor por defecto
**/
#ifndef CertCAPI_H
#define CertCAPI_H

#include "../h/Certificado.h"

#include <windows.h>
#include <wincrypt.h>


#define TIPO_CODIFICACION	(PKCS_7_ASN_ENCODING | X509_ASN_ENCODING)


class CertCAPI : public Certificado
{
public:
	CertCAPI();
	CertCAPI(PCCERT_CONTEXT handle);
	CertCAPI(const ArsCadena &encoded);
	CertCAPI(CertCAPI *copy);
	virtual ~CertCAPI();

	virtual Certificado* clone();

	PCCERT_CONTEXT getHandle() const;
	PCERT_INFO getInfo() const;

	void setHandle(PCCERT_CONTEXT value);

	// ISerializable
	virtual ArsCadena serializar();
	virtual bool deserializar(const ArsCadena &serie);

	static ArsCadena GetNombreCSP(PCCERT_CONTEXT cert);

protected:
	virtual void loadData();

	void loadFechas();
	void loadNombre();
	void loadEmisor();
	void loadNombreEmisor();
	void loadNumeroSerie();
	void loadKeyContainer();

private:
	PCCERT_CONTEXT	handle;
};


#endif

