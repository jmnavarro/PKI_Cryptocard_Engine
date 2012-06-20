#include "../h/AlmacenCertificadoBuilder.h"

#include "../h/AlmacenCertificadoCL.h"
#include "../h/CertCL.h"

#include "../h/AlmacenCertificadoCAPI.h"
#include "../h/CertCAPI.h"

#include "../../LibUtils/h/ArsCadena.h"

#include <pshpack8.h>
	#include <new>
#include <poppack.h>


AlmacenCertificadoBuilder::AlmacenCertificadoBuilder()
{
}


AlmacenCertificado* AlmacenCertificadoBuilder::Crear(const char *_nombreToolkit, const char *_nombreAlmacen)
{
	AlmacenCertificado  *ret;
	bool ok;
	TipoToolkit tipo;
	ArsCadena nombreToolkit = _nombreToolkit;
	ArsCadena nombreAlmacen = _nombreAlmacen;

	ok = true;
	if (nombreToolkit == NOMBRE_TOOLKIT_CRYPTLIB)
		tipo = ToolkitCryptLib;
	else if (nombreToolkit == NOMBRE_TOOLKIT_CAPI)
		tipo = ToolkitCAPI;
	else
		ok = false;

	if (ok)
	{
		try	{
			ret = Crear(tipo, _nombreAlmacen);
		}
		catch (std::bad_alloc)
		{
			ret = NULL;
		}
	}
	else
		ret = NULL;

	return (ret);
}


AlmacenCertificado* AlmacenCertificadoBuilder::Crear(AlmacenCertificadoBuilder::TipoToolkit tipoToolkit, const char *_nombreAlmacen)
{
	AlmacenCertificado *ret;
	ArsCadena nombreAlmacen = _nombreAlmacen;

	ret = NULL;
	try
	{
		switch (tipoToolkit)
		{
			case ToolkitCryptLib:
			{
				ret = new AlmacenCertificadoCL(nombreAlmacen);
				break;
			}

			case ToolkitCAPI:
			{
				ret = new AlmacenCertificadoCAPI(nombreAlmacen);
				break;
			}
		}
	}
	catch (std::bad_alloc)
	{
		ret = NULL;
	}

	return (ret);
}



Certificado* AlmacenCertificadoBuilder::CrearCertificado(const char *_nombreToolkit)
{
	Certificado  *ret;
	TipoToolkit  tipo;
	bool ok;
	ArsCadena nombreToolkit = _nombreToolkit;

	ok = true;
	if (nombreToolkit == NOMBRE_TOOLKIT_CRYPTLIB)
		tipo = ToolkitCryptLib;
	else if (nombreToolkit == NOMBRE_TOOLKIT_CAPI)
		tipo = ToolkitCAPI;
	else
		ok = false;

	if (ok)
	{
		try {
			ret = CrearCertificado(tipo);
		}
		catch (std::bad_alloc) {
			ret = NULL;
		}
	}
	else
		ret = NULL;

	return (ret);
}


Certificado* AlmacenCertificadoBuilder::CrearCertificado(AlmacenCertificadoBuilder::TipoToolkit tipoToolkit)
{
	Certificado *ret;

	ret = NULL;
	try
	{
		switch (tipoToolkit)
		{
			case ToolkitCryptLib:
			{
				ret = new CertCL();
				break;
			}

			case ToolkitCAPI:
			{
				ret = new CertCAPI();
				break;
			}
		}
	}
	catch (std::bad_alloc)
	{
		ret = NULL;
	}

	return (ret);
}


void AlmacenCertificadoBuilder::Liberar(AlmacenCertificado* &obj)
{
	if (obj != NULL)
	{
		delete obj;
		obj = NULL;
	}
}

void AlmacenCertificadoBuilder::Liberar(Certificado* &obj)
{
	if (obj != NULL)
	{
		delete obj;
		obj = NULL;
	}
}

