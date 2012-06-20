#include "../h/TarjetaCL.h"

#include "../h/AlmacenCertificadoCL.h"
#include "../h/FirmadorCL.h"

#define lengthof(x)	(sizeof(x)/sizeof(x[0]))


TarjetaCL::TarjetaCL(const ArsCadena &rutaDLL, const ArsCadena &nombreDispositivo)
	: Tarjeta(nombreDispositivo)
{
	lastError = CRYPT_OK;
	handle = 0;
	rutaDriver = rutaDLL;
}

TarjetaCL::~TarjetaCL()
{
	desconectar();
}


bool TarjetaCL::conectar(Certificado *cert /*=NULL*/)
{
	// en cryptLib no tiene sentido conectar a una tarjeta a partir del certificado, por
	// lo que ignoramos el parámetro
	bool  ret;
	int   status;
	ArsCadena nombreDispositivo;

	if ( !isConectado() )
	{
		ret = configurarDriver(getRutaDriver());
		if (ret)
		{
			nombreDispositivo = getNombreDispositivo();
			status = CL->deviceOpen(&handle, CRYPT_UNUSED,
									CL_DEVICE_PKCS11,
									nombreDispositivo.cadena() );

			ret = CL->isOK(status);
			if (!ret)
			{
				// si hay un cambio de tarjeta, se produce un CRYPT_ERROR_SIGNALLED.
				// Se soluciona abriendo y cerrando de nuevo el entorno.
				if (status == CRYPT_ERROR_SIGNALLED)
				{
					ret = reiniciarConexion();
					if (!ret)
						handle = 0;
				}
				else
					handle = 0;
			}
		}
	}
	else
		ret = true;

	return (ret);
}


bool TarjetaCL::desconectar()
{
	bool  ok;
	int   ret;

	if ( isConectado() )
	{
		ret = CL->deviceClose(handle);
		ok = CL->isOK(ret);
		if (ok)
			handle = 0;
	}
	else
		ok = true;

	return (ok);
}


bool TarjetaCL::isConectado() const
{
	return (handle != 0);
}


bool TarjetaCL::reiniciarConexion()
{
	int status;
	bool ret;


	CL->end();
	status = CL->init();
	ret = CL->isOK(status);
	if (ret)
	{
		status = CL->deviceOpen(&handle, CRYPT_UNUSED,
								CL_DEVICE_PKCS11,
								getNombreDispositivo().cadena() );
		ret = CL->isOK(status);
	}
	else
	{
		// tengo que liberar el singleton de cryptLib para que en la
		// próxima llamada intente hacer el "init".
		cryptLib::freeInstancia(false);
	}


    return (ret);
}


AlmacenCertificado* TarjetaCL::abrirAlmacen(const ArsCadena &nombre /*=""*/)
{
	AlmacenCertificado*  ret;

	// NO se permite abrir el almacén cuando la tarjeta está cerrada
	if ( isConectado() )
	{
		ret = new AlmacenCertificadoCL("", handle);

		if ( !ret->open() )
		{
			delete ret;
			ret = NULL;
		}
	}
	else
		ret = NULL;

	return (ret);
}


bool TarjetaCL::cerrarAlmacen(AlmacenCertificado* almacen)
{
	if ( almacen->close() )
	{
		delete almacen;
		almacen = NULL;
	}

	return (almacen == NULL);
}


bool TarjetaCL::establecerPIN(const ArsCadena &pin, ArsCadena &msg)
{
	bool ok;
	int ret;

	ok = false;

	if ( isConectado() )
	{
		ok = comprobarPIN(pin);
		if (!ok)
			msg = GetMensajeErrorEstablecerPIN(CRYPT_OK);
	}
	else
		msg = "No se puede establecer el PIN si la tarjeta está cerrada.";

	return (ok);
}


ArsCadena TarjetaCL::GetLastMensajeError() const
{
	return CL->getErrorString(lastError);
}


ArsCadena TarjetaCL::GetMensajeErrorEstablecerPIN(int codigo) const
{
	ArsCadena ret;

	// si pasa error ok, se obtiene el mensaje del último error.
	if (codigo == CRYPT_OK)
		codigo = lastError;

	switch (codigo)
	{
		case CRYPT_ERROR_WRONGKEY:
		case CRYPT_ERROR_PARAM4:
		{
			ret = "El PIN que ha introducido no es correcto.";
			break;
		}
		case CRYPT_ERROR_SIGNALLED:
		{
			// bug #4226
			ret = "Se ha detectado que ha cambiado la tarjeta.\r\n\r\nIntroduzca la tarjeta original.";
			break;
		}
		default:
		{
			ret = CL->getErrorString(codigo);
			break;
		}
	}

	return (ret);
}


