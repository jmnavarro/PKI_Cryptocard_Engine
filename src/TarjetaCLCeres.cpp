#include "../h/TarjetaCLCeres.h"


#define DRIVER_PKCS11_CERES			"PkcsV2GK.dll"
#define NOMBRE_DISPOSITIVO_CERES    "Modulo CryptokiV2"


TarjetaCLCeres::TarjetaCLCeres()
	: TarjetaCL(DRIVER_PKCS11_CERES, NOMBRE_DISPOSITIVO_CERES)
{
}


TarjetaCLCeres::~TarjetaCLCeres()
{
}

 