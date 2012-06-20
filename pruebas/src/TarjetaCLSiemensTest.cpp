#include "../h/TarjetaCLSiemensTest.h"

#include "TestSuite.h"

#undef	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#define TEST_TARJETA_SIEMENS
#undef TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"


TarjetaCLSiemensTest::TarjetaCLSiemensTest(std::string name)
	: TestCase(name)
{
}

TarjetaCLSiemensTest::~TarjetaCLSiemensTest()
{
}



void TarjetaCLSiemensTest::setUp()
{
	obj = new TarjetaCLSiemens();
}

void TarjetaCLSiemensTest::tearDown()
{
	delete obj;
}



Test* TarjetaCLSiemensTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaCLSiemens (requiere dos tarjetas cualquiera)");

	suite->addTest( new TarjetaCLSiemensTestCaller("Conectar",		&TarjetaCLSiemensTest::testConectar) );
	suite->addTest( new TarjetaCLSiemensTestCaller("PedirPIN",		&TarjetaCLSiemensTest::testPedirPIN) );
	suite->addTest( new TarjetaCLSiemensTestCaller("CambioTarjeta",	&TarjetaCLSiemensTest::testCambioTarjeta) );

	return (suite);
}


void TarjetaCLSiemensTest::testConectar()
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


void TarjetaCLSiemensTest::testPedirPIN()
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


void TarjetaCLSiemensTest::testCambioTarjeta()
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

