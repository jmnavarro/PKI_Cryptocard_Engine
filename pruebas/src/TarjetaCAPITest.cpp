#include "../h/TarjetaCAPITest.h"

#include "TestSuite.h"

#include "../../h/TarjetaCAPI.h"


TarjetaCAPITest::TarjetaCAPITest(std::string name)
	: TestCase(name)
{
}

TarjetaCAPITest::~TarjetaCAPITest()
{
}

void TarjetaCAPITest::setUp()
{
	fixture = createTarjeta();
}

void TarjetaCAPITest::tearDown()
{
	delete fixture;
}


void TarjetaCAPITest::probarPIN()
{
	ArsCadena	msg;
	bool		establecerPINCorrecto;

	VALIDAR( fixture != NULL );

	establecerPINCorrecto = fixture->establecerPIN(getPINErroneo(), msg);
	VALIDAR_CON_MENSAJE( !establecerPINCorrecto, msg.cadena() );

	establecerPINCorrecto = fixture->establecerPIN(getPINCorrecto(), msg);
	VALIDAR_CON_MENSAJE( establecerPINCorrecto, msg.cadena() );

	establecerPINCorrecto = fixture->establecerPIN(getPINErroneoCorto(), msg);
	VALIDAR_CON_MENSAJE( !establecerPINCorrecto, msg.cadena() );

	// se vuelve a validar el correcto para que la tarjeta no se bloqué
	establecerPINCorrecto = fixture->establecerPIN(getPINCorrecto(), msg);
	VALIDAR_CON_MENSAJE( establecerPINCorrecto, msg.cadena() );

	establecerPINCorrecto = fixture->establecerPIN(getPINErroneoLargo(), msg);
	VALIDAR_CON_MENSAJE( !establecerPINCorrecto, msg.cadena() );
}

