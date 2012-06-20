#include "../h/TarjetaCAPIeT.h"


#define NOMBRE_TARJETA_ETOKEN	"eToken Base Cryptographic Provider"
//#define NOMBRE_ALMACEN_ETOKEN	"My\\eToken"
#define NOMBRE_ALMACEN_ETOKEN	"MY"

#define ALGORITMO_HASH			CALG_SHA1

#define TIPO_PIN				PP_KEYEXCHANGE_PIN	/*PP_SIGNATURE_PIN*/
#define TIPO_CLAVE				AT_KEYEXCHANGE		/*AT_SIGNATURE*/


TarjetaCAPIeT::TarjetaCAPIeT()
	: TarjetaCAPI(NOMBRE_TARJETA_ETOKEN)
{
}

TarjetaCAPIeT::~TarjetaCAPIeT()
{
}


AlmacenCertificado* TarjetaCAPIeT::abrirAlmacen(const ArsCadena &nombre)
{
	return TarjetaCAPI::abrirAlmacen(NOMBRE_ALMACEN_ETOKEN);
}


bool TarjetaCAPIeT::establecerPIN(const ArsCadena &pin, ArsCadena &msg)
{
	// en este caso sobrescribo el Template Method porque el tipo de PIN a utilizar
	// es distinto al definido en TarjetaCAPI
	bool ok = false;

	if ( isConectado() )
	{
		ok = comprobarPIN(pin);
		if (ok)
		{
			ok = CryptSetProvParam(getHandle(), TIPO_PIN, pin.cadena(), 0);
			if (!ok)
				msg = GetMensajeErrorEstablecerPIN(GetLastError());
		}
		else
			msg = GetMensajeErrorEstablecerPIN(GetLastError());
	}
	else
	{
		// llamo al padre para que recoja el mensaje
		ok = TarjetaCAPI::establecerPIN(pin, msg);
	}

	return (ok);
}


bool TarjetaCAPIeT::comprobarPIN(const ArsCadena &pin)
{
	// hay que hacer una firma (o cifrado) y si da el error NTE_SILENT_CONTEXT
	// es que el pin era incorrecto.
	HCRYPTHASH	hash;
	DWORD		size;
	DWORD		lastError;
	bool		ret;
	char		*oldPIN;

	ret = false;
	size = 0;
	lastError = 0;
	oldPIN = NULL;

	// me guardo el pin actual
	if ( CryptGetProvParam(getHandle(), TIPO_PIN, NULL, &size, 0) )
	{
		oldPIN = new char[size];

		CryptGetProvParam(getHandle(), TIPO_PIN, oldPIN, &size, 0);
	}

	// establezco el nuevo PIN
	CryptSetProvParam(getHandle(), TIPO_PIN, pin.cadena(), 0);


	// hago una firma ficticia
	if ( CryptCreateHash(getHandle(), ALGORITMO_HASH, 0, 0, &hash) )
	{
		BYTE datos[] = {0x12};

		ret = CryptHashData(hash, datos, 1, 0);
		if (ret)
		{
			ret = CryptSignHash(hash, TIPO_CLAVE, NULL, 0, NULL, &size);
			if (ret)
			{
				BYTE* dummy = new BYTE[size];

				ret = CryptSignHash(hash, TIPO_CLAVE, NULL, 0, dummy, &size);
				if (ret)
				{
					// el pin es correcto porque no ha necesitado mostrar la ventana del pin
				}
				else
				{
					// el pin es incorrecto porque ha intentado sacar la ventana de pin y no ha podido
					lastError = GetLastError();
				}

				delete [] dummy;
			}
			CryptDestroyHash(hash);
		}
	}


	// recupero el PIN anterior
	if (oldPIN != NULL)
	{
		if (oldPIN[0] == '\0')
			CryptSetProvParam(getHandle(), TIPO_PIN, NULL, 0);
		else
			CryptSetProvParam(getHandle(), TIPO_PIN, oldPIN, 0);

		delete [] oldPIN;
	}


	// se restablece el error para que TarjetaCAPI::GetMensajeErrorEstablecerPIN
	// recupere el mensaje de error producido
	if (lastError != 0)
		SetLastError(lastError);


	return (ret);
}
