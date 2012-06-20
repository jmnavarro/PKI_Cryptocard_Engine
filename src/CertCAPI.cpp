#include "../h/CertCAPI.h"

#include "../h/ConversorDatos.h"


CertCAPI::CertCAPI()
	: Certificado()
{
	this->handle = 0;
	resetData();
}


CertCAPI::CertCAPI(PCCERT_CONTEXT  handle)
	: Certificado()
{
	this->handle = handle;
	loadData();
}


CertCAPI::CertCAPI(const ArsCadena &encoded)
	: Certificado()
{
	// ojo: en vez de llamar al constructor padre con el parámetro "encoded", se
	// llama al constructor por defecto, y después se llama al método "deserializar",
	// que es en realidad lo que haría el constructor padre de este.
	// Esto se hace así para evitar la restricción del C++ a la hora de llamar a métodos
	// abstractos en el constructor.
	this->handle = 0;

	deserializar(encoded);
}


CertCAPI::CertCAPI(CertCAPI *copy)
	: Certificado(copy)
{
	// hay que duplicar el handle de CAPI, porque si no, tendremos dos objetos CertCAPI
	// apuntando hacia el mismo handle
	this->handle = CertDuplicateCertificateContext(copy->handle);
}


CertCAPI::~CertCAPI()
{
	if (handle != NULL)
		CertFreeCertificateContext(handle);
}


Certificado* CertCAPI::clone()
{
	return ( new CertCAPI(this) );
}


PCCERT_CONTEXT CertCAPI::getHandle() const
{
	return (handle);
}


PCERT_INFO CertCAPI::getInfo() const
{
	return (handle != NULL) ? handle->pCertInfo : NULL;
}


void CertCAPI::setHandle(PCCERT_CONTEXT value)
{
	handle = value;
	loadData();
}


void CertCAPI::loadData()
{
	if (handle != NULL)
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


void CertCAPI::loadFechas()
{
	SYSTEMTIME	tmp;

	FileTimeToSystemTime(&getInfo()->NotBefore, &tmp);
	setFechaDesde(&tmp);

	FileTimeToSystemTime(&getInfo()->NotAfter, &tmp);
	setFechaHasta(&tmp);
}


void CertCAPI::loadNombre()
{
	const int BUFF_SIZE = 128;
	char buff[BUFF_SIZE];

	if (CertGetNameString(handle, CERT_NAME_SIMPLE_DISPLAY_TYPE,
							0, NULL, buff, BUFF_SIZE))
	{
		setNombre(buff);
	}
	else
		setNombre("");
}


void CertCAPI::loadEmisor()
{
	ArsCadena str;

	str = ConversorDatos::ByteToStr(getInfo()->Issuer.pbData,
									getInfo()->Issuer.cbData);

	setEmisor(str);
}


void CertCAPI::loadNombreEmisor()
{
	const int BUFF_SIZE = 128;
	char buff[BUFF_SIZE];

	if (CertGetNameString(handle, CERT_NAME_SIMPLE_DISPLAY_TYPE,
							CERT_NAME_ISSUER_FLAG, NULL, buff, BUFF_SIZE))
	{
		setNombreEmisor(buff);
	}
	else
		setNombreEmisor("");
}


void CertCAPI::loadNumeroSerie()
{
	ArsCadena str;

	str = ConversorDatos::ByteToStr(getInfo()->SerialNumber.pbData,
									getInfo()->SerialNumber.cbData);

	setNumeroSerie(str);
}


void CertCAPI::loadKeyContainer()
{
	CRYPT_KEY_PROV_INFO	*keyInfo;
	DWORD size;
	char  *aux;

	size = sizeof(CRYPT_KEY_PROV_INFO);

	if ( CertGetCertificateContextProperty(handle, CERT_KEY_PROV_INFO_PROP_ID, NULL, &size) )
	{
		keyInfo = (CRYPT_KEY_PROV_INFO *) new BYTE[size];

		if ( CertGetCertificateContextProperty(handle, CERT_KEY_PROV_INFO_PROP_ID, keyInfo, &size) )
		{
			aux = ConversorDatos::ConvertirUnicodeACadena(keyInfo->pwszContainerName);
			if (aux != NULL)
			{
				setKeyContainer(aux);
				delete [] aux;
			}
			else
				setKeyContainer("");
		}
		delete [] keyInfo;
	}
}



ArsCadena CertCAPI::serializar()
{
	ArsCadena  str;

	if (handle != NULL)
		str = ConversorDatos::ByteToStr(handle->pbCertEncoded, handle->cbCertEncoded);
	else
		str = "";

	return (str);
}


bool CertCAPI::deserializar(const ArsCadena &serie)
{
	LPBYTE	datos;
	DWORD	lenDatos;

	if (handle != NULL)
	{
		CertFreeCertificateContext(handle);
		handle = NULL;
	}

	lenDatos = serie.longitud() / 2;
	datos = new BYTE[lenDatos];

	ConversorDatos::StrToByte(serie, datos, lenDatos);

	handle = CertCreateCertificateContext(TIPO_CODIFICACION, datos, lenDatos);

	delete [] datos;

	loadData();

	return (handle != NULL);
}


ArsCadena CertCAPI::GetNombreCSP(PCCERT_CONTEXT cert)
{
	CRYPT_KEY_PROV_INFO	*keyInfo;
	DWORD size;
	char  *aux;
	ArsCadena ret;

	size = sizeof(CRYPT_KEY_PROV_INFO);

	if ( CertGetCertificateContextProperty(cert, CERT_KEY_PROV_INFO_PROP_ID, NULL, &size) )
	{
		keyInfo = (CRYPT_KEY_PROV_INFO *) new BYTE[size];

		if ( CertGetCertificateContextProperty(cert, CERT_KEY_PROV_INFO_PROP_ID, keyInfo, &size) )
		{
			aux = ConversorDatos::ConvertirUnicodeACadena(keyInfo->pwszProvName);
			if (aux != NULL)
			{
				ret = aux;
				delete [] aux;
			}
		}
		delete [] keyInfo;
	}

	return (ret);
}


