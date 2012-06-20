#include "../h/TarjetaCAPISiemens.h"

#define NOMBRE_TARJETA_SIEMENS	"CardOS_CSP"
#define NOMBRE_ALMACEN_SIEMENS	"My"

#define ALGORITMO_HASH			CALG_SHA1
#define TIPO_PIN				PP_SIGNATURE_PIN


TarjetaCAPISiemens::TarjetaCAPISiemens()
	: TarjetaCAPI(NOMBRE_TARJETA_SIEMENS)
{
}

TarjetaCAPISiemens::~TarjetaCAPISiemens()
{
}


bool TarjetaCAPISiemens::conectar(Certificado *cert)
{
	// Parece ser que el CardOS_CSP tiene un bug aleatorio cuando se intenta abrir
	// el contexto de tarjeta con CRYPT_SILENT. Retorna un NTE_SILENT_CONTEXT, pero si
	// se quita el CRYPT_SILENT, en realidad no está sacando ninguna ventana.
	// Si después de este error se vuelve a intentar abrir el contexto con SILENT,
	// ya no retorna error.
	bool ok;

	ok = TarjetaCAPI::conectar(cert);
	
	if ( !ok && GetLastError() == (DWORD) NTE_SILENT_CONTEXT )
	{
		ok = TarjetaCAPI::conectar(cert);
	}

	return (ok);
}


AlmacenCertificado* TarjetaCAPISiemens::abrirAlmacen(const ArsCadena &nombre)
{
	return TarjetaCAPI::abrirAlmacen(NOMBRE_ALMACEN_SIEMENS);
}



