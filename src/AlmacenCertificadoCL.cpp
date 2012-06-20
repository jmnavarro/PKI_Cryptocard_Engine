#include "../h/AlmacenCertificadoCL.h"

#include "../h/CertCL.h"
#include "../h/ValidacionCertificadoCL.h"

#include <assert>


#define HANDLE_ALMACEN_CERRADO			(CL_KEYSET) -1
#define HANDLE_ALMACEN_TARJETA_ABIERTO	(CL_KEYSET) -2


AlmacenCertificadoCL::AlmacenCertificadoCL(const ArsCadena &nombre, const CL_DEVICE &_dev /*=0*/)
	: AlmacenCertificado(nombre)
{
	dev = _dev;
	handle = HANDLE_ALMACEN_CERRADO;
}

AlmacenCertificadoCL::~AlmacenCertificadoCL()
{
	close();
}


char* AlmacenCertificadoCL::calcularParametrosBusqueda(ClaveBusqueda claveBusqueda, void *data, CL_KEYID_TYPE *keyIDType)
{
	ArsCadena	*aux;
	char		*keyID;

	assert(keyIDType != NULL);

	switch (claveBusqueda)
	{
		case ClaveBusquedaNombre:
		{
			assert(data != NULL);
			
			aux = (ArsCadena*) data;

			*keyIDType = CL_KEYID_NAME;
			keyID = aux->cadena();
			break;
		}
		case ClaveBusquedaEMail:
		{
			assert(data != NULL);

			aux = (ArsCadena*) data;

			*keyIDType = CL_KEYID_EMAIL;
			keyID = aux->cadena();
			break;
		}
		case ClaveBusquedaPrimero:
		{
			*keyIDType = CL_KEYID_FIRST;
			keyID = NULL;
			break;
		}
		default:
		{
			*keyIDType = CL_KEYID_NONE;
			keyID = NULL;
			break;
		}
	}

	return (keyID);
}


Certificado* AlmacenCertificadoCL::loadCertificado(ClaveBusqueda claveBusqueda, void *data)
{
	CertCL	  		*ret;
	CL_KEYID_TYPE	keyIDType;
	CL_CERTIFICATE	hPublica;
	CL_CERTIFICATE	hPrivada;
	int		  		status;
	char	  		*keyID;

	keyID = calcularParametrosBusqueda(claveBusqueda, data, &keyIDType);
	
	if (keyIDType != CL_KEYID_NONE)
	{
		// Permito que solo exista la clave pública.
		// En este caso se podrán realizar ciertas operaciones (como verificación de
		// firmas o mostrar los datos del certificado) pero no la operación de firma.
		status = CL->getPublicKey(dev, &hPublica, keyIDType, keyID);
		if ( cryptStatusOK(status) )
		{
			status = CL->getPrivateKey(dev, &hPrivada, keyIDType, keyID);
			if ( cryptStatusError(status) )
				hPrivada = 0;

			ret = crearCertificado(hPublica, hPrivada);
			if (ret != NULL)
				recordarParametrosBusqueda(ret, claveBusqueda, keyID);
		}
		else
			ret = NULL;
	}
	else
		ret = NULL;

	return (ret);
}


bool AlmacenCertificadoCL::reloadCertificado(CertCL* certificado)
{
	bool	ret;
	int		status;
	char	*keyID;
	CL_KEYID_TYPE	keyIDType;
	CL_CERTIFICATE	hPrivada;

	if ( !certificado->tieneClavePrivada() )
	{
		keyID = certificado->getParametrosBuscado(&keyIDType);

		ret = (keyIDType != CL_KEYID_NONE);
		if (ret)
		{
			status = CL->getPrivateKey(dev, &hPrivada, keyIDType, keyID);
			ret = cryptStatusOK(status);
			if (ret)
				certificado->setClavePrivada(hPrivada);
		}
	}
	else
		ret = true;

	return (ret);
}


ValidacionCertificado*  AlmacenCertificadoCL::createValidacionCertificado()
{
	return new ValidacionCertificadoCL(this);
}


Certificado* AlmacenCertificadoCL::enumCertificados(Certificado* anterior)
{
	Certificado *ret;

	if (anterior == NULL)
		ret = loadCertificado(ClaveBusquedaPrimero, NULL);
	else
	{
		delete anterior;
		ret = NULL;
	}

	return (ret);
}


int AlmacenCertificadoCL::getNumeroCertificados()
{
	int ret;

	if (handle == HANDLE_ALMACEN_CERRADO)
		ret = -1;
	else if ( esAlmacenEnTarjeta() )
		ret = hayCertificadosEnTarjeta() ? 1 : 0;
	else
		ret = (int) handle;

	return (ret);
}


bool AlmacenCertificadoCL::hayCertificadosEnTarjeta()
{
	CL_CERTIFICATE	hCert;
	int		status;
	bool	hayAlguno;

	hCert = 0;

	status = CL->getPublicKey(dev, &hCert, CL_KEYID_FIRST, NULL);
	hayAlguno = (cryptStatusOK(status) || status == CRYPT_ERROR_DUPLICATE);

	if (hCert != 0)
		CL->destroyCert(hCert);

	return (hayAlguno);
}


bool AlmacenCertificadoCL::esAlmacenEnTarjeta()
{
	return (getNombre() == "");
}


