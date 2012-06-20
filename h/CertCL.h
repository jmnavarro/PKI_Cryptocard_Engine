/**
 * Se implementa una especialización de "Certificado" que implementa toda la
 * funcionalidad utilizando la librería cryptLib.
 *
 * @author JM
 *
 * @date 04/01/2005 - JM  @li Versión inicial
 * @date 12/01/2005 - JM  @li Se mantienen el handle tanto de la clave privada como de la pública
 * @date 11/03/2005 - JM  @li Añadido el control de los parámetros de búsqueda para saber a través de qué método se cargó un certificado 
 * @date 06/05/2005 - JM  @li Añadido constructor por defecto 
**/
#if !defined(_CERT_CL_H_)
#define _CERT_CL_H_


#include "cryptLibAPI.h"
#include "../h/Certificado.h"


class CertCL : public Certificado
{
public:
	typedef enum
	{
		BuscadoPorEtiqueta,
		BuscadoPorPosicion,
		BuscadoPorNada
	} ParametrosBusqueda;

public:
	CertCL();
	CertCL(const CL_CERTIFICATE publica, const CL_CERTIFICATE privada);
	CertCL(CertCL *copy);
	CertCL(const ArsCadena &encoded);
	virtual ~CertCL();

	virtual Certificado* clone();

	bool tieneClavePrivada() const;
	CL_CERTIFICATE getClavePrivada() const;
	CL_CERTIFICATE getClavePublica() const;

	void setClavePrivada(const CL_CERTIFICATE value);
	void setClavePublica(const CL_CERTIFICATE value);

	// ISerializable
	virtual ArsCadena serializar();
	virtual bool deserializar(const ArsCadena &serie);

	char* getParametrosBuscado(CL_KEYID_TYPE *tipo);
	void setParametrosBuscado(ParametrosBusqueda params, char* clave);

protected:
	virtual void loadData();

	void loadFechas();
	void loadNombre();
	void loadEmisor();
	void loadNombreEmisor();
	void loadNumeroSerie();
	void loadKeyContainer();

	void liberarClaves(void);
	CL_CERTIFICATE duplicarClavePublica(const CL_CERTIFICATE origen);

private:
	ArsCadena getCadenaAtributo(CL_ATTRIBUTE_TYPE tipo, bool codificar);
	int getCommonName(char* buff, int* len);

	CL_CERTIFICATE	clavePrivada;
	CL_CERTIFICATE	clavePublica;

	ParametrosBusqueda	paramsBusqueda;
	ArsCadena			claveBusqueda;
};


#endif
