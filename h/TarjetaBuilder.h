/**
 * Se implementa "creadora" de objetos "Tarjeta", siguiendo el patrón "Builder".
 * Con esta clase se consigue evitar el acoplamiento entre el código cliente y
 * las clases de implementación concreta (CAPI y cryptLib).
 *
 * @author JM
 *
 * @date 27/01/2005 - JM  @li Versión inicial
 * @date 18/02/2005 - JM  @li Se añade soporte para tarjetas Siemens (Generalitat de Valencia)
 * @date 04/03/2005 - JM  @li Se añade soporte para tarjetas Starcos (Camerfirma y ANCERT)
 * @date 11/05/2005 - JM  @li Se añade la autodetección de la tarjeta
**/
#if !defined(_TARJETA_BUILDER_H_)
#define _TARJETA_BUILDER_H_


class Tarjeta;


#define NOMBRE_TARJETA_CERES	"Ceres"
#define NOMBRE_TARJETA_SIEMENS	"Siemens"
#define NOMBRE_TARJETA_STARCOS	"Starcos"
/*
#define NOMBRE_TARJETA_NUEVA	"Nueva"
*/

#if defined(_DEBUG)
#	define NOMBRE_TARJETA_ETOKEN	"eToken"
#endif

#define NOMBRE_TOOLKIT_CRYPTLIB	"cryptLib"
#define NOMBRE_TOOLKIT_CAPI		"CryptoAPI"


class TarjetaBuilder
{
public:
	typedef enum
	{
		  TarjetaCeres
		, TarjetaSiemens
		, TarjetaStarcos
/*
		, TarjetaNueva
*/
#if defined(_DEBUG)
		, TarjetaEToken
#endif
		, TarjetaDesconocida
	} TipoTarjeta;

	typedef enum
	{
		ToolkitCryptLib,
		TookitCAPI
	} TipoToolkit;

public:
	static TipoTarjeta* CrearListaTarjetasSoportadas(int &total);
	static void LiberarListaTarjetasSoportadas(TipoTarjeta* &lista);

	static Tarjeta* AutodetectarTarjeta(TipoTarjeta &tarjetaDetectada, TipoToolkit tipoToolkit);
	static Tarjeta* AutodetectarTarjeta(TipoTarjeta tipos[], int numTipos, TipoTarjeta &tarjetaDetectada, TipoToolkit tipoToolkit);

	static Tarjeta* CrearTarjeta(const char *nombreTarjeta, const char *nombreToolkit);
	static Tarjeta* CrearTarjeta(TipoTarjeta tipoTarjeta, TipoToolkit tipoToolkit);

	static void Liberar(Tarjeta* &obj);

	static const char* ConvertirTipoTarjeta(TipoTarjeta tipo);
	static TipoTarjeta ConvertirTipoTarjeta(const char* _tipo);
};


#endif

