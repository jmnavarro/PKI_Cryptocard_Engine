#include "../h/TarjetaCAPIStarcos.h"

#define NOMBRE_TARJETA_STARCOS	"SafeSign CSP Version 1.0"
#define NOMBRE_ALMACEN_STARCOS	"My"

#define ALGORITMO_HASH			CALG_SHA1
#define TIPO_PIN				PP_KEYEXCHANGE_PIN



TarjetaCAPIStarcos::TarjetaCAPIStarcos()
	: TarjetaCAPI(NOMBRE_TARJETA_STARCOS)
{
}

TarjetaCAPIStarcos::~TarjetaCAPIStarcos()
{
}


AlmacenCertificado* TarjetaCAPIStarcos::abrirAlmacen(const ArsCadena &nombre)
{
	return TarjetaCAPI::abrirAlmacen(NOMBRE_ALMACEN_STARCOS);
}


