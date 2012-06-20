#if !defined(TARJETA_CAPI_STARCOS_H)
#define TARJETA_CAPI_STARCOS_H


#include "TarjetaCAPI.h"


class TarjetaCAPIStarcos : public TarjetaCAPI
{
public:
	TarjetaCAPIStarcos();
	virtual ~TarjetaCAPIStarcos();

	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre = "");
};


#endif
