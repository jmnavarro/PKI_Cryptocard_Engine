#include "../h/TarjetaCAPI.h"

#include "../h/AlmacenCertificadoCAPI.h"
#include "../h/FirmadorCAPI.h"
#include "../h/CertCAPI.h"


#define TIPO_PIN	PP_KEYEXCHANGE_PIN


TarjetaCAPI::TarjetaCAPI(const ArsCadena &_nombre)
	: Tarjeta(_nombre)
{
	handle = 0;
}

TarjetaCAPI::~TarjetaCAPI()
{
}


bool TarjetaCAPI::conectar(Certificado *cert /*=NULL*/)
{
	bool  ok;
	char  *keyContainer;
	ArsCadena	aux;

	if (cert == NULL)
	{
		keyContainer = NULL;
	}
	else
	{
		aux = cert->getKeyContainer();
		keyContainer = aux.cadena();
	}

	ok = CryptAcquireContext(&handle, keyContainer, getNombre().cadena(),
							PROV_RSA_FULL, CRYPT_SILENT);

	return (ok);
}


bool TarjetaCAPI::desconectar()
{
	bool ok;

	if (handle != 0)
	{
		ok = CryptReleaseContext(handle, 0);
		if (ok)
			handle = 0;
	}
	else
		ok = true;
		
	return (ok);
}


bool TarjetaCAPI::isConectado() const
{
	return (handle != 0);
}


AlmacenCertificado* TarjetaCAPI::abrirAlmacen(const ArsCadena &nombre)
{
	AlmacenCertificado*  ret;

	// se permite abrir el almacén aunque la tarjeta esté cerrada
	ret = new AlmacenCertificadoCAPI(nombre, handle);

	if ( !ret->open() )
	{
		delete ret;
		ret = NULL;
	}

	return (ret);
}


bool TarjetaCAPI::cerrarAlmacen(AlmacenCertificado* almacen)
{
	if ( almacen->close() )
	{
		delete almacen;
		almacen = NULL;
	}

	return (almacen == NULL);
}


bool TarjetaCAPI::establecerPIN(const ArsCadena &pin, ArsCadena &msg)
{
	bool ok;

	ok = isConectado();
	if (ok)
	{
		// por defecto, la mayoría los los CSP hacen la comprobación del PIN en la
		// llamada al CryptSetProvParam (al menos el CSP de Siemens y el de Starcos)
		// Sin embargo, hay otros CSP (como el de Ceres) que utilizan otro sistema
		// para comprobar el PIN.
		// En la especialización de "TarjetaCAPI" correspondiente, se debe codificar
		// la comprobación con el método necesario
		ok = CryptSetProvParam(getHandle(), TIPO_PIN, pin.cadena(), 0);
		if (ok)
			msg = "El PIN es correcto.";
		else
			msg = GetMensajeErrorEstablecerPIN(GetLastError());
	}
	else
		msg = "No se puede establecer el PIN si la tarjeta está cerrada.";

	return (ok);
}


ArsCadena TarjetaCAPI::GetMensajeErrorEstablecerPIN(int codigo) const
{
	ArsCadena	ret;
	char		*buffMsg;
	DWORD		error;

	error = (DWORD) codigo;

	switch (error)
	{
		case NTE_FAIL:
		{
			// error interno: posiblemente no esté la tarjeta
			ret = "Error estableciendo el PIN: posiblemente no esté disponible la tarjeta.";
			break;
		}

		case SCARD_W_WRONG_CHV:
		{
			// The card cannot be accessed because the wrong PIN was presented.
			ret = "El PIN que ha introducido no es correcto.";
			break;
		}

		case SCARD_W_CHV_BLOCKED:
		{
			// The card cannot be accessed because the maximum number of PIN entry attempts has been reached.
			ret = "No es posible validar el PIN debido a que la tarjeta está bloqueada.";
			break;
		}

		default:
		{
			if ( !FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, error,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(char *) &buffMsg, 0, NULL ) )
			{
				ret = "Error genérico estableciendo el PIN.";
			}
			else
			{
				ret = buffMsg;
				LocalFree(buffMsg);
			}
			break;
		}
	}

	return (ret);
}


Firmador* TarjetaCAPI::createFirmador()
{
	return new FirmadorCAPI(handle);
}


HCRYPTPROV TarjetaCAPI::getHandle() const
{
	return handle;
}


ArsCadena TarjetaCAPI::getEtiqueta() const
{
	char  buff[256];
	DWORD len = 256;
	ArsCadena ret;

	if ( isConectado() )
	{
		if ( CryptGetProvParam(getHandle(), PP_NAME, buff, &len, 0) )
			ret = buff;
	}

	return (ret);
}

