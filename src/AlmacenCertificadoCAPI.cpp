#include "../h/AlmacenCertificadoCAPI.h"
#include "../h/CertCAPI.h"
#include "../h/ValidacionCertificadoCAPI.h"
#include "../h/ConversorDatos.h"


AlmacenCertificadoCAPI::AlmacenCertificadoCAPI(const ArsCadena &nombre, HCRYPTPROV  _csp)
	: AlmacenCertificado(nombre)
{
	csp = _csp;
	handle = NULL;
}


AlmacenCertificadoCAPI::~AlmacenCertificadoCAPI()
{
	close();
}


void* AlmacenCertificadoCAPI::calcularParametrosBusqueda(ClaveBusqueda claveBusqueda, void* datos, DWORD &tipo)
{
	void* parametros;
	NumeroSerie	*numSerie;
	int len;

	switch (claveBusqueda)
	{
		case ClaveBusquedaPrimero:
		{
			tipo = CERT_FIND_ANY;
			parametros = NULL;
			break;
		}

		case ClaveBusquedaNumSerie:
		{
			CERT_ID	*certID;
			NumeroSerie	*numSerie;

			// hay que rellenar una estructura CERT_ID con los parámetros de búsqueda
			// En este caso, la estructura se crea dinamicamente, por lo que debe ser
			// liberada por el que llame a esta función
			numSerie = reinterpret_cast<NumeroSerie*> (datos);

			certID = new CERT_ID;
			ZeroMemory(certID, sizeof(CERT_ID));
			certID->dwIdChoice = CERT_ID_ISSUER_SERIAL_NUMBER;

			len = numSerie->emisor.longitud() / 2;
			certID->IssuerSerialNumber.Issuer.cbData = len;
			certID->IssuerSerialNumber.Issuer.pbData = new BYTE[len];
			ConversorDatos::StrToByte(numSerie->emisor,
									certID->IssuerSerialNumber.Issuer.pbData, len);

			len = numSerie->numSerie.longitud() / 2;
			certID->IssuerSerialNumber.SerialNumber.cbData = len;
			certID->IssuerSerialNumber.SerialNumber.pbData = new BYTE[len];
			ConversorDatos::StrToByte(numSerie->numSerie,
									certID->IssuerSerialNumber.SerialNumber.pbData, len);

			tipo = CERT_FIND_CERT_ID;
			parametros = certID;
			break;
		}
		case ClaveBusquedaSubStrAsunto:
		{
			ArsCadena *dataStr;

			// el formato de las cadenas debe ser UNICODE, por lo que hay que
			// convertirlo.
			// La cadena convertida debe ser liberada por el que llame a este método
			dataStr = reinterpret_cast<ArsCadena*> (datos);

			tipo = CERT_FIND_SUBJECT_STR;
			parametros = ConversorDatos::ConvertirCadenaAUnicode (dataStr->cadena());
			break;
		}
		default:
		{
			// tipos no soportados para CAPI
			tipo = 0xFFFFFFFF;
			parametros = NULL;
			break;
		}
	}

	return (parametros);
}


void AlmacenCertificadoCAPI::liberarParametrosBusqueda(ClaveBusqueda claveBusqueda, void* parametros)
{
	// Dependiendo del tipo de búsqueda que hayamos hecho, hay que liberar unos tipos
	// de datos u otros.
	// En calcularParametrosBusqueda se ve que reservas de memoria se hacen por cada
	// tipo de búsqueda.

	switch (claveBusqueda)
	{
		case ClaveBusquedaSubStrAsunto:
		{
			wchar_t	*cadena;

			// liberamos la cadena unicode
			cadena = (wchar_t*)parametros;
			
			delete [] cadena;

			break;
		}

		case ClaveBusquedaNumSerie:
		{
			CERT_ID	*certID;

			// liberamos la estructura CERT_ID y sus buffers internos.
			certID = (CERT_ID*) parametros;

			delete [] certID->IssuerSerialNumber.Issuer.pbData;
			delete [] certID->IssuerSerialNumber.SerialNumber.pbData;
			delete certID;

			break;
		}
	}
}


