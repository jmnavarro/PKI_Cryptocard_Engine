#include "../h/TarjetaCAPICeresTest.h"

#include "TestSuite.h"

#include "../../h/TarjetaCAPICeres.h"
#include "../../h/CertCAPI.h"
#include "../../h/AlmacenCertificado.h"

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"



TarjetaCAPICeresTest::TarjetaCAPICeresTest(std::string name)
	: TarjetaCAPITest(name)
{
}

TarjetaCAPICeresTest::~TarjetaCAPICeresTest()
{
}


Test* TarjetaCAPICeresTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaCAPICeres");

	suite->addTest( new TarjetaCAPICeresTestCaller("PedirPIN",		&TarjetaCAPICeresTest::testPedirPIN) );

	return (suite);
}


TarjetaCAPI* TarjetaCAPICeresTest::createTarjeta()
{
	TarjetaCAPI* ret;
	AlmacenCertificado *store;
	Certificado	*cert;
	bool ok = false;

	ret = new TarjetaCAPICeres();

	store = dynamic_cast<TarjetaCAPICeres*> (ret)->abrirAlmacen();
	if (store != NULL)
	{
		cert = store->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
		if (cert != NULL)
		{
			ok = ret->conectar(cert);
			delete cert;
		}
		ret->cerrarAlmacen(store);
	}

	if (!ok)
	{
		delete ret;
		ret = NULL;
	}

	return (ret);
}


ArsCadena TarjetaCAPICeresTest::getPINCorrecto()
{
	return PIN_CORRECTO;
}

ArsCadena TarjetaCAPICeresTest::getPINErroneo()
{
	return PIN_ERRONEO;
}

ArsCadena TarjetaCAPICeresTest::getPINErroneoCorto()
{
	return PIN_ERRONEO_CORTO;
}

ArsCadena TarjetaCAPICeresTest::getPINErroneoLargo()
{
	return PIN_ERRONEO_LARGO;
}


void TarjetaCAPICeresTest::testPedirPIN()
{
	probarPIN();
}