bool AlmacenCertificadoCL::open()
{
	int numeroCRLs;

	if ( esAlmacenEnTarjeta() )
		handle = HANDLE_ALMACEN_TARJETA_ABIERTO;
	else
	{
		numeroCRLs = cargarCRLs();
		if (numeroCRLs == -1)
			handle = 0;
		else
			handle = numeroCRLs;
	}

	return (true);
}


bool AlmacenCertificadoCL::close()
{
	if ( !esAlmacenEnTarjeta() )
		listaCRL.Vaciar();

	handle = HANDLE_ALMACEN_CERRADO;

	return (true);
}


bool AlmacenCertificadoCL::isCerrado()
{
	return (handle == HANDLE_ALMACEN_CERRADO);
}


void AlmacenCertificadoCL::recordarParametrosBusqueda(CertCL *certificado, ClaveBusqueda claveBusqueda, char* id)
{
	CertCL::ParametrosBusqueda buscadoPor;

	switch (claveBusqueda)
	{
		case ClaveBusquedaNombre:
		case ClaveBusquedaEMail:
		{
			buscadoPor = CertCL::BuscadoPorEtiqueta;
			break;
		}
		case ClaveBusquedaPrimero:
		{
			buscadoPor = CertCL::BuscadoPorPosicion;
			break;
		}
		default:
		{
			buscadoPor = CertCL::BuscadoPorNada;
			break;
		}
	}

	if (buscadoPor != CertCL::BuscadoPorNada)
		certificado->setParametrosBuscado(buscadoPor, id);
}


CertCL* AlmacenCertificadoCL::crearCertificado(const CL_CERTIFICATE clavePublica, const CL_CERTIFICATE clavePrivada)
{
	return ( new CertCL(clavePublica, clavePrivada) );
}


DWORD AlmacenCertificadoCL::eliminarCRL(const ArsCadena &nombreFichero)
{
	bool ret;
	ArsCadena rutaCRL;

	if ( esAlmacenEnTarjeta() || isCerrado() || handle == 0)
		return (false);

	rutaCRL = getRutaAlmacen() + extraerNombreFichero(nombreFichero);

	ret = DeleteFile(rutaCRL.cadena());
	if (ret)
		handle--;

	return (ret);
}


bool  AlmacenCertificadoCL::almacenarCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos)
{
	bool ret;

	if ( esAlmacenEnTarjeta() || isCerrado())
		return (false);

	if ( comprobarDatosCRL(datos, lenDatos) )
	{
		ret = copiarNuevaCRL(nombreFichero, datos, lenDatos);
		if (ret)
			handle++;
	}
	else
		ret = false;

	return (ret);
}


bool AlmacenCertificadoCL::comprobarDatosCRL(const void* datos, DWORD lenDatos)
{
	bool	ok;
	int		status;
	CL_CERTIFICATE	crl;

	status = CL->importCert((void*)datos, lenDatos, CRYPT_UNUSED, &crl);
	ok = cryptStatusOK(status);

	if (ok)
		CL->destroyCert(crl);

	return (ok);
}


bool AlmacenCertificadoCL::copiarNuevaCRL(const ArsCadena &nombreFichero, const void* datos, DWORD lenDatos)
{
	bool ret;
	ArsCadena rutaCRL;
	HANDLE h;
	DWORD escrito = 0;

	rutaCRL = getRutaAlmacen() + extraerNombreFichero(nombreFichero);

	ret = false;

	// copio los datos en la nueva ruta
	h = CreateFile(rutaCRL.cadena(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (h != INVALID_HANDLE_VALUE)
	{
		if (WriteFile(h, datos, lenDatos, &escrito, NULL) )
			ret = true;
		CloseHandle(h);
	}

	return (ret);
}

int AlmacenCertificadoCL::cargarCRLs()
{
	ArsCadena		mascara;
	ArsCadena		rutaBase;
	WIN32_FIND_DATA	data;
	HANDLE			hFind;
	int				total;
	bool			ok;

	listaCRL.Vaciar();

	rutaBase = getRutaAlmacen();
	mascara = rutaBase + "*.crl";

	hFind = FindFirstFile(mascara.cadena(), &data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		total = -1;
	}
	else
	{
		total = 0;
		do
		{
			mascara = rutaBase;
			mascara += data.cFileName;
			listaCRL.MeterAtras(mascara);
			total++;

			if ( !FindNextFile(hFind, &data) )
			{
				ok = false;
				if ( GetLastError() != ERROR_NO_MORE_FILES )
					total = -1;
			}
		} while (ok);
	}

	return (total);
}


ArsCadena AlmacenCertificadoCL::getRutaAlmacen()
{
	ArsCadena ret;

	if ( !esAlmacenEnTarjeta() )
	{
		ret = getNombre();
		if (ret[ret.longitud()-1] != '\\')
			ret += "\\";
	}

	return (ret);
}


ArsCadena AlmacenCertificadoCL::extraerNombreFichero(const ArsCadena &rutaFichero)
{
	ArsCadena nombre;
	ArsCadena aux;

	aux = rutaFichero;

	while (aux != "")
		aux.extraerToken('\\', nombre);

	return (nombre);
}


ArsCadena AlmacenCertificadoCL::getCRL(int index)
{
	ArsCadena ret;

	if (index < 0 || index >= listaCRL.GetNumero())
		ret = "";
	else
		ret = listaCRL[index];

	return (ret);
}

