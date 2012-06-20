#include "../h/FirmadorCAPITest.h"

#include "TestSuite.h"

#include "../../h/FirmadorCAPI.h"
#include "../../h/AlmacenCertificado.h"
#include "../../h/Certificado.h"
#include "../../h/CertCAPI.h"
#include "../../h/FirmaDetached.h"
#include "../../h/ConversorDatos.h"


#define TEST_TOOLKIT_CAPI
#undef  TEST_TOOLKIT_CL

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS

#undef  TEST_CERTIFICADO_CAMERFIRMA
#undef  TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1

#include "../h/DatosTarjeta.h"
#include "../h/DatosCertificado.h"
#include "../h/DatosFirma.h"


//
// configuramos el tipo de tarjeta a utilizar en el test:
//
#if defined(TEST_TARJETA_CERES)
#	include "../../h/TarjetaCAPICeres.h"
#elif defined(TEST_TARJETA_ETOKEN)
#	include "../../h/TarjetaCAPIeT.h"
#elif defined(TEST_TARJETA_SIEMENS)
#	include "../../h/TarjetaCAPISiemens.h"
#elif defined(TEST_TARJETA_STARCOS)
#	include "../../h/TarjetaCAPIStarcos.h"
#endif



FirmadorCAPITest::FirmadorCAPITest(std::string name)
	: TestCase(name)
{
}

FirmadorCAPITest::~FirmadorCAPITest()
{
}


void FirmadorCAPITest::setUp()
{
}

void FirmadorCAPITest::tearDown()
{
}


Test* FirmadorCAPITest::suite()
{
	TestSuite *suite = new TestSuite("FirmadorCAPI");

	suite->addTest( new FirmadorCAPITestCaller("Resumir",		 		&FirmadorCAPITest::testResumir) );
	suite->addTest( new FirmadorCAPITestCaller("FirmarDetached", 		&FirmadorCAPITest::testFirmarDetached) );
	suite->addTest( new FirmadorCAPITestCaller("ValidarFirma",   		&FirmadorCAPITest::testValidarFirma) );
	suite->addTest( new FirmadorCAPITestCaller("ValidarFirmaSerializada",&FirmadorCAPITest::testValidarFirmaSerializada) );
#if defined(TEST_TARJETA_SIEMENS)
	suite->addTest( new FirmadorCAPITestCaller("ValidarFirmaCertificadosGVA",   		&FirmadorCAPITest::testValidarFirmaCertificadosGVA) );
#endif

	return (suite);
}


