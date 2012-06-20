#include "../h/TarjetaCAPICeres.h"


#define NOMBRE_TARJETA_CERES	"CeresCSP"
#define NOMBRE_ALMACEN_CERES	"My\\Tarjeta Ceres"
#define ALGORITMO_HASH			CALG_SHA1
#define TIPO_PIN_CERES			PP_SIGNATURE_PIN


TarjetaCAPICeres::TarjetaCAPICeres()
	: TarjetaCAPI(NOMBRE_TARJETA_CERES)
{
}

TarjetaCAPICeres::~TarjetaCAPICeres()
{
}


AlmacenCertificado* TarjetaCAPICeres::abrirAlmacen(const ArsCadena &nombre)
{
	return TarjetaCAPI::abrirAlmacen(NOMBRE_ALMACEN_CERES);
}


bool TarjetaCAPICeres::establecerPIN(const ArsCadena &pin, ArsCadena &msg)
{
	bool ok;

	//
	// Para la tarjeta Ceres, la comprobación es mas compleja:
	// Dado que la función CryptSetProvParam no retorna si el PIN es correcto (sólo
	// lo establece), hay que hacer una firma ficticia para ver si se puede (si la
	// firma es correcta) o por el contrario el pin era incorrecta (no se puede
	// firmar).
	//
	ok = isConectado();
	if (ok)
	{
		// establezco el nuevo PIN
		CryptSetProvParam(getHandle(), TIPO_PIN_CERES, pin.cadena(), 0);

		ok = generarFirmaFicticia();
		if (ok)
			msg = "El PIN es correcto.";
		else
			msg = GetMensajeErrorEstablecerPIN(GetLastError());
	}
	else
		msg = "No se puede establecer el PIN si la tarjeta está cerrada.";

	return (ok);
}


bool TarjetaCAPICeres::generarFirmaFicticia()
{
	HCRYPTHASH	hash;
	DWORD		size;
	DWORD		lastError;
	BYTE 		datos[] = {0x12};
	bool		ret;

	size = 0;
	lastError = 0;

	ret = CryptCreateHash(getHandle(), ALGORITMO_HASH, 0, 0, &hash);
	if (ret)
	{
		ret = CryptHashData(hash, datos, 1, 0);
		if (ret)
		{
			ret = CryptSignHash(hash, AT_SIGNATURE, NULL, 0, NULL, &size);
			if (ret)
			{
				BYTE* dummy = new BYTE[size];

				ret = CryptSignHash(hash, AT_SIGNATURE, NULL, 0, dummy, &size);
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

	// se restablece el error para que TarjetaCAPI::GetMensajeErrorEstablecerPIN
	// recupere el mensaje de error producido
	if (lastError != 0)
		SetLastError(lastError);

	return (ret);
}
