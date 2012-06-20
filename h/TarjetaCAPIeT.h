#if !defined(_TARJETA_CAPI_ET_H_)
#define _TARJETA_CAPI_ET_H_


#include "TarjetaCAPI.h"


class TarjetaCAPIeT : public TarjetaCAPI
{
public:
	TarjetaCAPIeT();
	virtual ~TarjetaCAPIeT();

	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre = "");
	virtual bool establecerPIN(const ArsCadena &pin, ArsCadena &msg);

protected:
	virtual bool comprobarPIN(const ArsCadena &pin);
	
};


#endif
 