Firma* FirmadorCAPITest::generarFirma(Certificado** cert)
{
	ArsCadena	cad(SUBSTR_CERTIFICADO);
	ArsCadena	msg;
	Tarjeta		*tarjeta;
	Firma 		*firma;
	AlmacenCertificado *store;
	Firmador	*obj;
	Certificado	*certTmp;

	firma = NULL;
	certTmp = NULL;

	tarjeta = createTarjeta();
	try
	{
		store = tarjeta->abrirAlmacen("");
		VALIDAR(store != NULL);
		try
		{
			// para crear el certificado lo tengo que hacer a través del almacén
			certTmp = store->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &cad);
			VALIDAR(certTmp != NULL);
			try
			{
				VALIDAR( tarjeta->conectar(certTmp) );
				VALIDAR( tarjeta->establecerPIN(PIN_CORRECTO, msg) );

				obj = tarjeta->createFirmador();
				try
				{
					firma = obj->firmar(Firmador::TipoFirmaDetached, TEXTO, *certTmp);
				}
				__finally {
					delete obj;
				}
			}
			__finally {
				if (cert != NULL)
					*cert = certTmp->clone();

				delete certTmp;
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


TarjetaCAPI* FirmadorCAPITest::createTarjeta()
{
	TarjetaCAPI	*ret;

#if defined(TEST_TARJETA_CERES)
	ret = new TarjetaCAPICeres();
#elif defined(TEST_TARJETA_ETOKEN)
	ret = new TarjetaCAPIeT();
#elif defined(TEST_TARJETA_SIEMENS)
	ret = new TarjetaCAPISiemens();
#elif defined(TEST_TARJETA_STARCOS)
	ret = new TarjetaCAPIStarcos();
#else
	ret = NULL;
#endif

	return (ret);
}


void FirmadorCAPITest::testResumir()
{
	Tarjeta		*tarjeta;
	AlmacenCertificado *store;
	Certificado	*cert;
	Firmador	*obj;
	ArsCadena	cad(SUBSTR_CERTIFICADO);
	ArsCadena	msg;

	tarjeta = createTarjeta();
	VALIDAR(tarjeta != NULL);
	try
	{
		store = tarjeta->abrirAlmacen("");
		VALIDAR(store != NULL);
		try
		{
			Resumen	*resumen = NULL;
			Resumen *resumen2 = NULL;

			// para crear el certificado lo tengo que hacer a través del almacén
			cert = store->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &cad);
			VALIDAR(cert != NULL);
			try
			{
//				ArsCadena a = cert->serializar();

				VALIDAR( tarjeta->conectar(cert) );

				obj = tarjeta->createFirmador();
				try
				{
					resumen  = obj->resumir(TEXTO);
					resumen2 = obj->resumir(TEXTO);
					try
					{
						VALIDAR(resumen  != NULL);
						VALIDAR(resumen2 != NULL);

						VALIDAR_IGUALES(resumen->getSize(), resumen2->getSize());
						VALIDAR( 0 == memcmp(resumen->getBuffer(), resumen2->getBuffer(), resumen->getSize()) );

						// Posible especificación: que el resumen sea serializable y persistente
						ArsCadena cad(ConversorDatos::ByteToStr(resumen->getBuffer(), resumen->getSize()));

						VALIDAR( 0 == memcmp(cad.cadena(), RESUMEN, cad.longitud()) );
					}
					__finally {
						delete resumen2;
						delete resumen;
					}

					resumen = obj->resumir(NULL);
					VALIDAR(resumen == NULL);
				}
				__finally
				{
					delete obj;
				}
			}
			__finally
			{
				delete cert;
			}
		}
		__finally
		{
			tarjeta->cerrarAlmacen(store);
		}
	}
	__finally
	{
		delete tarjeta;
	}
}


void FirmadorCAPITest::testFirmarDetached()
{
	ArsCadena	cad(SUBSTR_CERTIFICADO);
	Firma		*firma;

	firma = generarFirma();
	try
	{
		VALIDAR(firma != NULL);

		ArsCadena cad(firma->serializar());

		VALIDAR( cad.longitud() > 0 );
		VALIDAR( 0 == memcmp(cad.cadena(), FIRMA, cad.longitud()) );
	}
	__finally {
		delete firma;
	}
}


void FirmadorCAPITest::testValidarFirmaSerializada()
{
	ArsCadena	msg, cad(SUBSTR_CERTIFICADO);
	Firma 		*firma;
	Certificado	*cert;
	Firmador	*firmador;
	Tarjeta		*tarjeta;

	tarjeta = createTarjeta();
	try
	{
		firma = new FirmaDetached();
		cert  = new CertCAPI(CERTIFICADO_ENCODED);
		try
		{
			VALIDAR(firma != NULL);
			VALIDAR(cert != NULL);

			firma->deserializar(FIRMA);

			firmador = tarjeta->createFirmador();
			try
			{
				// indicando el certificado
				VALIDAR( firmador->validar(TEXTO, *firma, cert) );

				// que utilice el certificado incrustado en el PKCS7
				VALIDAR( firmador->validar(TEXTO, *firma) );

				VALIDAR( !firmador->validar("san perico de los palotes", *firma, cert) );
				VALIDAR( !firmador->validar("san perico de los palotes", *firma) );
			}
			__finally {
				delete firmador;
			}
		}
		__finally
		{
			delete firma;
			delete cert;
		}
	}
	__finally {
		delete tarjeta;
	}
}


void FirmadorCAPITest::testValidarFirma()
{
	ArsCadena	msg, cad(SUBSTR_CERTIFICADO);
	Firma 		*firma;
	Certificado	*cert;
	Firmador	*firmador;
	Tarjeta		*tarjeta;
	AlmacenCertificado *store;
	bool esPINCorrecto;

	tarjeta = createTarjeta();
	VALIDAR(tarjeta != NULL);
	try
	{
		store = tarjeta->abrirAlmacen("");
		VALIDAR(store != NULL);
		try
		{
			cert = store->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &cad);
			VALIDAR( cert != NULL );
			try
			{
				VALIDAR( tarjeta->conectar(cert) );

				esPINCorrecto = tarjeta->establecerPIN(PIN_CORRECTO, msg);
				VALIDAR_CON_MENSAJE(esPINCorrecto, msg.cadena());

				firmador = tarjeta->createFirmador();
				try
				{
					firma = firmador->firmar(Firmador::TipoFirmaDetached, TEXTO, *cert);
					VALIDAR( firma != NULL );
					try
					{
						VALIDAR( firmador->validar(TEXTO, *firma) );
						VALIDAR( firmador->validar(TEXTO, *firma, cert) );

						VALIDAR( !firmador->validar("ja je ji jo ju", *firma) );
						VALIDAR( !firmador->validar("ja je ji jo ju", *firma, cert) );
					}
					__finally {
						delete firma;
					}
				}
				__finally {
					if (firmador != NULL)
						delete firmador;
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
}


bool FirmadorCAPITest::validarFirmasGVA(TarjetaCAPI* tarjeta, Certificado* cert)
{
	Firma 		*firma;
	Firmador	*firmador;
	bool ret;

	ret = true;

	try
	{
		firmador = tarjeta->createFirmador();
		try
		{
			firma = firmador->firmar(Firmador::TipoFirmaDetached, (LPBYTE)TEXTO, strlen(TEXTO), *cert);
			VALIDAR( firma != NULL );
			try
			{
				VALIDAR( firmador->validar(TEXTO, *firma, cert) );
				VALIDAR( firmador->validar(TEXTO, *firma) );

				Tarjeta		*factory = createTarjeta();
				Firmador	*firmador2 = factory->createFirmador();
				Firma		*firma2 = new FirmaDetached(firma->getBuffer(), firma->getSize());
				try
				{
					VALIDAR_IGUALES(firma->getSize(), firma2->getSize());
					VALIDAR((*firma) == (*firma2));

					VALIDAR( firmador->validar(TEXTO, *firma, cert) );
					VALIDAR( firmador->validar(TEXTO, *firma) );

					VALIDAR( firmador2->validar(TEXTO, *firma2, cert) );
					VALIDAR( firmador2->validar(TEXTO, *firma2) );

					VALIDAR( firmador2->validar((LPBYTE)TEXTO, strlen(TEXTO), *firma2, cert) );
					VALIDAR( firmador2->validar((LPBYTE)TEXTO, strlen(TEXTO), *firma2) );

					char* ja = "ja je ji jo ju";
					VALIDAR( !firmador2->validar((LPBYTE)ja, strlen(ja), *firma2) );
					VALIDAR( !firmador2->validar((LPBYTE)ja, strlen(ja), *firma2, cert) );
				}
				__finally
				{
					delete firma2;
					delete firmador2;
					delete factory;
				}
			}
			__finally {
				delete firma;
			}
		}
		__finally {
			delete firmador;
		}
	}
	catch (CppUnitException e) {
		ret = false;
	}

	return (ret);
}


void FirmadorCAPITest::testValidarFirmaCertificadosGVA()
{
	// firma GVA_1
	#define EMISOR_FIRMA	"3051310B3009060355040613024553311F301D060355040A131647656E6572616C697461742056616C656E6369616E61310F300D060355040B1306504B494756413110300E0603550403130743415445535431"
	#define	NUM_SERIE_FIRMA	"AC211342"

	// cifrado GVA_1
	#define EMISOR_CIFRADO		"3051310B3009060355040613024553311F301D060355040A131647656E6572616C697461742056616C656E6369616E61310F300D060355040B1306504B494756413110300E0603550403130743415445535431"
	#define	NUM_SERIE_CIFRADO	"AE211342"
/*
	// firma GVA_2
	#define EMISOR_FIRMA	"3051310B3009060355040613024553311F301D060355040A131647656E6572616C697461742056616C656E6369616E61310F300D060355040B1306504B494756413110300E0603550403130743415445535431"
	#define	NUM_SERIE_FIRMA	"F3231342"

	// cifrado GVA_2
	#define EMISOR_CIFRADO		"3051310B3009060355040613024553311F301D060355040A131647656E6572616C697461742056616C656E6369616E61310F300D060355040B1306504B494756413110300E0603550403130743415445535431"
	#define	NUM_SERIE_CIFRADO	"F7231342"
*/
/*
	// firma e-firma
	#define EMISOR_FIRMA	"3051310B3009060355040613024553311F301D060355040A131647656E6572616C697461742056616C656E6369616E61310F300D060355040B1306504B494756413110300E0603550403130743415445535431"
	#define	NUM_SERIE_FIRMA	"2B10A641"

	// cifrado e-firma
	#define EMISOR_CIFRADO		"3051310B3009060355040613024553311F301D060355040A131647656E6572616C697461742056616C656E6369616E61310F300D060355040B1306504B494756413110300E0603550403130743415445535431"
	#define	NUM_SERIE_CIFRADO	"2E10A641"
*/

	CertCAPI	*certificadoFirma, *certificadoCifrado;
	TarjetaCAPI	*tarjeta;
	AlmacenCertificado *store;
	AlmacenCertificado::NumeroSerie  nsFirma;
	AlmacenCertificado::NumeroSerie  nsCifrado;
	ArsCadena msg;
	bool esPINCorrecto;



	tarjeta = createTarjeta();
	VALIDAR(tarjeta != NULL);
	try
	{
		VALIDAR( tarjeta->conectar(NULL) );

		store = tarjeta->abrirAlmacen("");
		VALIDAR(store != NULL);
		try
		{
			esPINCorrecto = tarjeta->establecerPIN(PIN_CORRECTO, msg);
			VALIDAR_CON_MENSAJE(esPINCorrecto, msg.cadena());

			nsFirma.emisor		= EMISOR_FIRMA;
			nsFirma.numSerie	= NUM_SERIE_FIRMA;
			nsCifrado.emisor	= EMISOR_CIFRADO;
			nsCifrado.numSerie	= NUM_SERIE_CIFRADO;

			certificadoFirma   = (CertCAPI*) store->loadCertificado(AlmacenCertificado::ClaveBusquedaNumSerie, &nsFirma);
			certificadoCifrado = (CertCAPI*) store->loadCertificado(AlmacenCertificado::ClaveBusquedaNumSerie, &nsCifrado);
			VALIDAR( certificadoFirma != NULL );
			VALIDAR( certificadoCifrado != NULL );
			try
			{
				VALIDAR( validarFirmasGVA(tarjeta, certificadoCifrado) );
				VALIDAR( validarFirmasGVA(tarjeta, certificadoFirma) );
			}
			__finally {
				delete certificadoFirma;
				delete certificadoCifrado;
			}
		}
		__finally {
			tarjeta->cerrarAlmacen(store);
		}
	}
	__finally {
		delete tarjeta;
	}
}


