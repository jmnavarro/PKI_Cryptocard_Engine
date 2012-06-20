#include "../h/TarjetaCAPIeTTest.h"

#include "TestSuite.h"

#include "../../h/AlmacenCertificado.h"
#include "../../h/CertCAPI.h"

#undef	TEST_TARJETA_CERES
#define	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"

#define TEST_CERTIFICADO_CAMERFIRMA
#undef  TEST_CERTIFICADO_GVA_1
#include "../h/DatosCertificado.h"



TarjetaCAPIeTTest::TarjetaCAPIeTTest(std::string name)
	: TestCase(name)
{
}

TarjetaCAPIeTTest::~TarjetaCAPIeTTest()
{
}



void TarjetaCAPIeTTest::setUp()
{
	obj = new TarjetaCAPIeT();
}

void TarjetaCAPIeTTest::tearDown()
{
	delete obj;
}



Test* TarjetaCAPIeTTest::suite()
{
	TestSuite *suite = new TestSuite("eToken con CryptoAPI");

	suite->addTest( new TarjetaCAPIeTTestCaller("PedirPIN",	&TarjetaCAPIeTTest::testPedirPIN) );

	return (suite);
}


void TarjetaCAPIeTTest::testPedirPIN()
{
	ArsCadena	msg, substr;
	AlmacenCertificado *store;
	Certificado	*cert;
	bool		conectado;
	bool		esPINCorrecto;

	VALIDAR( obj->conectar() );

	store = obj->abrirAlmacen();
	VALIDAR(store != NULL);
	__try
	{
		substr = SUBSTR_CERTIFICADO;

		cert = store->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &substr);
		VALIDAR(cert != NULL);
		__try
		{
			VALIDAR( obj->conectar(cert) );

			esPINCorrecto = obj->establecerPIN(PIN_ERRONEO, msg);
			VALIDAR_CON_MENSAJE( !esPINCorrecto, msg.cadena() );

			esPINCorrecto = obj->establecerPIN(PIN_CORRECTO, msg);
			VALIDAR_CON_MENSAJE( esPINCorrecto, msg.cadena() );
		}
		__finally {
			delete cert;
		}
	}
	__finally {
		obj->cerrarAlmacen(store);
	}
}

 