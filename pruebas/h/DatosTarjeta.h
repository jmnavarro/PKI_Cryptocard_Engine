//
// Defines de los datos de la tarjeta a utilizar en las pruebas
// Dependiendo del tipo de tarjeta que queramos utilizar, hay que definir un símbolo u otro:
// Tarjetas soportadas:
//		#define TEST_TARJETA_CERES
//				Para utilizar tarjetas CERES-FNMT que vienen con el CryptoKit
//		#define TEST_TARJETA_ETOKEN
//				Para utilizar el token criptográfico USB "eToken".
//		#define TEST_TARJETA_SIEMENS
//				Para utilizar la tarjeta criptográfica SIEMENS de la GVA (la nº 1)
//		#define TEST_TARJETA_STARCOS
//				Para utilizar la tarjeta criptográfica STARCOS de Camerfirma (Tarjeta blanca TEST-PFP)
//

// datos tarjeta CERES
#define  CERES_ALMACEN_TARJETA		"MY\\Tarjeta CERES"
#define  CERES_PIN_CORRECTO			"123456789"
#define  CERES_PIN_ERRONEO			"987654321"
#define  CERES_PIN_ERRONEO_CORTO	"87654321"
#define  CERES_PIN_ERRONEO_LARGO	"0987654321"
#define  CERES_ETIQUETA_CERTIFICADO	""

// datos tarjeta eToken
#define  ETOKEN_ALMACEN_TARJETA			"MY"	/* "MY\\eToken" */
#define  ETOKEN_PIN_CORRECTO			"123456789"
#define  ETOKEN_PIN_ERRONEO				"987654321"
#define  ETOKEN_PIN_ERRONEO_CORTO		"87654321"
#define  ETOKEN_PIN_ERRONEO_LARGO		"0987654321"
#define  ETOKEN_ETIQUETA_CERTIFICADO	""

// datos tarjeta Siemens
#define  SIEMENS_ALMACEN_TARJETA		"MY"
#define  SIEMENS_PIN_CORRECTO			"12345678"
#define  SIEMENS_PIN_ERRONEO			"87654321"
#define  SIEMENS_PIN_ERRONEO_CORTO		"7654321"
#define  SIEMENS_PIN_ERRONEO_LARGO		"987654321"
#define  SIEMENS_ETIQUETA_CERTIFICADO	"FIRMA"

// datos tarjeta Starcos
#define  STARCOS_ALMACEN_TARJETA		"MY"
#define  STARCOS_PIN_CORRECTO			"12345678"
#define  STARCOS_PIN_ERRONEO			"87654321"
#define  STARCOS_PIN_ERRONEO_CORTO		"7654321"
#define  STARCOS_PIN_ERRONEO_LARGO		"987654321"
#define  STARCOS_ETIQUETA_CERTIFICADO	""


//
// Valores de tarjeta a utilizar desde el exterior
//
#if defined(TEST_TARJETA_CERES)
#	pragma message ("	- Se usará la tarjeta Ceres")
#	define PIN_CORRECTO			CERES_PIN_CORRECTO
#	define PIN_ERRONEO			CERES_PIN_ERRONEO
#	define PIN_ERRONEO_CORTO	CERES_PIN_ERRONEO_CORTO
#	define PIN_ERRONEO_LARGO	CERES_PIN_ERRONEO_LARGO
#	define ALMACEN_TARJETA		CERES_ALMACEN_TARJETA
#	define ETIQUETA_CERTIFICADO	CERES_ETIQUETA_CERTIFICADO

#elif defined(TEST_TARJETA_ETOKEN)
#	pragma message ("	- Se usará la tarjeta eToken")
#	define PIN_CORRECTO			ETOKEN_PIN_CORRECTO
#	define PIN_ERRONEO			ETOKEN_PIN_ERRONEO
#	define PIN_ERRONEO_CORTO	ETOKEN_PIN_ERRONEO_CORTO
#	define PIN_ERRONEO_LARGO	ETOKEN_PIN_ERRONEO_LARGO
#	define ALMACEN_TARJETA		ETOKEN_ALMACEN_TARJETA
#	define ETIQUETA_CERTIFICADO	ETOKEN_ETIQUETA_CERTIFICADO

#elif defined(TEST_TARJETA_SIEMENS)
#	pragma message ("	- Se usará la tarjeta Siemens")
#	define PIN_CORRECTO			SIEMENS_PIN_CORRECTO
#	define PIN_ERRONEO			SIEMENS_PIN_ERRONEO
#	define PIN_ERRONEO_CORTO	SIEMENS_PIN_ERRONEO_CORTO
#	define PIN_ERRONEO_LARGO	SIEMENS_PIN_ERRONEO_LARGO
#	define ALMACEN_TARJETA		SIEMENS_ALMACEN_TARJETA
#	define ETIQUETA_CERTIFICADO	SIEMENS_ETIQUETA_CERTIFICADO

#elif defined(TEST_TARJETA_STARCOS)
#	pragma message ("	- Se usará la tarjeta Starcos")
#	define PIN_CORRECTO			STARCOS_PIN_CORRECTO
#	define PIN_ERRONEO			STARCOS_PIN_ERRONEO
#	define PIN_ERRONEO_CORTO	STARCOS_PIN_ERRONEO_CORTO
#	define PIN_ERRONEO_LARGO	STARCOS_PIN_ERRONEO_LARGO
#	define ALMACEN_TARJETA		STARCOS_ALMACEN_TARJETA
#	define ETIQUETA_CERTIFICADO	STARCOS_ETIQUETA_CERTIFICADO

#else
#	pragma message ("	- ATENCIÓN: no se ha configurado ninguna tarjeta para hacer las pruebas.")

#endif


// Valores referentes a los almacenes a utilizar
#define  ALMACEN_PRUEBAS		ALMACEN_TARJETA
#define  ALMACEN_CERTIFICADOS	ALMACEN_PRUEBAS
