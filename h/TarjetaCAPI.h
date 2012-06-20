#if !defined(TARJETA_CAPI_H)
#define TARJETA_CAPI_H

#include "Tarjeta.h"
#include "../../LibUtils/h/ArsCadena.h"

#include <windows.h>
#include <wincrypt.h>


class TarjetaCAPI : public Tarjeta
{
public:
	TarjetaCAPI(const ArsCadena &_nombre);
	virtual ~TarjetaCAPI();

	virtual bool conectar(Certificado *cert = NULL);
	virtual bool desconectar();
	virtual bool isConectado() const;

	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre);
	virtual bool cerrarAlmacen(AlmacenCertificado* almacen);

	virtual bool establecerPIN(const ArsCadena &pin, ArsCadena &msg);

	virtual Firmador* createFirmador();

	virtual ArsCadena getEtiqueta() const;

	HCRYPTPROV getHandle() const;

protected:
	virtual ArsCadena GetMensajeErrorEstablecerPIN(int codigo) const;

private:
	HCRYPTPROV  handle;
};


#endif

