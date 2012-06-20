#include "../h/CertCLTest.h"

#include "TestSuite.h"

#include "../../h/AlmacenCertificadoCL.h"
#include "../../h/CertCL.h"
#include "../../../LibUtils/h/ArsCadena.h"

#define TEST_TOOLKIT_CL

#undef  TEST_CERTIFICADO_CAMERFIRMA
#undef TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define  TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1
#include "../h/DatosCertificado.h"

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef TEST_TARJETA_STARCOS
#include "../h/DatosTarjeta.h"

#if defined(TEST_TARJETA_CERES)
#	include "../../h/TarjetaCLCeres.h"
#elif defined(TEST_TARJETA_SIEMENS)
#	include "../../h/TarjetaCLSiemens.h"
#elif defined(TEST_TARJETA_STARCOS)
#	include "../../h/TarjetaCLStarcos.h"
#endif



CertCLTest::CertCLTest(std::string name)
	: TestCase(name)
{
}


CertCLTest::~CertCLTest()
{
}


void CertCLTest::setUp()
{
}


void CertCLTest::tearDown()
{
}


Test* CertCLTest::suite()
{
	TestSuite *suite = new TestSuite("CertCL (requiere Tarjeta B)");

	suite->addTest( new CertCLTestCaller("Serializar",		&CertCLTest::testSerializar) );
	suite->addTest( new CertCLTestCaller("Deserializar",	&CertCLTest::testDeserializar) );
	suite->addTest( new CertCLTestCaller("CrearDesdeBuffer",&CertCLTest::testCrearDesdeBuffer) );
	suite->addTest( new CertCLTestCaller("Clonar",			&CertCLTest::testClonar) );

	return (suite);
}

void CertCLTest::ValidarCertificado(Certificado* cert)
{
	SYSTEMTIME	st;

	VALIDAR( cert != NULL );

	st = cert->getFechaDesde();
	VALIDAR_IGUALES(DESDE_ANIO, st.wYear);
	VALIDAR_IGUALES(DESDE_MES, st.wMonth);
	VALIDAR_IGUALES(DESDE_DIA, st.wDay);
	VALIDAR_IGUALES(DESDE_HORAS, st.wHour);
	VALIDAR_IGUALES(DESDE_MINUTOS, st.wMinute);
	VALIDAR_IGUALES(DESDE_SEGUNDOS, st.wSecond);

	st = cert->getFechaHasta();
	VALIDAR_IGUALES(HASTA_ANIO, st.wYear);
	VALIDAR_IGUALES(HASTA_MES, st.wMonth);
	VALIDAR_IGUALES(HASTA_DIA, st.wDay);
	VALIDAR_IGUALES(HASTA_HORAS, st.wHour);
	VALIDAR_IGUALES(HASTA_MINUTOS, st.wMinute);
	VALIDAR_IGUALES(HASTA_SEGUNDOS, st.wSecond);

	VALIDAR_IGUALES(NOMBRE_CERTIFICADO, cert->getNombre().cadena());
	VALIDAR_IGUALES(EMISOR_CERTIFICADO, cert->getEmisor().cadena());
	VALIDAR_IGUALES(NOMBRE_EMISOR_CERTIFICADO, cert->getNombreEmisor().cadena());
	VALIDAR_IGUALES(NUMERO_SERIE_CERTIFICADO, cert->getNumeroSerie().cadena());
}


Tarjeta* CertCLTest::createTarjeta()
{
	Tarjeta* tarjeta;

#if defined(TEST_TARJETA_CERES)
	tarjeta = new TarjetaCLCeres();
#elif defined(TEST_TARJETA_SIEMENS)
	tarjeta = new TarjetaCLSiemens();
#elif defined(TEST_TARJETA_STARCOS)
	tarjeta = new TarjetaCLStarcos();
#else
	VALIDAR_CON_MENSAJE(false, "No se ha configurado una tarjeta");
#endif

	return (tarjeta);
}


void CertCLTest::testSerializar()
{
	ArsCadena	label;
	Tarjeta		*tarjeta;
	AlmacenCertificado *alm;
	Certificado	*cert;

	ArsCadena	encoded;
	ArsCadena	msg;
	bool		pinOK;

	tarjeta = createTarjeta();
	try
	{
		VALIDAR( tarjeta->conectar() );
		alm = tarjeta->abrirAlmacen("");
		VALIDAR(alm != NULL);
		try
		{
			pinOK = tarjeta->establecerPIN(PIN_CORRECTO, msg);
			VALIDAR_CON_MENSAJE(pinOK, msg.cadena());

			cert = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
			if (cert == NULL && ETIQUETA_CERTIFICADO[0] != '\0')
			{
				label = ETIQUETA_CERTIFICADO;
				cert = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaNombre, &label);
			}

			VALIDAR(cert != NULL);
			try
			{
				ValidarCertificado(cert);

				encoded = cert->serializar();
				VALIDAR(encoded != "");

				ArsCadena aux(CERTIFICADO_ENCODED);

				VALIDAR_IGUALES(aux.longitud(), encoded.longitud());
				VALIDAR_IGUALES(aux.cadena(), encoded.cadena());
				VALIDAR_IGUALES(CERTIFICADO_ENCODED, encoded.cadena());
			}
			__finally
			{
				delete cert;
			}
		}
		__finally
		{
			delete alm;
		}
	}
	__finally
	{
		delete tarjeta;
	}
}


