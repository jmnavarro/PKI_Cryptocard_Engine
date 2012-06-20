#include "../h/TarjetaBuilderCLTest.h"

#include "TestSuite.h"

#include "../../h/TarjetaBuilder.h"



TarjetaBuilderCLTest::TarjetaBuilderCLTest(std::string name)
	: TestCase(name)
{
}

TarjetaBuilderCLTest::~TarjetaBuilderCLTest()
{
}



Test* TarjetaBuilderCLTest::suite()
{
	TestSuite *suite = new TestSuite("TarjetaBuilder (requiere una tarjeta de cada tipo)");

	suite->addTest( new TarjetaBuilderCLTestCaller("CrearTarjetaCERES",		&TarjetaBuilderCLTest::testCrearTarjetaCERES) );
	suite->addTest( new TarjetaBuilderCLTestCaller("CrearTarjetaSTARCOS",	&TarjetaBuilderCLTest::testCrearTarjetaSTARCOS) );
	suite->addTest( new TarjetaBuilderCLTestCaller("CrearTarjetaSIEMENS",	&TarjetaBuilderCLTest::testCrearTarjetaSIEMENS) );
	suite->addTest( new TarjetaBuilderCLTestCaller("AutodetectarTarjetasSoportadas",&TarjetaBuilderCLTest::testAutodetectarTarjetasSoportadas) );
	suite->addTest( new TarjetaBuilderCLTestCaller("AutodetectarListaTarjetas",		&TarjetaBuilderCLTest::testAutodetectarListaTarjetas) );
	suite->addTest( new TarjetaBuilderCLTestCaller("ConvertirTipoTarjeta",	&TarjetaBuilderCLTest::testConvertirTipoTarjeta) );

	return (suite);
}


void TarjetaBuilderCLTest::testCrearTarjetaCERES()
{
	MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo CERES...", "Tarjeta CERES", MB_ICONINFORMATION);

	Tarjeta *tarjeta;

	tarjeta = TarjetaBuilder::CrearTarjeta(TarjetaBuilder::TarjetaCeres, TarjetaBuilder::ToolkitCryptLib);
	VALIDAR( tarjeta != NULL );
	VALIDAR( tarjeta->conectar() );

	TarjetaBuilder::Liberar(tarjeta);
	VALIDAR( tarjeta == NULL );
}


void TarjetaBuilderCLTest::testCrearTarjetaSTARCOS()
{
	MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo STARCOS...", "Tarjeta STARCOS", MB_ICONINFORMATION);

	Tarjeta *tarjeta;

	tarjeta = TarjetaBuilder::CrearTarjeta(TarjetaBuilder::TarjetaStarcos, TarjetaBuilder::ToolkitCryptLib);
	VALIDAR( tarjeta != NULL );
	VALIDAR( tarjeta->conectar() );

	TarjetaBuilder::Liberar(tarjeta);
	VALIDAR( tarjeta == NULL );
}


void TarjetaBuilderCLTest::testCrearTarjetaSIEMENS()
{
	MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo SIEMENS...", "Tarjeta SIEMENS", MB_ICONINFORMATION);

	Tarjeta *tarjeta;

	tarjeta = TarjetaBuilder::CrearTarjeta(TarjetaBuilder::TarjetaSiemens, TarjetaBuilder::ToolkitCryptLib);
	VALIDAR( tarjeta != NULL );
	VALIDAR( tarjeta->conectar() );

	TarjetaBuilder::Liberar(tarjeta);
	VALIDAR( tarjeta == NULL );
}


void TarjetaBuilderCLTest::testAutodetectarTarjetasSoportadas()
{
	Tarjeta *tarjeta;
	TarjetaBuilder::TipoTarjeta tipo;

	MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo CERES...", "Test autodetección de tarjeta", MB_ICONINFORMATION);
	tarjeta = TarjetaBuilder::AutodetectarTarjeta(tipo, TarjetaBuilder::ToolkitCryptLib);
	VALIDAR( tarjeta != NULL );
	try {
		VALIDAR( tipo == TarjetaBuilder::TarjetaCeres );
		VALIDAR( tarjeta->conectar() );
	}
	__finally {
		TarjetaBuilder::Liberar(tarjeta);
		VALIDAR( tarjeta == NULL );
	}

	MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo STARCOS...", "Test autodetección de tarjeta", MB_ICONINFORMATION);
	tarjeta = TarjetaBuilder::AutodetectarTarjeta(tipo, TarjetaBuilder::ToolkitCryptLib);
	VALIDAR( tarjeta != NULL );
	try {
		VALIDAR( tipo == TarjetaBuilder::TarjetaStarcos );
		VALIDAR( tarjeta->conectar() );
	}
	__finally {
		TarjetaBuilder::Liberar(tarjeta);
		VALIDAR( tarjeta == NULL );
	}

	MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo SIEMENS...", "Test autodetección de tarjeta", MB_ICONINFORMATION);
	tarjeta = TarjetaBuilder::AutodetectarTarjeta(tipo, TarjetaBuilder::ToolkitCryptLib);
	VALIDAR( tarjeta != NULL );
	try {
		VALIDAR( tipo == TarjetaBuilder::TarjetaSiemens );
		VALIDAR( tarjeta->conectar() );
	}
	__finally {
		TarjetaBuilder::Liberar(tarjeta);
		VALIDAR( tarjeta == NULL );
	}
}


