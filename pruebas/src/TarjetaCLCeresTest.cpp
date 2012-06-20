#include "../h/TarjetaCLCeresTest.h"

#include "TestSuite.h"

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"




TarjetaCLCeresTest::TarjetaCLCeresTest(std::string name)
	: TestCase(name)
{
}

TarjetaCLCeresTest::~TarjetaCLCeresTest()
{
}



void TarjetaCLCeresTest::setUp()
{
	obj = new TarjetaCLCeres();
}

void TarjetaCLCeresTest::tearDown()
{
	delete obj;
}



Test* TarjetaCLCeresTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaCLCeres (requiere dos tarjetas cualquiera)");

	suite->addTest( new TarjetaCLCeresTestCaller("Conectar",	  &TarjetaCLCeresTest::testConectar) );
	suite->addTest( new TarjetaCLCeresTestCaller("PedirPIN",	  &TarjetaCLCeresTest::testPedirPIN) );
	suite->addTest( new TarjetaCLCeresTestCaller("CambioTarjeta", &TarjetaCLCeresTest::testCambioTarjeta) );

	return (suite);
}


void TarjetaCLCeresTest::testConectar()
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


void TarjetaCLCeresTest::testPedirPIN()
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


void TarjetaCLCeresTest::testCambioTarjeta()
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

