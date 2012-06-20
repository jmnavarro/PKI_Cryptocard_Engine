#include "../h/TarjetaCLSiemens.h"


#define DRIVER_PKCS11_SIEMENS		"CardOS_PKCS11.dll"
#define NOMBRE_DISPOSITIVO_SIEMENS	"CardOS_PKCS11 by DevTeam"


TarjetaCLSiemens::TarjetaCLSiemens()
	: TarjetaCL(DRIVER_PKCS11_SIEMENS, NOMBRE_DISPOSITIVO_SIEMENS)
{
}


TarjetaCLSiemens::~TarjetaCLSiemens()
{
}

 