#include "../h/CertCL.h"
#include "../h/ConversorDatos.h"

#include <assert.h>
#include <time.h>


#define SIZE_CERTIFICADO_EXPORTADO	(4 * 1024) /* 4 KB */



inline void StdTimeToSystemTime(struct tm *in, SYSTEMTIME *out)
{
	out->wYear			= in->tm_year + 1900;
	out->wDayOfWeek		= in->tm_wday;
	out->wMonth			= in->tm_mon + 1;
	out->wDay			= in->tm_mday;
	out->wHour			= in->tm_hour;
	out->wMinute		= in->tm_min;
	out->wSecond		= in->tm_sec;
	out->wMilliseconds 	= 0;
}


CertCL::CertCL()
	: Certificado()
{
	clavePublica = 0;
	clavePrivada = 0;

	resetData();
}


CertCL::CertCL(const CL_CERTIFICATE publica, const CL_CERTIFICATE privada)
	: Certificado()
{
	clavePublica = publica;
	clavePrivada = privada;

	paramsBusqueda	= BuscadoPorNada;

	loadData();
}


CertCL::CertCL(const ArsCadena &encoded)
	: Certificado()
{
	clavePublica = 0;
	clavePrivada = 0;

	deserializar(encoded);
}


CertCL::CertCL(CertCL *copy)
	: Certificado(copy)
{
	// Hay que duplicar el handle de las claves, porque si no, tendremos dos objetos
	// CertCL apuntando hacia los mismos handles
	// Debido al problema de duplicación del handles en cryptLib (ver los comentarios
	// del método "duplicarClavePublica"), un certificado creado a través de este
	// constructor no tendrá clave privada.
	this->clavePublica = 0;
	this->clavePrivada = 0;

	if (copy->getClavePublica() != 0)
		this->clavePublica = duplicarClavePublica(copy->getClavePublica());
}


CertCL::~CertCL()
{
	liberarClaves();
}


Certificado* CertCL::clone()
{
	return ( new CertCL(this) );
}


bool CertCL::tieneClavePrivada() const
{
	return (clavePrivada != 0);
}


CL_CERTIFICATE CertCL::getClavePrivada() const
{
	return (clavePrivada);
}

CL_CERTIFICATE CertCL::getClavePublica() const
{
	return (clavePublica);
}


void CertCL::setClavePrivada(const CL_CERTIFICATE value)
{
	clavePrivada = value;
	loadData();
}


void CertCL::setClavePublica(const CL_CERTIFICATE value)
{
	clavePublica = value;
	loadData();
}

void CertCL::loadData()
{
	if (clavePublica != 0)
	{
		loadFechas();
		loadNombre();
		loadEmisor();
		loadNombreEmisor();
		loadNumeroSerie();
		loadKeyContainer();
	}
	else
		resetData();
}


void CertCL::loadFechas()
{
	time_t		t_aux;
	struct tm	*fecha;
	int			len;
	int			status;
	SYSTEMTIME	tmp;

	len = 0;
	status = CL->getAttributeString(clavePublica, CL_CERTINFO_VALIDFROM, (void*)&t_aux, &len );
	if ( CL->isOK(status) )
	{
		fecha = localtime(&t_aux);
		StdTimeToSystemTime(fecha, &tmp);

		setFechaDesde(&tmp);
	}

	len = 0;
	status = CL->getAttributeString(clavePublica, CL_CERTINFO_VALIDTO, (void*)&t_aux, &len );
	if ( CL->isOK(status) )
	{
		fecha = localtime(&t_aux);
		StdTimeToSystemTime(fecha, &tmp);

		setFechaHasta(&tmp);
	}
}


void CertCL::loadNombre()
{
	ArsCadena  str;

	str = getCadenaAtributo(CL_CERTINFO_COMMONNAME, false);

	setNombre(str);
}


void CertCL::loadEmisor()
{
	ArsCadena  str;

	str = getCadenaAtributo(CL_CERTINFO_ISSUERUNIQUEID, true);

	setEmisor(str);
}


void CertCL::loadNombreEmisor()
{
	ArsCadena  str;

	str = getCadenaAtributo(CL_CERTINFO_ISSUERNAME, false);

	setNombreEmisor(str);
}


void CertCL::loadNumeroSerie()
{
	ArsCadena  str;

	str = getCadenaAtributo(CL_CERTINFO_SERIALNUMBER, true);

	setNumeroSerie(str);
}


void CertCL::loadKeyContainer()
{
	setKeyContainer("");
}


ArsCadena CertCL::serializar()
{
	ArsCadena	str;
	LPBYTE		datos;
	int			len;
	int			status;

	if (clavePublica != 0)
	{
		len = SIZE_CERTIFICADO_EXPORTADO;
		datos = new BYTE[len];
		memset(datos, 0, len);

		// en principio uso la clave pública pero habría que verlo
		status = CL->exportCert(datos, &len, CL_CERTFORMAT_TEXT_CERTCHAIN, clavePublica);
		if ( CL->isOK(status) )
			str = (char*)datos;

		delete [] datos;
	}

	return (str);
}


