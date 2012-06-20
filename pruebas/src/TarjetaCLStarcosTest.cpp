#include "../h/TarjetaCLStarcosTest.h"

#include "TestSuite.h"

#undef	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#define TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"


TarjetaCLStarcosTest::TarjetaCLStarcosTest(std::string name)
	: TestCase(name)
{
}

TarjetaCLStarcosTest::~TarjetaCLStarcosTest()
{
}



void TarjetaCLStarcosTest::setUp()
{
	obj = new TarjetaCLStarcos();
}

void TarjetaCLStarcosTest::tearDown()
{
	delete obj;
}



Test* TarjetaCLStarcosTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaCLStarcos (requiere dos tarjetas cualquiera)");

	suite->addTest( new TarjetaCLStarcosTestCaller("Conectar",		&TarjetaCLStarcosTest::testConectar) );
	suite->addTest( new TarjetaCLStarcosTestCaller("PedirPIN",		&TarjetaCLStarcosTest::testPedirPIN) );
	suite->addTest( new TarjetaCLStarcosTestCaller("CambioTarjeta",	&TarjetaCLStarcosTest::testCambioTarjeta) );

	return (suite);
}


void TarjetaCLStarcosTest::testConectar()
{
	VALIDAR(obj->conectar());			// conecta
	__try
	{
		VALIDAR(obj->conectar());		// no hace nada
	}
	__finally
	{
		VALIDAR(obj->desconectar());	// desconecta
		VALIDAR(obj->desconectar());	// no hace nada
	}
}


void TarjetaCLStarcosTest::testPedirPIN()
{
	bool		pinOK;
	ArsCadena	msg;

	VALIDAR( obj->conectar() );
	__try
	{
		pinOK = obj->establecerPIN(PIN_ERRONEO, msg);
		VALIDAR_CON_MENSAJE( !pinOK, msg.cadena() );

		pinOK = obj->establecerPIN(PIN_CORRECTO, msg);
		VALIDAR_CON_MENSAJE( pinOK, msg.cadena() );

		// volver a establecer
		pinOK = obj->establecerPIN(PIN_CORRECTO, msg);
		VALIDAR_CON_MENSAJE( pinOK, msg.cadena() );
	}
	__finally {
		VALIDAR( obj->desconectar() );
	}
}


void TarjetaCLStarcosTest::testCambioTarjeta()
{
	MessageBox(GetActiveWindow(), "Introduce una tarjeta...", "Test cambio tarjeta", MB_ICONINFORMATION);
	VALIDAR( obj->conectar() );
	VALIDAR( obj->desconectar() );

	MessageBox(GetActiveWindow(), "Ahora introduce otra tarjeta distinta...", "Test cambio tarjeta", MB_ICONINFORMATION);
	VALIDAR( obj->conectar() );
	VALIDAR( obj->desconectar() );

	MessageBox(GetActiveWindow(), "Y otra más...", "Test cambio tarjeta", MB_ICONINFORMATION);
	VALIDAR( obj->conectar() );
	VALIDAR( obj->desconectar() );
}

