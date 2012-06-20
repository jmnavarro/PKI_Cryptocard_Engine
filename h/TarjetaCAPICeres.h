#if !defined(TARJETA_CERES_H)
#define TARJETA_CERES_H


#include "TarjetaCAPI.h"


class TarjetaCAPICeres : public TarjetaCAPI
{
public:
	TarjetaCAPICeres();
	virtual ~TarjetaCAPICeres();

	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre = "");

	virtual bool establecerPIN(const ArsCadena &pin, ArsCadena &msg);

private:
	bool generarFirmaFicticia();
};


#endif
