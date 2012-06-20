#include "../h/ValidacionCertificadoCAPITest.h"

#include "TestSuite.h"

#include "../../h/ValidacionCertificado.h"
#include "../../h/AlmacenCertificadoCAPI.h"
#include "../../h/Certificado.h"

#undef  TEST_CERTIFICADO_CAMERFIRMA
#undef  TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1
#include "../h/DatosCertificado.h"

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"

#define TEST_TOOLKIT_CAPI
#undef  TEST_TOOLKIT_CL
#include "../h/DatosFirma.h"



ValidacionCertificadoCAPITest::ValidacionCertificadoCAPITest(std::string name)
	: TestCase(name)
{
}

ValidacionCertificadoCAPITest::~ValidacionCertificadoCAPITest()
{
}

//--------------------------------------------------------------------------------------

void ValidacionCertificadoCAPITest::setUp()
{
	almacenValidacion = new AlmacenCertificadoCAPI(ALMACEN_VALIDACION);
	almacenValidacion->open();

	obj = almacenValidacion->createValidacionCertificado();

	almacenCertificados = new AlmacenCertificadoCAPI(ALMACEN_CERTIFICADOS);
	almacenCertificados->open();
}

void ValidacionCertificadoCAPITest::tearDown()
{
	delete obj;
	obj = NULL;

	almacenValidacion->close();
	delete almacenValidacion;
	almacenValidacion = NULL;

	almacenCertificados->close();
	delete almacenCertificados;
	almacenCertificados = NULL;
}

//--------------------------------------------------------------------------------------

Test* ValidacionCertificadoCAPITest::suite()
{
	TestSuite *suite = new TestSuite("ValidacionCertificadoCAPI");

	suite->addTest( new ValidacionCertificadoCAPITestCaller("testValidarAlmacenCerrado", &ValidacionCertificadoCAPITest::testValidarAlmacenCerrado) );
	suite->addTest( new ValidacionCertificadoCAPITestCaller("testEstaRevocado", &ValidacionCertificadoCAPITest::testEstaRevocado) );
	suite->addTest( new ValidacionCertificadoCAPITestCaller("testEstaCaducado", &ValidacionCertificadoCAPITest::testEstaCaducado) );

	return (suite);
}



void ValidacionCertificadoCAPITest::RegistrarCRL()
{
	HANDLE hFile;
	DWORD size, lenDatos;
	LPBYTE datos;


	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	hFile = CreateFile(FICHERO_CRL, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	VALIDAR(hFile != INVALID_HANDLE_VALUE);
	try
	{
		size = GetFileSize(hFile, NULL);

		datos = new BYTE[size];
		try
		{
			lenDatos = 0;
			ReadFile(hFile, datos, size, &lenDatos, NULL);
			VALIDAR( almacenCertificados->almacenarCRL(FICHERO_CRL, datos, lenDatos) );
		}
		__finally {
			delete [] datos;
		}
	}
	__finally {
		CloseHandle(hFile);
	}
}


void ValidacionCertificadoCAPITest::testEstaRevocado()
{
	ArsCadena nifRevocado(SUBSTR_CERTIFICADO_REVOCADO);
	ArsCadena nifCaducado(SUBSTR_CERTIFICADO_CADUCADO);
	ArsCadena nifOK(SUBSTR_CERTIFICADO_OK);
	Certificado	*cert;

	RegistrarCRL();

	cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifRevocado);
	try	{
		VALIDAR_IGUALES(ValidacionCertificado::EstadoRevRevocado, obj->estaRevocado(cert) );
	}
	__finally {
		delete cert;
	}

	cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifCaducado);
	try	{
		VALIDAR_IGUALES(ValidacionCertificado::EstadoRevValido, obj->estaRevocado(cert) );
	}
	__finally {
		delete cert;
	}

	cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifOK);
	try	{
		VALIDAR_IGUALES(ValidacionCertificado::EstadoRevValido, obj->estaRevocado(cert) );
	}
	__finally {
		delete cert;
	}
}


void ValidacionCertificadoCAPITest::testEstaCaducado()
{
	ArsCadena nifRevocado(SUBSTR_CERTIFICADO_REVOCADO);
	ArsCadena nifCaducado(SUBSTR_CERTIFICADO_CADUCADO);
	ArsCadena nifOK(SUBSTR_CERTIFICADO_OK);
	Certificado	*cert;

	cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifCaducado);
	try	{
		VALIDAR_IGUALES(ValidacionCertificado::EstadoCadCaducado, obj->estaCaducado(cert) );
	}
	__finally {
		delete cert;
	}

	cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifRevocado);
	try	{
		VALIDAR_IGUALES(ValidacionCertificado::EstadoCadValido, obj->estaCaducado(cert) );
	}
	__finally {
		delete cert;
	}

	cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifOK);
	try	{
		VALIDAR_IGUALES(ValidacionCertificado::EstadoCadValido, obj->estaCaducado(cert) );
	}
	__finally {
		delete cert;
	}
}


void ValidacionCertificadoCAPITest::testValidarAlmacenCerrado()
{
	AlmacenCertificadoCAPI	*almacen;
	ValidacionCertificado	*validador;
	ArsCadena nifOK(SUBSTR_CERTIFICADO_OK);

	almacen = new AlmacenCertificadoCAPI(ALMACEN_VALIDACION);
	try
	{
		validador = almacen->createValidacionCertificado();
		try
		{
			Certificado	*cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &nifOK);
			try	{
				// la revocación requiere que el almacén esté abierto. La caducidad no.
				VALIDAR_IGUALES(ValidacionCertificado::EstadoRevError, validador->estaRevocado(cert) );
				VALIDAR_IGUALES(ValidacionCertificado::EstadoCadValido, validador->estaCaducado(cert) );
			}
			__finally {
				delete cert;
			}
		}
		__finally {
			delete validador;
		}
	}
	__finally {
		delete almacen;
	}

}

