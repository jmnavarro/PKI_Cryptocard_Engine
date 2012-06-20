#include "../h/TarjetaBuilder.h"

#include "../h/TarjetaCLCeres.h"
#include "../h/TarjetaCAPICeres.h"

#include "../h/TarjetaCAPISiemens.h"
#include "../h/TarjetaCLSiemens.h"

#include "../h/TarjetaCAPIStarcos.h"
#include "../h/TarjetaCLStarcos.h"

#include "../../../Base/LibUtilidades/h/Definiciones.h"

#if defined(_DEBUG)
#	include "../h/TarjetaCAPIeT.h"
#endif

#include "../../LibUtils/h/ArsCadena.h"

#include <pshpack8.h>
	#include <new>
#include <poppack.h>


TarjetaBuilder::TipoTarjeta* TarjetaBuilder::CrearListaTarjetasSoportadas(int &total)
{
	TipoTarjeta	tipos[] = {TarjetaCeres, TarjetaStarcos, TarjetaSiemens /*, TarjetaNueva*/};
	TipoTarjeta  *ret;

	total = sizeof(tipos)/sizeof(tipos[0]);
	NUEVO_ARRAY(ret, TarjetaBuilder::TipoTarjeta, total);
	if (ret)
	{
		for (int i = 0; i < total; i++)
			ret[i] = tipos[i];
	}

	return (ret);

}


Tarjeta* TarjetaBuilder::AutodetectarTarjeta(TipoTarjeta &tarjetaDetectada, TipoToolkit tipoToolkit)
{
	TipoTarjeta *soportadas;
	int total;
	Tarjeta *ret;

	soportadas = CrearListaTarjetasSoportadas(total);
	if (soportadas == NULL || total == 0)
		ret = NULL;
	else
		ret = AutodetectarTarjeta(soportadas, total, tarjetaDetectada, tipoToolkit);

	LiberarListaTarjetasSoportadas(soportadas);
	
	return (ret);
}


Tarjeta* TarjetaBuilder::AutodetectarTarjeta(TipoTarjeta tipos[], int numTipos, TipoTarjeta &tarjetaDetectada, TipoToolkit tipoToolkit)
{
	Tarjeta  *ret;

	ret = NULL;
	tarjetaDetectada = TarjetaDesconocida;

	for(int i = 0; ret == NULL && i < numTipos; i++)
	{
		if (tipos[i] != TarjetaDesconocida)
		{
			ret = CrearTarjeta(tipos[i], tipoToolkit);

			// si no conecta puede ser por:
			//		No tener instalado el driver
			//		No estar insertada la tarjeta en el lector
			if ( ret != NULL && !ret->conectar() )
			{
				delete ret;
				ret = NULL;
			}
			else
				tarjetaDetectada = tipos[i];
		}
	}

	return (ret);
}



Tarjeta* TarjetaBuilder::CrearTarjeta(const char *_nombreTarjeta, const char *_nombreToolkit)
{
	Tarjeta	*ret;

	ArsCadena nombreTarjeta;
	ArsCadena nombreToolkit;

	nombreTarjeta = _nombreTarjeta;
	nombreToolkit = _nombreToolkit;

	ret = NULL;

	try
	{
		if (nombreTarjeta == NOMBRE_TARJETA_CERES)
		{
			if (nombreToolkit == NOMBRE_TOOLKIT_CRYPTLIB)
			{
				ret = new TarjetaCLCeres();
			}
			else if (nombreToolkit == NOMBRE_TOOLKIT_CAPI)
			{
				ret = new TarjetaCAPICeres();
			}
		}
		else if (nombreTarjeta == NOMBRE_TARJETA_SIEMENS)
		{
			if (nombreToolkit == NOMBRE_TOOLKIT_CAPI)
			{
				ret = new TarjetaCAPISiemens();
			}
			else if (nombreToolkit == NOMBRE_TOOLKIT_CRYPTLIB)
			{
				ret = new TarjetaCLSiemens();
			}
		}
		else if (nombreTarjeta == NOMBRE_TARJETA_STARCOS)
		{
			if (nombreToolkit == NOMBRE_TOOLKIT_CAPI)
			{
				ret = new TarjetaCAPIStarcos();
			}
			else if (nombreToolkit == NOMBRE_TOOLKIT_CRYPTLIB)
			{
				ret = new TarjetaCLStarcos();
			}
		}
/*
		else if (nombreTarjeta == NOMBRE_TARJETA_NUEVA)
		{
			if (nombreToolkit == NOMBRE_TOOLKIT_CAPI)
			{
				ret = new TarjetaCAPINueva();
			}
			else if (nombreToolkit == NOMBRE_TOOLKIT_CRYPTLIB)
			{
				ret = new TarjetaCLNueva();
			}
		}
*/	
#if defined(_DEBUG)
		else if (nombreTarjeta == NOMBRE_TARJETA_ETOKEN)
		{
			//
			// Por ahora el eToken no se soporta
			//
			ret = NULL;
		}
#endif
	}
	catch (std::bad_alloc)
	{
		ret = NULL;
	}

	return (ret);
}


