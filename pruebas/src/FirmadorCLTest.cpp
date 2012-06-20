#include "../h/FirmadorCLTest.h"

#include "TestSuite.h"

#include "../../h/FirmadorCL.h"
#include "../../h/CertCL.h"
#include "../../h/AlmacenCertificado.h"
#include "../../h/Certificado.h"

#include "../../h/FirmaDetached.h"
#include "../../h/ConversorDatos.h"

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef	TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"

#undef	TEST_CERTIFICADO_CAMERFIRMA
#undef	TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1
#include "../h/DatosCertificado.h"

#undef  TEST_TOOLKIT_CAPI
#define TEST_TOOLKIT_CL
#include "../h/DatosFirma.h"

//
// configuramos el tipo de tarjeta a utilizar en el test:
//
#if defined(TEST_TARJETA_CERES)
#	include "../../h/TarjetaCLCeres.h"
#elif defined(TEST_TARJETA_ETOKEN)
#	include "../../h/TarjetaCLeT.h"
#elif defined(TEST_TARJETA_SIEMENS)
#	include "../../h/TarjetaCLSiemens.h"
#elif defined(TEST_TARJETA_STARCOS)
#	include "../../h/TarjetaCLStarcos.h"
#endif



FirmadorCLTest::FirmadorCLTest(std::string name)
	: TestCase(name)
{
}

FirmadorCLTest::~FirmadorCLTest()
{
}


void FirmadorCLTest::setUp()
{
}

void FirmadorCLTest::tearDown()
{
}


Test* FirmadorCLTest::suite()
{
	TestSuite *suite = new TestSuite("FirmadorCL (requiere Tarjeta B)");

	suite->addTest( new FirmadorCLTestCaller("Resumir",			&FirmadorCLTest::testResumir) );
	suite->addTest( new FirmadorCLTestCaller("FirmarDetached",	&FirmadorCLTest::testFirmarDetached) );
	suite->addTest( new FirmadorCLTestCaller("ValidarFirma",   	&FirmadorCLTest::testValidarFirma) );

	return (suite);
}


Firma* FirmadorCLTest::generarFirma()
{
	ArsCadena	label(ETIQUETA_CERTIFICADO);
	ArsCadena	msg;
	Tarjeta		*tarjeta;
	Firma 		*firma;
	AlmacenCertificado *store;
	Firmador	*obj;
	Certificado	*cert;

	firma = NULL;
	cert = NULL;

	tarjeta = createTarjeta();
	try
	{
		VALIDAR( tarjeta->conectar() );

		store = tarjeta->abrirAlmacen("");
		VALIDAR(store != NULL);
		try
		{
			// hay que establecer el pin antes para que sea capaz de cargar la
			// clave privada junto con el certificado.
			VALIDAR( tarjeta->establecerPIN(PIN_CORRECTO, msg) );

			// para crear el certificado lo tengo que hacer a través del almacén
			cert = store->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
			// si no hemos podido obtener el primero, lo intentamos por nombre
			if (cert == NULL && ETIQUETA_CERTIFICADO[0] != '\0')
				cert = store->loadCertificado(AlmacenCertificado::ClaveBusquedaNombre, &label);
				
			VALIDAR(cert != NULL);
			try
			{
				obj = tarjeta->createFirmador();
				try
				{
					firma = obj->firmar(Firmador::TipoFirmaDetached, TEXTO, *cert);
/*
					ArsCadena a;
					a = firma->serializar();

					VALIDAR(a != "");
*/
				}
				__finally {
					delete obj;
				}
			}
			__finally {
				delete cert;
			}
		}
		__finally {
			tarjeta->cerrarAlmacen(store);
		}
	}
	__finally {
		delete tarjeta;
	}

	return (firma);
}


Tarjeta* FirmadorCLTest::createTarjeta()
{
	Tarjeta	*ret;

#if defined(TEST_TARJETA_CERES)
	ret = new TarjetaCLCeres();
#elif defined(TEST_TARJETA_ETOKEN)
	ret = new TarjetaCLeT();
#elif defined(TEST_TARJETA_SIEMENS)
	ret = new TarjetaCLSiemens();
#elif defined(TEST_TARJETA_STARCOS)
	ret = new TarjetaCLStarcos();
#else
	VALIDAR_CON_MENSAJE(false, "No se ha configurado ninguna tarjeta");
#endif

	return (ret);
}


void FirmadorCLTest::testResumir()
{
	Tarjeta		*tarjeta;
	AlmacenCertificado *store;
	Firmador	*obj;
	ArsCadena	msg;

	tarjeta = createTarjeta();
	VALIDAR(tarjeta != NULL);
	try
	{
		VALIDAR( tarjeta->conectar() );

		Resumen	*resumen = NULL;
		Resumen *resumen2 = NULL;

		obj = tarjeta->createFirmador();
		try
		{
			resumen  = obj->resumir(TEXTO);
			resumen2 = obj->resumir(TEXTO);

			VALIDAR(resumen  != NULL);
			VALIDAR(resumen2 != NULL);

			try
			{
				VALIDAR_IGUALES(resumen->getSize(), resumen2->getSize());
				VALIDAR( 0 == memcmp(resumen->getBuffer(), resumen2->getBuffer(), resumen->getSize()) );

				// Posible especificación: que el resumen sea serializable y persistente
				ArsCadena cad(ConversorDatos::ByteToStr(resumen->getBuffer(), resumen->getSize()));

				VALIDAR_IGUALES( RESUMEN, cad.cadena() );
			}
			__finally {
				delete resumen2;
				delete resumen;
			}

			// si resumimos un nulo, debe retornar nulo
			resumen = obj->resumir(NULL);
			VALIDAR(resumen == NULL);

			// si resumimos vacío, debe retornar vacío nulo
			resumen = obj->resumir("");
			VALIDAR(resumen == NULL);

		}
		__finally {
			delete obj;
		}
	}
	__finally {
		delete tarjeta;
	}
}


void FirmadorCLTest::testFirmarDetached()
{
	Firma		*firma;

	firma = generarFirma();
	try
	{
		VALIDAR(firma != NULL);

		ArsCadena cad(firma->serializar());

		VALIDAR( cad.longitud() > 0 );
		// ojo: cryptLib mete la fecha de la firma, por lo que no es posible comparar
		// la firma generada con un patrón.
	}
	__finally {
		delete firma;
	}
}


void FirmadorCLTest::testValidarFirma()
{
	Firma 		*firma;
	Certificado	*cert;
	Firmador	*obj;
	Tarjeta		*tarjeta;

	cert = new CertCL(0, 0);
	try
	{
		VALIDAR(cert->deserializar(CERTIFICADO_ENCODED));

		firma = new FirmaDetached();
		try
		{
			VALIDAR( firma->deserializar(FIRMA) );

			tarjeta = createTarjeta();
			try
			{
				obj = tarjeta->createFirmador();
				try
				{
					// correcto
					VALIDAR( obj->validar(TEXTO, *firma, cert) );
					// incorrecto
					VALIDAR( !obj->validar("san perico de los palotes", *firma, cert) );
				}
				__finally {
					delete obj;
				}
			}
			__finally {
				delete tarjeta;
			}
		}
		__finally {
			delete firma;
		}
	}
	__finally {
		delete cert;
	}
}

