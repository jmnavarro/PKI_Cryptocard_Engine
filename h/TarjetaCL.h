#if !defined(TARJETA_CL_H)
#define TARJETA_CL_H

#include "cryptLibAPI.h"

#include "Tarjeta.h"
#include "../../LibUtils/h/ArsCadena.h"


class Certificado;
class Firmador;


class TarjetaCL : public Tarjeta
{
public:
	TarjetaCL(const ArsCadena &rutaDLL, const ArsCadena &nombreDispositivo);
	virtual ~TarjetaCL();

	virtual bool conectar(Certificado *cert = NULL);
	virtual bool desconectar();
	virtual bool isConectado() const;

	virtual AlmacenCertificado* abrirAlmacen(const ArsCadena &nombre = "");
	virtual bool cerrarAlmacen(AlmacenCertificado* almacen);

	virtual bool establecerPIN(const ArsCadena &pin, ArsCadena &msg);

	virtual Firmador* createFirmador();

	virtual ArsCadena getEtiqueta() const;

	ArsCadena GetLastMensajeError() const;

protected:
	virtual bool comprobarPIN(const ArsCadena &pin);
	virtual ArsCadena GetMensajeErrorEstablecerPIN(int codigo) const;

	bool reiniciarConexion();

	ArsCadena	getRutaDriver() const;
	ArsCadena	getNombreDispositivo() const;

	CL_DEVICE getHandle() const;

private:
	bool configurarDriver(const ArsCadena &ruta) const;

	CL_DEVICE	handle;
	ArsCadena	rutaDriver;
	int		 	lastError;
};


#endif