Certificado* AlmacenCertificadoCAPI::loadCertificado(ClaveBusqueda claveBusqueda, void *data)
{
	Certificado		*ret;
	PCCERT_CONTEXT	cert;
	DWORD tipoBusqueda;
	void *parametros;
	bool ok;

	// Obtener el certificado.
	//
	// Para firmar, no vale un certificado cualquiera, sino uno que contenga una clave
	// privada. Si lo que queremos es validar una firma, con que tenga clave pública
	// (todos la tienen) es suficiente.
	//
	// La búsqueda del certificado se puede hacer por varios campos de los que aparecen
	// en la pestaña "Detalles" del certificado.
	//

	if (handle == NULL)
		return (NULL);

	ret = NULL;

	parametros = calcularParametrosBusqueda(claveBusqueda, data, tipoBusqueda);

	if (tipoBusqueda != 0xFFFFFFFF)
	{
		cert = CertFindCertificateInStore(handle, TIPO_CODIFICACION, 0,
								tipoBusqueda, parametros, NULL);

		// una vez terminada la búsqueda, podemos liberar sus parámetros, que desde
		// aquí no sabemos de qué tipo son y cuando ocupan
		liberarParametrosBusqueda(claveBusqueda, parametros);

		if (cert != NULL)
		{
			// comprobar si el certificado realmente corresponde al CSP (en caso de haber CSP)
			if (csp != 0)
				ok = CorrespondeCertificadoConCSP(cert);
			else
				ok = true;

			if (ok)
				ret = crearCertificado(cert);
		}
	}

	return (ret);
}


ValidacionCertificado*  AlmacenCertificadoCAPI::createValidacionCertificado()
{
	return new ValidacionCertificadoCAPI(this);
}


Certificado* AlmacenCertificadoCAPI::enumCertificados(Certificado* anterior)
{
	PCCERT_CONTEXT	hAnterior;
	PCCERT_CONTEXT	hSiguiente;
	CertCAPI		*certCAPI;
	bool continuar;

	if (handle == 0)
		return (NULL);

	continuar = false;
	do
	{
		if (anterior != NULL)
		{
			certCAPI  = dynamic_cast<CertCAPI*> (anterior);
			hAnterior = certCAPI->getHandle();
		}
		else if (!continuar)
		{
			certCAPI  = NULL;
			hAnterior = NULL;
		}

		hSiguiente = ::CertEnumCertificatesInStore(handle, hAnterior);

		// se libera el anterior, ya que la llamada a CertEnum... ha liberado su handle
		if (certCAPI != NULL)
		{
			certCAPI->setHandle(NULL);
			delete certCAPI;
			certCAPI = NULL;
		}

		// comprobar si corresponde al CSP actual
		if (hSiguiente != NULL && csp != 0)
			continuar = !CorrespondeCertificadoConCSP(hSiguiente);
		else
			continuar = false;

		if (continuar)
		{
			anterior = NULL;
			hAnterior = hSiguiente;
			hSiguiente = NULL;
			certCAPI = NULL;
		}
	}
	while (continuar);

	return ( hSiguiente != NULL ? new CertCAPI(hSiguiente) : NULL );
}


int AlmacenCertificadoCAPI::getNumeroCertificados()
{
	PCCERT_CONTEXT	hCertificado;
	int ret = 0;

	if (handle == 0)
		return (-1);

	hCertificado = NULL;
	
	do
	{
		hCertificado = ::CertEnumCertificatesInStore(handle, hCertificado);
		if (hCertificado != NULL)
		{
			if (CorrespondeCertificadoConCSP(hCertificado))
				ret++;
		}
	}
	while (hCertificado != NULL);

	return (ret);
}


HCERTSTORE AlmacenCertificadoCAPI::getHandle() const
{
	return (handle);
}


void AlmacenCertificadoCAPI::setCSP(HCRYPTPROV _csp)
{
	if (csp == 0)
		csp = _csp;
}


bool AlmacenCertificadoCAPI::EsNombreAlmacenSistema(const ArsCadena &nombre)
{
	bool esSistema;

	esSistema = (stricmp(nombre.cadena(), "CA") == 0  ||
				 stricmp(nombre.cadena(), "MY")  == 0 ||
				 stricmp(nombre.cadena(), "ROOT") == 0 ||
				 stricmp(nombre.cadena(), "SPC") == 0);

	return (esSistema);
}