bool CertCL::deserializar(const ArsCadena &serie)
{
	int status;

	liberarClaves();

	status = CL->importCert(serie.cadena(), serie.longitud(), CRYPT_UNUSED, &clavePublica);
	if ( CL->isOK(status) )
	{
		// se importa el certificado, pero sin clave privada
		loadData();
	}
	else
	{
		clavePublica = 0;
		clavePrivada = 0;
	}

	return (clavePublica != 0);
}


char* CertCL::getParametrosBuscado(CL_KEYID_TYPE *tipo)
{
	char* clave;

	// las aserciones también se pueden (y deben) poner en el código para
	// detectar situaciones anómalas.
	assert(tipo != NULL);

	switch (paramsBusqueda)
	{
		case BuscadoPorEtiqueta:
		{
			clave = claveBusqueda.cadena();
			*tipo = CL_KEYID_NAME;
			break;
		}
		case BuscadoPorPosicion:
		{
			clave = NULL;
			*tipo = CL_KEYID_FIRST;
			break;
		}
		case BuscadoPorNada:
		{
			clave = NULL;
			*tipo = CL_KEYID_NONE;
			break;
		}
	}

	return (clave);
}


void CertCL::setParametrosBuscado(CertCL::ParametrosBusqueda params, char* clave)
{
	paramsBusqueda = params;

	if (clave == NULL)
		claveBusqueda = "";
	else
		claveBusqueda = clave;
}


ArsCadena CertCL::getCadenaAtributo(CL_ATTRIBUTE_TYPE tipo, bool codificar)
{
	const int 	BUFF_SIZE = 512;

	char 		buff[BUFF_SIZE];
	ArsCadena 	str;
	int			status;
	int			len;

	memset(buff, 0, BUFF_SIZE);
	len = BUFF_SIZE;

	switch (tipo)
	{
		case CL_CERTINFO_ISSUERUNIQUEID:
		{
			// Por motivos de seguridad, cryptLib no permite obtener el Id el
			// emisor, así que lo componemos a través de su nombre codificado.
			tipo = CL_CERTINFO_ISSUERNAME;
			codificar = true;

			// ¡no break!
		}
		case CL_CERTINFO_ISSUERNAME:
		{

			// algunos atributos se obtiene de una forma especial
			status = CL->setAttribute(clavePublica, tipo, CRYPT_UNUSED);
			if ( CL->isOK(status) )
			{
				len = 0;
				status = getCommonName(buff, &len);
			}
			break;
		}
		default:
		{
			status = CL->getAttributeString(clavePublica, tipo, buff, &len);
			break;
		}
	}

	if ( CL->isOK(status) )
	{
		if (codificar)
			str = ConversorDatos::ByteToStr(buff, len);
		else
			str = buff;
	}

	return (str);
}


int CertCL::getCommonName(char* buff, int* len)
{
	int status;

	assert(buff != NULL);
	assert(len  != NULL);

	if (CL->isOK(CL->getAttributeString(clavePublica, CL_CERTINFO_COMMONNAME, buff, len)) ||
		CL->isOK(CL->getAttributeString(clavePublica, CL_CERTINFO_ORGANIZATIONNAME, buff, len)) ||
		CL->isOK(CL->getAttributeString(clavePublica, CL_CERTINFO_ORGANIZATIONALUNITNAME, buff, len)) ||
		CL->isOK(CL->getAttributeString(clavePublica, CL_CERTINFO_RFC822NAME, buff, len)) ||
		CL->isOK(CL->getAttributeString(clavePublica, CL_CERTINFO_DNSNAME, buff, len)) ||
		CL->isOK(CL->getAttributeString(clavePublica, CL_CERTINFO_UNIFORMRESOURCEIDENTIFIER, buff, len)))
	{
		status = CRYPT_OK;
		buff[*len] = '\0';
	}
	else
	{
		// no se ha podido recuperar
		status = CRYPT_ERROR_FAILED;
		*len = 0;
	}

	return (status);
}


void CertCL::liberarClaves(void)
{
	if (clavePublica != 0)
	{
		CL->destroyCert(clavePublica);
		clavePublica = 0;
	}

	if (clavePrivada != 0)
	{
		CL->destroyCert(clavePrivada);
		clavePrivada = 0;
	}
}


CL_CERTIFICATE CertCL::duplicarClavePublica(const CL_CERTIFICATE origen)
{
	// crpytLib no permite duplicar handles, sino que la única opción es exportar
	// y volver a importar el certificado.
	// El inconveniente es que la exportación/importación no duplica la clave privada,
	// sino solo la pública
	LPBYTE		datos;
	int			len;
	int			status;
	CL_CERTIFICATE	copia;

	len = SIZE_CERTIFICADO_EXPORTADO;
	datos = new BYTE[len];
	memset(datos, 0, len);

	// exportar clave pública
	status = CL->exportCert(datos, &len, CL_CERTFORMAT_CERTIFICATE, origen);
	if ( CL->isOK(status) )
	{
		// volver a importarla (para duplicar el handle)
		status = CL->importCert(datos, len, CRYPT_UNUSED, &copia);
		if ( !CL->isOK(status) )
			copia = 0;
	}

	delete [] datos;

	return (copia);
}

