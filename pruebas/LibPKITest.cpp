#include "TarjetaCLCeresTest.h"
#include "TarjetaCLSiemensTest.h"
#include "TarjetaCLStarcosTest.h"
#include "ValidacionCertificadoCLTest.h"
#include "AlmacenCertificadoCLTest.h"
#include "CertCLTest.h"
#include "FirmadorCLTest.h"
#include "TarjetaBuilderCLTest.h"

#include <vcl.h>


#include "ITestRunner.h"
#include "TestSuite.h"

#include "BufferDinamicoTest.h"
#include "ConversorDatosTest.h"
#include "FirmaDetachedTest.h"
#include "AlmacenCertificadoCAPITest.h"
#include "FirmadorCAPITest.h"
#include "ValidacionCertificadoCAPITest.h"
#include "CertCAPITest.h"

#include "TarjetaCAPICeresTest.h"
#include "TarjetaCAPISiemensTest.h"
#include "TarjetaCAPIStarcosTest.h"
#include "TarjetaCAPIeTTest.h"


void configurarArbolTest(ITestRunner *runner)
{
	TestSuite *grupoCAPI, *grupoCL;

	runner->addTest( BufferDinamicoTest::suite() );
	runner->addTest( ConversorDatosTest::suite() );
	runner->addTest( FirmaDetachedTest::suite() );

	grupoCAPI = new TestSuite("CryptoAPI");
	grupoCAPI->addTest( AlmacenCertificadoCAPITest::suite() );
	grupoCAPI->addTest( FirmadorCAPITest::suite() );
	grupoCAPI->addTest( ValidacionCertificadoCAPITest::suite() );
	grupoCAPI->addTest( TarjetaCAPICeresTest::suite() );
	grupoCAPI->addTest( TarjetaCAPISiemensTest::suite() );
	grupoCAPI->addTest( TarjetaCAPIStarcosTest::suite() );
	grupoCAPI->addTest( CertCAPITest::suite() );

	grupoCL = new TestSuite("cryptLib");
	grupoCL->addTest( AlmacenCertificadoCLTest::suite() );
	grupoCL->addTest( FirmadorCLTest::suite() );
	grupoCL->addTest( ValidacionCertificadoCLTest::suite() );
	grupoCL->addTest( TarjetaCLCeresTest::suite() );
	grupoCL->addTest( TarjetaCLSiemensTest::suite() );
	grupoCL->addTest( TarjetaCLStarcosTest::suite() );
	grupoCL->addTest( CertCLTest::suite() );
	grupoCL->addTest( TarjetaBuilderCLTest::suite() );

	runner->addTest(grupoCAPI);
	runner->addTest(grupoCL);

//	grupoCAPI->addTest( TarjetaCAPIeTTest::suite() );
}


WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		ITestRunner	runner;

		configurarArbolTest(&runner);

		runner.run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}

	// liberar el singleton propio de cryptLib
//	cryptLib::freeInstancia();

	return 0;
}

