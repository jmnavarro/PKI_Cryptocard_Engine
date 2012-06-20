#include "../h/TarjetaCAPIStarcosTest.h"

#include "TestSuite.h"

#include "../../h/TarjetaCAPIStarcos.h"

#undef	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#define TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"



TarjetaCAPIStarcosTest::TarjetaCAPIStarcosTest(std::string name)
	: TarjetaCAPITest(name)
{
}

TarjetaCAPIStarcosTest::~TarjetaCAPIStarcosTest()
{
}


Test* TarjetaCAPIStarcosTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaCAPIStarcos");

	suite->addTest( new TarjetaCAPIStarcosTestCaller("PedirPIN", &TarjetaCAPIStarcosTest::testPedirPIN) );

	return (suite);
}


TarjetaCAPI* TarjetaCAPIStarcosTest::createTarjeta()
{
	TarjetaCAPI* ret;

	ret = new TarjetaCAPIStarcos();

	if ( !ret->conectar() )
	{
		delete ret;
		ret = NULL;
	}

	return (ret);
}

ArsCadena TarjetaCAPIStarcosTest::getPINCorrecto()
{
	return PIN_CORRECTO;
}

ArsCadena TarjetaCAPIStarcosTest::getPINErroneo()
{
	return PIN_ERRONEO;
}

ArsCadena TarjetaCAPIStarcosTest::getPINErroneoCorto()
{
	return PIN_ERRONEO_CORTO;
}

ArsCadena TarjetaCAPIStarcosTest::getPINErroneoLargo()
{
	return PIN_ERRONEO_LARGO;
}


void TarjetaCAPIStarcosTest::testPedirPIN()
{
	probarPIN();
}