Tarjeta* TarjetaBuilder::CrearTarjeta(TarjetaBuilder::TipoTarjeta tipoTarjeta, TarjetaBuilder::TipoToolkit tipoToolkit)
{
	Tarjeta *ret;

	ret = NULL;
	try
	{
		switch (tipoTarjeta)
		{
			case TarjetaCeres:
			{
				switch(tipoToolkit)
				{
					case ToolkitCryptLib:
					{
						ret = new TarjetaCLCeres();
						break;
					}
					case TookitCAPI:
					{
						ret = new TarjetaCAPICeres();
						break;
					}
					default:
					{
						ret = NULL;
						break;
					}
				}
				break;
			}

			case TarjetaSiemens:
			{
				switch (tipoToolkit)
				{
					case TookitCAPI:
					{
						ret = new TarjetaCAPISiemens();
						break;
					}
					case ToolkitCryptLib:
					{
						ret = new TarjetaCLSiemens();
						break;
					}
				}
				break;
			}

			case TarjetaStarcos:
			{
				switch (tipoToolkit)
				{
					case TookitCAPI:
					{
						ret = new TarjetaCAPIStarcos();
						break;
					}
					case ToolkitCryptLib:
					{
						ret = new TarjetaCLStarcos();
						break;
					}
				}
				break;
			}
/*
			case TarjetaNueva:
			{
				switch (tipoToolkit)
				{
					case TookitCAPI:
					{
						ret = new TarjetaCAPINueva();
						break;
					}
					case ToolkitCryptLib:
					{
						ret = new TarjetaCLNueva();
						break;
					}
				}
				break;
			}
*/			

#if defined(_DEBUG)
			case TarjetaEToken:
			{
				// por ahora no se soporta
				ret = NULL;
				break;
			}
#endif
		}
	}
	catch (std::bad_alloc)
	{
		ret = NULL;
	}

	return (ret);
}


void TarjetaBuilder::Liberar(Tarjeta* &obj)
{
	if (obj != NULL)
	{
		delete obj;
		obj = NULL;
	}
}


const char* TarjetaBuilder::ConvertirTipoTarjeta(TipoTarjeta tipo)
{
	const char* ret;

	switch (tipo)
	{
		case TarjetaCeres:
		{
			ret = NOMBRE_TARJETA_CERES;
			break;
		}
		case TarjetaSiemens:
		{
			ret = NOMBRE_TARJETA_SIEMENS;
			break;
		}
		case TarjetaStarcos:
		{
			ret = NOMBRE_TARJETA_STARCOS;
			break;
		}
/*
		case TarjetaNueva:
		{
			ret = NOMBRE_TARJETA_NUEVA;
			break;
		}
*/
		default:
		{
			ret = NULL;
			break;
		}
	}

	return (ret);
}


TarjetaBuilder::TipoTarjeta TarjetaBuilder::ConvertirTipoTarjeta(const char* _tipo)
{
	ArsCadena tipo = _tipo;
	TipoTarjeta ret;

	if (tipo == NOMBRE_TARJETA_CERES)
		ret = TarjetaCeres;
	else if (tipo == NOMBRE_TARJETA_SIEMENS)
		ret = TarjetaSiemens;
	else if (tipo == NOMBRE_TARJETA_STARCOS)
		ret = TarjetaStarcos;
/*
	else if (tipo == NOMBRE_TARJETA_NUEVA)
		ret = TarjetaNueva;
*/
	else
		ret = TarjetaDesconocida;

	return (ret);
}


void TarjetaBuilder::LiberarListaTarjetasSoportadas(TipoTarjeta* &lista)
{
	BORRAR_ARRAY(lista);
}