void TarjetaBuilderCLTest::testAutodetectarListaTarjetas()
{
	// Parece (sólo parece) que los drivers STARCOS y SIEMENS no son muy compatibles.
	// Cuando abrimos intentamos conectar con una tarjeta SIEMENS, y dentro del lector
	// hay una STARCOS, conecta ok, y no debería ya que la que hay dentro del lector no
	// es SIEMENS. Parece un bug del driver de SIEMENS, aunque habría que haber pruebas
	// más exaustivas.
	// Al revés parace que no ocurre (intentar conectar a una STARCOS cuando tenemos
	// una SIEMENS dentro).
	// Para desenmascarar el bug hay que cambiar la variable
	// 		TipoTarjeta	tipos[] = {TarjetaCeres, TarjetaSiemens, TarjetaStarcos};
	// en el método TarjetaBuilder::CrearListaTarjetasSoportadas.

	Tarjeta *tarjeta;
	TarjetaBuilder::TipoTarjeta tipo;
	TarjetaBuilder::TipoTarjeta *soportadas;
	int total;

	soportadas = TarjetaBuilder::CrearListaTarjetasSoportadas(total);
	try
	{
		soportadas[0] = TarjetaBuilder::TarjetaSiemens;
		soportadas[1] = TarjetaBuilder::TarjetaStarcos;
		for (int i = 0; i < total - 2; i++)
			soportadas[i+2] = TarjetaBuilder::TarjetaDesconocida;

		MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo SIEMENS...", "Test autodetección de tarjeta", MB_ICONINFORMATION);
		tarjeta = TarjetaBuilder::AutodetectarTarjeta(tipo, TarjetaBuilder::ToolkitCryptLib);
		VALIDAR( tipo == TarjetaBuilder::TarjetaSiemens );
		VALIDAR( tarjeta != NULL );
		VALIDAR( tarjeta->conectar() );
		TarjetaBuilder::Liberar(tarjeta);
		VALIDAR( tarjeta == NULL );

		MessageBox(GetActiveWindow(), "Introduce una tarjeta de tipo STARCOS...", "Test autodetección de tarjeta", MB_ICONINFORMATION);
		tarjeta = TarjetaBuilder::AutodetectarTarjeta(tipo, TarjetaBuilder::ToolkitCryptLib);
		VALIDAR( tipo == TarjetaBuilder::TarjetaStarcos );
		VALIDAR( tarjeta != NULL );
		VALIDAR( tarjeta->conectar() );
		TarjetaBuilder::Liberar(tarjeta);
		VALIDAR( tarjeta == NULL );

	}
	__finally {
		TarjetaBuilder::LiberarListaTarjetasSoportadas(soportadas);
	}
}


void TarjetaBuilderCLTest::testConvertirTipoTarjeta()
{
	TarjetaBuilder::TipoTarjeta	tipo;
	ArsCadena nombre;

	nombre = TarjetaBuilder::ConvertirTipoTarjeta(TarjetaBuilder::TarjetaCeres);
	VALIDAR(nombre == NOMBRE_TARJETA_CERES);

	nombre = TarjetaBuilder::ConvertirTipoTarjeta(TarjetaBuilder::TarjetaStarcos);
	VALIDAR(nombre == NOMBRE_TARJETA_STARCOS);

	nombre = TarjetaBuilder::ConvertirTipoTarjeta(TarjetaBuilder::TarjetaSiemens);
	VALIDAR(nombre == NOMBRE_TARJETA_SIEMENS);


	tipo = TarjetaBuilder::ConvertirTipoTarjeta(NOMBRE_TARJETA_CERES);
	VALIDAR(tipo == TarjetaBuilder::TarjetaCeres);

	tipo = TarjetaBuilder::ConvertirTipoTarjeta(NOMBRE_TARJETA_STARCOS);
	VALIDAR(tipo == TarjetaBuilder::TarjetaStarcos);

	tipo = TarjetaBuilder::ConvertirTipoTarjeta(NOMBRE_TARJETA_SIEMENS);
	VALIDAR(tipo == TarjetaBuilder::TarjetaSiemens);
}