void CertCLTest::testDeserializar()
{
	ArsCadena	encoded;
	ArsCadena	newEncoded;
	Certificado	*cert;

	cert = new CertCL(0, 0);
	try
	{
		VALIDAR( cert->deserializar(CERTIFICADO_ENCODED) );
		ValidarCertificado(cert);

		newEncoded = cert->serializar();
		VALIDAR(newEncoded != "");
		VALIDAR_IGUALES(CERTIFICADO_ENCODED, newEncoded.cadena());
	}
	__finally
	{
		delete cert;
	}
}


void CertCLTest::testCrearDesdeBuffer()
{
	Tarjeta		*tarjeta;
	AlmacenCertificado	*alm;
	Certificado	*certNew;
	Certificado *certOri;

	bool pinOK;
	ArsCadena msg;
	ArsCadena	encoded;
	ArsCadena	substr(SUBSTR_CERTIFICADO);


	tarjeta = createTarjeta();
	try
	{
		VALIDAR( tarjeta->conectar() );
		alm = tarjeta->abrirAlmacen("");
		VALIDAR(alm != NULL);
		try
		{
			pinOK = tarjeta->establecerPIN(PIN_CORRECTO, msg);
			VALIDAR_CON_MENSAJE(pinOK, msg.cadena());

			certOri = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
			if (certOri == NULL && ETIQUETA_CERTIFICADO[0] != '\0')
			{
				ArsCadena label(ETIQUETA_CERTIFICADO);
				certOri = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaNombre, &label);
			}
			VALIDAR(certOri != NULL);
			try
			{
				ValidarCertificado(certOri);

				encoded = certOri->serializar();
				VALIDAR(encoded != "");
				VALIDAR_IGUALES(CERTIFICADO_ENCODED, encoded.cadena());

				certNew = new CertCL(encoded);
				try
				{
					VALIDAR(dynamic_cast<CertCL*> (certNew)->getClavePublica() != 0);
					VALIDAR(dynamic_cast<CertCL*> (certNew)->getClavePrivada() == 0);
					ValidarCertificado(certNew);
				}
				__finally {
					delete certNew;
				}
			}
			__finally {
				delete certOri;
			}
		}
		__finally {
			delete alm;
		}
	}
	__finally {
		delete tarjeta;
	}
}



void CertCLTest::testClonar()
{
	Tarjeta		*tarjeta;
	AlmacenCertificado	*alm;
	Certificado	*certCopia;
	Certificado *certOri;

	bool pinOK;
	ArsCadena msg;
	ArsCadena	encoded;
	ArsCadena	substr(SUBSTR_CERTIFICADO);


	tarjeta = createTarjeta();
	try
	{
		VALIDAR( tarjeta->conectar() );
		alm = tarjeta->abrirAlmacen("");
		VALIDAR(alm != NULL);
		try
		{
			pinOK = tarjeta->establecerPIN(PIN_CORRECTO, msg);
			VALIDAR_CON_MENSAJE(pinOK, msg.cadena());

			certOri = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
			if (certOri == NULL && ETIQUETA_CERTIFICADO[0] != '\0')
			{
				ArsCadena label(ETIQUETA_CERTIFICADO);
				certOri = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaNombre, &label);
			}
			VALIDAR(certOri != NULL);
			try
			{
				ValidarCertificado(certOri);

				certCopia = certOri->clone();
				try
				{
					// en la clonación, se mantiene la clave pública, pero no la privada
					// (por limitación de cryptLib)
					VALIDAR(dynamic_cast<CertCL*> (certCopia)->getClavePublica() != 0);
					VALIDAR(dynamic_cast<CertCL*> (certCopia)->getClavePrivada() == 0);
					ValidarCertificado(certCopia);
				}
				__finally {
					delete certCopia;
				}
			}
			__finally {
				delete certOri;
				certOri = NULL;
			}
		}
		__finally {
			delete alm;
		}
	}
	__finally {
		delete tarjeta;
	}
}