bool AlmacenCertificadoCAPI::open()
{
	wchar_t *nombre;

	close();

	// ATENCION:
	// En caso de abrir almacenes desde Servicios en LocalSystem, habría que hacer
	// cambios en la llamada CertOpenStore, con las banderas:
	//		CERT_SYSTEM_STORE_SERVICES
	//		CERT_SYSTEM_STORE_CURRENT_SERVICE
	// Ver CertOpenStore en MSDN.

	if ( EsNombreAlmacenSistema(getNombre()) )
		handle = ::CertOpenSystemStore(csp, getNombre().cadena());
	else
	{
		nombre = ConversorDatos::ConvertirCadenaAUnicode(getNombre().cadena());

		handle = ::CertOpenStore(CERT_STORE_PROV_PHYSICAL, 0, csp,
								CERT_SYSTEM_STORE_CURRENT_USER | CERT_STORE_OPEN_EXISTING_FLAG | CERT_STORE_NO_CRYPT_RELEASE_FLAG,
								nombre);

		// hay que liberar la cadena Unicode porque "ConvertirCadenaAUnicode" lo ha creado
		delete [] nombre;
	}

	return (handle != NULL);
}


bool AlmacenCertificadoCAPI::close()
{
	bool ret;

	ret = true;

	if (handle)
	{
		ret = (bool) ::CertCloseStore(handle, 0);

		if (ret)
			handle = NULL;
	}

	return (ret);
}


Certificado* AlmacenCertificadoCAPI::crearCertificado(PCCERT_CONTEXT  hCert)
{
	return ( new CertCAPI(hCert) );
}


DWORD AlmacenCertificadoCAPI::eliminarCRL(const ArsCadena &emisor)
{
	DWORD 	ret;
	PCCRL_CONTEXT crl;
	LPBYTE	bytesEmisor, emisorActual;
	DWORD	lenBytesEmisor, lenEmisorActual;
	bool	error, terminado, esEmisorBuscado;

	if (handle != NULL && emisor != "")
	{
		// convierto el emisor
		lenBytesEmisor = emisor.longitud() / 2;
		bytesEmisor = new BYTE[lenBytesEmisor];
		ConversorDatos::StrToByte(emisor, bytesEmisor, lenBytesEmisor);

		// cargar y recorrer las CRLs disponibles dentro del almacén
		crl = NULL;
		ret = 0;
		error = false;
		do
		{
			crl = CertEnumCRLsInStore(handle, crl);
			terminado = (crl == NULL);

			if (!terminado)
			{
				emisorActual = crl->pCrlInfo->Issuer.pbData;
				lenEmisorActual = crl->pCrlInfo->Issuer.cbData;

				esEmisorBuscado  = lenEmisorActual == lenBytesEmisor;
				esEmisorBuscado &=  (0 == memcmp(emisorActual, bytesEmisor, lenBytesEmisor));

				if (esEmisorBuscado)
				{
					if (CertDeleteCRLFromStore(crl))
					{
						ret++;
						// para reiniciar la búsqueda.
						crl = NULL;
					}
					else
					{
						error = true;
						terminado = true;
					}
				}
			}
		}
		while (!terminado);

		if (error)
			ret = (DWORD) -2;

		delete [] bytesEmisor;
	}
	else
		ret = (DWORD) -1;

	return (ret);
}


bool AlmacenCertificadoCAPI::almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos)
{
	// el nombre del fichero no se usa

	bool ret;

	if (handle != NULL && datos != NULL && lenDatos > 0)
	{
		ret = (bool) CertAddEncodedCRLToStore(handle,
									TIPO_CODIFICACION,
									(BYTE*) datos, lenDatos,
									CERT_STORE_ADD_REPLACE_EXISTING, NULL);
	}
	else
		ret = false;

	return (ret);
}


bool AlmacenCertificadoCAPI::CorrespondeCertificadoConCSP(PCCERT_CONTEXT cert) const
{
	char buffNombreCSP[256];
	DWORD	len = 256;
	ArsCadena nombreCSP;
	ArsCadena cspCertificado;
	bool ok;

	if (csp != 0)
	{
		CryptGetProvParam(csp, PP_NAME, buffNombreCSP, &len, 0);
		nombreCSP = buffNombreCSP;

		cspCertificado = CertCAPI::GetNombreCSP(cert);

		ok = (cspCertificado == nombreCSP);
	}
	else
		ok = true;

	return (ok);
}


