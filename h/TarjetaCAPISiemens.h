#if !defined(TARJETA_SIEMENS_H)
#define TARJETA_SIEMENS_H


#include "TarjetaCAPI.h"


class TarjetaCAPISiemens : public TarjetaCAPI
{
public:
	TarjetaCAPISiemens();
	virtual ~TarjetaCAPISiemens();

	virtual bool conectar(Certificado *cert = NULL);
	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre = "");
};


#endif
