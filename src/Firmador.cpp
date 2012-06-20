#include "../h/Firmador.h"
#include "../h/FirmaDetached.h"


Firmador::Firmador()
{
}

Firmador::~Firmador()
{
}

//
// Métodos de firma
//
Firma* Firmador::firmar(TipoFirma tipo, const ArsCadena &data, const Certificado &cert)
{
	BufferDinamico	buffData(data.cadena(), data.longitud());

	return firmar(tipo, buffData, cert);
}


Firma* Firmador::firmar(TipoFirma tipo, const char *data, const Certificado &cert)
{
	if (data == NULL)
		return NULL;

	BufferDinamico	buffData(data, strlen(data));

	return firmar(tipo, buffData, cert);
}


Firma* Firmador::firmar(TipoFirma tipo, const LPBYTE data, DWORD size, const Certificado &cert)
{
	if (data == NULL || size == 0)
		return NULL;

	BufferDinamico	buffData(data, size);

	return firmar(tipo, buffData, cert);
}


Firma* Firmador::firmar(TipoFirma tipo, const BufferDinamico &data, const Certificado &cert)
{
	Firma	*firma;

	firma = this->crearFirma(tipo);
	if (firma != NULL)
	{
		if ( !this->calcularFirma(data, cert, *firma) )
		{
			if (firma)
			{
				delete firma;
				firma = NULL;
			}
		}
	}

	return (firma);
}



//
// Métodos de resumen
//
Resumen* Firmador::resumir(const ArsCadena &data)
{
	BufferDinamico	buffData(data.cadena(), data.longitud());

	return resumir(buffData);
}


Resumen* Firmador::resumir(const char *data)
{
	if (data == NULL)
		return NULL;

	BufferDinamico	buffData(data, strlen(data));

	return resumir(buffData);
}


Resumen* Firmador::resumir(const LPBYTE data, DWORD size)
{
	if (data == NULL || size == 0)
		return NULL;

	BufferDinamico	buffData(data, size);

	return resumir(buffData);
}


Resumen* Firmador::resumir(const BufferDinamico &data)
{
	BufferDinamico	buffHash;
	Resumen* ret = this->crearResumen();

	if (ret != NULL)
		if ( !this->calcularResumen(data, *ret) )
		{
			delete ret;
			ret = NULL;
		}

	return (ret);
}


//
// Métodos de validación
//
bool Firmador::validar(const char* data, const Firma &firma, const Certificado *cert)
{
	if (data == NULL)
		return (false);

	BufferDinamico	buffData(data, strlen(data));

	return validar(buffData, firma, cert);
}

bool Firmador::validar(const LPBYTE data, DWORD size, const Firma &firma, const Certificado *cert)
{
	if (data == NULL || size == 0)
		return (false);

	BufferDinamico	buffData(data, size);

	return validar(buffData, firma, cert);
}

bool Firmador::validar(const BufferDinamico &data, const Firma &firma, const Certificado *cert)
{
	bool ret;

	ret = validarFirma(data, firma, cert);

	return (ret);
}



//
// Métodos factory para crear una firma o resumen
//
Firma* Firmador::crearFirma(TipoFirma tipo)
{
	Firma *ret;

	// aquí debería discriminarse el tipo de firma a crear, dependiendo de los tipos de
	// firma implementados
	switch (tipo)
	{
		case TipoFirmaDetached:
			ret = new FirmaDetached();
			break;

		default:
			ret = NULL;
	}

	return (ret);
}


Resumen* Firmador::crearResumen()
{
	return ( new Resumen() );
}