bool TarjetaCL::comprobarPIN(const ArsCadena &pin)
{
	int ret;
	bool ok;

	ok = false;

	if ( isConectado() )
	{
#define ESTABLECER_SOLO_PIN_DE_USUARIO

#if defined(ESTABLECER_SOLO_PIN_DE_USUARIO)
		// Establezco el nuevo pin.
		ret = CL->setAttributeString(handle, CL_DEVINFO_AUTHENT_USER, pin.cadena(), pin.longitud() );
		ok = CL->isOK(ret);
		if (!ok)
			lastError = ret;
#else
		const CL_ATTRIBUTE_TYPE PINES[2] =
			{ CL_DEVINFO_AUTHENT_SUPERVISOR, CL_DEVINFO_AUTHENT_USER };

		// Primero se establece el pin de administración, y después el de usuario
		ok = true;
		for (int iPin = 0; iPin < 2 && ok; iPin++)
		{
			ret = CL->setAttributeString(handle, PINES[iPin], pin.cadena(), pin.longitud() );
			ok = CL->isOK(ret);
			if (!ok)
				lastError = ret;
		}
#endif
	}

	return (ok);
}


Firmador* TarjetaCL::createFirmador()
{
	return new FirmadorCL(handle);
}


CL_DEVICE TarjetaCL::getHandle() const
{
	return (handle);
}


ArsCadena TarjetaCL::getRutaDriver() const
{
	return (rutaDriver);
}


ArsCadena TarjetaCL::getNombreDispositivo() const
{
	return getNombre();
}


bool TarjetaCL::configurarDriver(const ArsCadena &ruta) const
{
	// primero hay que mirar en que número de dispositivo está almacenado,
	// y si no está en ninguno, guardarlo en el primero

	int			ret;
	int 		len;
	bool		ok;
	char*		nombreDriver;
	ArsCadena	aux;
	bool		encontrado;
	CL_ATTRIBUTE_TYPE	devices[] = {CL_OPTION_DEVICE_PKCS11_DVR01,
		CL_OPTION_DEVICE_PKCS11_DVR02, CL_OPTION_DEVICE_PKCS11_DVR03,
		CL_OPTION_DEVICE_PKCS11_DVR04, CL_OPTION_DEVICE_PKCS11_DVR05};

	ok = true;
	encontrado = false;
	for (int i=0; i<lengthof(devices) && !encontrado && ok; i++)
	{
		len = 0;
		ret = CL->getAttributeString(CRYPT_UNUSED, devices[i], NULL, &len);
		ok = CL->isOK(ret);
		if (ok)
		{
			nombreDriver = new char[len+1];
			nombreDriver[len] = '\0';

			ret = CL->getAttributeString(CRYPT_UNUSED, devices[i], nombreDriver, &len);
			ok = CL->isOK(ret);
			if (ok)
			{
				aux = nombreDriver;
				encontrado = (aux == ruta);
			}

			delete [] nombreDriver;
		}
		else if (ret == CRYPT_ERROR_NOTFOUND)
			ok = true;
	}

	if (ok && !encontrado)
	{
		// Se almacena el nuevo driver en el 5º slot (el último)
		// De este modo se minimiza la posibilidad de sobrescribir la configuración que pudiera existir
		ret = CL->setAttributeString(CRYPT_UNUSED, CL_OPTION_DEVICE_PKCS11_DVR05, ruta.cadena(), ruta.longitud());
		ok = CL->isOK(ret);
		if (ok)
		{
			ret = CL->setAttribute(CRYPT_UNUSED, CL_OPTION_CONFIGCHANGED, FALSE);
			ok = CL->isOK(ret);
		}
	}

	return (ok);
}


ArsCadena TarjetaCL::getEtiqueta() const
{
	char label[CRYPT_MAX_TEXTSIZE];
	int labelLength;
	int status;
	ArsCadena ret;

	if ( isConectado() )
	{
		status = CL->getAttributeString(handle, CL_DEVINFO_LABEL, label, &labelLength);
		if ( CL->isOK(status) )
		{
			label[labelLength] = '\0';
			ret = label;
		}
	}

	return (ret);
}
