#include "../h/CertCAPITest.h"

#include "TestSuite.h"

#include "../../h/CertCAPI.h"
#include "../../h/BufferDinamico.h"
#include "../../h/AlmacenCertificadoCAPI.h"
#include "../../../LibUtils/h/ArsCadena.h"

#undef  TEST_CERTIFICADO_CAMERFIRMA
#undef  TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1

#define TEST_TOOLKIT_CAPI
#undef  TEST_TOOLKIT_CL

#include "../h/DatosCertificado.h"



CertCAPITest::CertCAPITest(std::string name)
	: TestCase(name)
{
}


CertCAPITest::~CertCAPITest()
{
}


void CertCAPITest::setUp()
{
}


void CertCAPITest::tearDown()
{
}


Test* CertCAPITest::suite()
{
	TestSuite *suite = new TestSuite("CertCAPI");

	suite->addTest( new CertCAPITestCaller("testSerializar",		&CertCAPITest::testSerializar) );
	suite->addTest( new CertCAPITestCaller("testDeserializar",		&CertCAPITest::testDeserializar) );
	suite->addTest( new CertCAPITestCaller("testCrearDesdeBuffer",	&CertCAPITest::testCrearDesdeBuffer) );

	return (suite);
}


void CertCAPITest::testSerializar()
{
	ArsCadena	encoded;
	Certificado	*cert;
	AlmacenCertificado *alm;
	ArsCadena	substr(SUBSTR_CERTIFICADO);

	alm = new AlmacenCertificadoCAPI("MY");
	try
	{
		VALIDAR( alm->open() );
		try
		{
			cert = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &substr);
			VALIDAR(cert != NULL);
			try
			{
				encoded = cert->serializar();
				VALIDAR(encoded != "");
				VALIDAR_IGUALES(CERTIFICADO_ENCODED, encoded.cadena());
			}
			__finally {
				delete cert;
			}
		}
		__finally {
			VALIDAR( alm->close() );
		}
	}
	__finally {
		delete alm;
	}
}


void CertCAPITest::testDeserializar()
{
	ArsCadena	encoded;
	Certificado	*cert;
	ArsCadena	substr(SUBSTR_CERTIFICADO);
	SYSTEMTIME	st;
	char		aux[32];

	cert = new CertCAPI((PCCERT_CONTEXT)0);
	try
	{
		VALIDAR( cert->deserializar(CERTIFICADO_ENCODED) );

		VALIDAR_IGUALES(NOMBRE_CERTIFICADO, cert->getNombre().cadena());
		VALIDAR_IGUALES(NOMBRE_EMISOR_CERTIFICADO, cert->getNombreEmisor().cadena());
		VALIDAR_IGUALES(EMISOR_CERTIFICADO, cert->getEmisor().cadena());
		VALIDAR_IGUALES(NUMERO_SERIE_CERTIFICADO, cert->getNumeroSerie().cadena());

		st = cert->getFechaDesde();
		sprintf(aux, "%02d/%02d/%04d", st.wDay, st.wMonth, st.wYear);
		VALIDAR_IGUALES(FECHA_DESDE, aux);

		st = cert->getFechaHasta();
		sprintf(aux, "%02d/%02d/%04d", st.wDay, st.wMonth, st.wYear);
		VALIDAR_IGUALES(FECHA_HASTA, aux);
	}
	__finally
	{
		delete cert;
	}
}


void CertCAPITest::testCrearDesdeBuffer()
{
	ArsCadena	encoded;
	Certificado	*certNew, *certOri;
	AlmacenCertificado	*alm;
	ArsCadena	substr(SUBSTR_CERTIFICADO);
	SYSTEMTIME	st;
	char		aux[32];

	alm = new AlmacenCertificadoCAPI("MY");
	try
	{
		VALIDAR( alm->open() );

		certOri = alm->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &substr);
		VALIDAR(certOri != NULL);
		try
		{
			encoded = certOri->serializar();
			VALIDAR(encoded != "");
			VALIDAR_IGUALES(CERTIFICADO_ENCODED, encoded.cadena());

			certNew = new CertCAPI(encoded);
			try
			{
				VALIDAR(dynamic_cast<CertCAPI*> (certNew)->getHandle() != 0);

				VALIDAR_IGUALES(NOMBRE_CERTIFICADO, certNew->getNombre().cadena());
				VALIDAR_IGUALES(NOMBRE_EMISOR_CERTIFICADO, certNew->getNombreEmisor().cadena());
				VALIDAR_IGUALES(EMISOR_CERTIFICADO, certNew->getEmisor().cadena());
				VALIDAR_IGUALES(NUMERO_SERIE_CERTIFICADO, certNew->getNumeroSerie().cadena());

				st = certNew->getFechaDesde();
				sprintf(aux, "%02d/%02d/%04d", st.wDay, st.wMonth, st.wYear);
				VALIDAR_IGUALES(FECHA_DESDE, aux);

				st = certNew->getFechaHasta();
				sprintf(aux, "%02d/%02d/%04d", st.wDay, st.wMonth, st.wYear);
				VALIDAR_IGUALES(FECHA_HASTA, aux);
			}
			__finally
			{
				delete certNew;
			}
		}
		__finally
		{
			delete certOri;
		}
	}
	__finally
	{
		delete alm;
	}

}


