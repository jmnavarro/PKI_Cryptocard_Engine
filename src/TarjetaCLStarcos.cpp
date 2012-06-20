#include "../h/TarjetaCLStarcos.h"


#define DRIVER_PKCS11_STARCOS		"aetpkss1.dll"
#define NOMBRE_DISPOSITIVO_STARCOS	"Cryptographic Token Interface"


TarjetaCLStarcos::TarjetaCLStarcos()
	: TarjetaCL(DRIVER_PKCS11_STARCOS, NOMBRE_DISPOSITIVO_STARCOS)
{
}


TarjetaCLStarcos::~TarjetaCLStarcos()
{
}

 