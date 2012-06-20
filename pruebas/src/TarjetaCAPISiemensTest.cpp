#include "../h/TarjetaCAPISiemensTest.h"

#include "TestSuite.h"

#include "../../h/TarjetaCAPISiemens.h"

#undef	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#define	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"



TarjetaCAPISiemensTest::TarjetaCAPISiemensTest(std::string name)
	: TarjetaCAPITest(name)
{
}

TarjetaCAPISiemensTest::~TarjetaCAPISiemensTest()
{
}


Test* TarjetaCAPISiemensTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaCAPISiemens");

	suite->addTest( new TarjetaCAPISiemensTestCaller("PedirPIN", &TarjetaCAPISiemensTest::testPedirPIN) );

	return (suite);
}


TarjetaCAPI* TarjetaCAPISiemensTest::createTarjeta()
{
	TarjetaCAPI* ret;

	ret = new TarjetaCAPISiemens();

	if ( !ret->conectar() )
	{
		delete ret;
		ret = NULL;
	}

	return (ret);
}

ArsCadena TarjetaCAPISiemensTest::getPINCorrecto()
{
	return PIN_CORRECTO;
}

ArsCadena TarjetaCAPISiemensTest::getPINErroneo()
{
	return PIN_ERRONEO;
}

ArsCadena TarjetaCAPISiemensTest::getPINErroneoCorto()
{
	return PIN_ERRONEO_CORTO;
}

ArsCadena TarjetaCAPISiemensTest::getPINErroneoLargo()
{
	return PIN_ERRONEO_LARGO;
}


void TarjetaCAPISiemensTest::testPedirPIN()
{
	probarPIN();
}

