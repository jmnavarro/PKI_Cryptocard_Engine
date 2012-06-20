#include "../h/AlmacenCertificadoCAPITest.h"

#include "TestSuite.h"

#include "../h/Certificado.h"
#include "../h/AlmacenCertificado.h"

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS

#undef  TEST_CERTIFICADO_CAMERFIRMA
#undef  TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1

#define TEST_TOOLKIT_CAPI
#undef  TEST_TOOLKIT_CL

#include "../h/DatosTarjeta.h"
#include "../h/DatosCertificado.h"
#include "../h/DatosFirma.h"



AlmacenCertificadoCAPITest::AlmacenCertificadoCAPITest(std::string name)
	: TestCase(name)
{
}

AlmacenCertificadoCAPITest::~AlmacenCertificadoCAPITest()
{
}


void AlmacenCertificadoCAPITest::setUp()
{
	obj = new AlmacenCertificadoCAPI(ALMACEN_PRUEBAS);
}

void AlmacenCertificadoCAPITest::tearDown()
{
	delete obj;
	obj = NULL;
}



Test* AlmacenCertificadoCAPITest::suite()
{
	TestSuite *suite = new TestSuite("AlmacenCertificadoCAPI");

	suite->addTest( new AlmacenCertificadoCAPITestCaller("OpenAlmacen",	  				&AlmacenCertificadoCAPITest::testOpenAlmacen) );
	suite->addTest( new AlmacenCertificadoCAPITestCaller("LoadCertificadoFromAsunto",	&AlmacenCertificadoCAPITest::testLoadCertificadoFromAsunto) );
	suite->addTest( new AlmacenCertificadoCAPITestCaller("LoadCertificadoFromNumSerie", &AlmacenCertificadoCAPITest::testLoadCertificadoFromNumSerie) );
	suite->addTest( new AlmacenCertificadoCAPITestCaller("EnumerarCertificados",		&AlmacenCertificadoCAPITest::testEnumerarCertificados) );
	suite->addTest( new AlmacenCertificadoCAPITestCaller("Añadir CRL",					&AlmacenCertificadoCAPITest::testAddCRL) );

	return (suite);
}

int AlmacenCertificadoCAPITest::getNumeroCRL(HCERTSTORE hAlmacen)
{
	int				total;
	PCCRL_CONTEXT	crl;

	crl = ::CertEnumCRLsInStore(hAlmacen, NULL);
	total = 0;

	while (crl != NULL)
	{
		total++;
		crl = ::CertEnumCRLsInStore(hAlmacen, crl);
	}

	return (total);
}

int AlmacenCertificadoCAPITest::getNumeroCertificados(HCERTSTORE hAlmacen)
{
	int				total;
	PCCERT_CONTEXT	cert;

	cert = ::CertEnumCertificatesInStore(hAlmacen, NULL);
	total = 0;

	while (cert != NULL)
	{
		total++;
		cert = ::CertEnumCertificatesInStore(hAlmacen, cert);
	}

	return (total);
}


void AlmacenCertificadoCAPITest::validarCertificado(Certificado* cert)
{
	SYSTEMTIME	st;

	VALIDAR( cert != NULL );

	st = cert->getFechaDesde();
	VALIDAR_IGUALES(DESDE_ANIO, st.wYear);
	VALIDAR_IGUALES(DESDE_MES, st.wMonth);
	VALIDAR_IGUALES(DESDE_DIA, st.wDay);
	VALIDAR_IGUALES(DESDE_HORAS, st.wHour + 2);
	VALIDAR_IGUALES(DESDE_MINUTOS, st.wMinute);
	VALIDAR_IGUALES(DESDE_SEGUNDOS, st.wSecond);

	st = cert->getFechaHasta();
	VALIDAR_IGUALES(HASTA_ANIO, st.wYear);
	VALIDAR_IGUALES(HASTA_MES, st.wMonth);
	VALIDAR_IGUALES(HASTA_DIA, st.wDay);
	VALIDAR_IGUALES(HASTA_HORAS, st.wHour + 2);
	VALIDAR_IGUALES(HASTA_MINUTOS, st.wMinute);
	VALIDAR_IGUALES(HASTA_SEGUNDOS, st.wSecond);

	VALIDAR_IGUALES(NOMBRE_CERTIFICADO, cert->getNombre().cadena());
	VALIDAR_IGUALES(EMISOR_CERTIFICADO, cert->getEmisor().cadena());
	VALIDAR_IGUALES(NOMBRE_EMISOR_CERTIFICADO, cert->getNombreEmisor().cadena());
	VALIDAR_IGUALES(NUMERO_SERIE_CERTIFICADO, cert->getNumeroSerie().cadena());
}


void AlmacenCertificadoCAPITest::testOpenAlmacen()
{
	VALIDAR( obj->open() );
	VALIDAR( obj->getNumeroCertificados() > 0 );
	VALIDAR( obj->close() );
}


void AlmacenCertificadoCAPITest::testLoadCertificadoFromAsunto()
{
	ArsCadena	substr(SUBSTR_CERTIFICADO), aux;
	Certificado	*cert;
	SYSTEMTIME	st;

	VALIDAR( obj->open() );
	VALIDAR( obj->getNumeroCertificados() > 0 );

	cert = obj->loadCertificado(AlmacenCertificado::ClaveBusquedaSubStrAsunto, &substr);
	try
	{
		VALIDAR( cert != NULL );
		validarCertificado(cert);
	}
	__finally {
		delete cert;
	}
}


void AlmacenCertificadoCAPITest::testLoadCertificadoFromNumSerie()
{
	AlmacenCertificado::NumeroSerie serie;
	ArsCadena	aux;
	Certificado	*cert;
	SYSTEMTIME	st;

	serie.emisor	= EMISOR_CERTIFICADO;
	serie.numSerie	= NUMERO_SERIE_CERTIFICADO;

	VALIDAR( obj->open() );
	VALIDAR( obj->getNumeroCertificados() > 0 );

	cert = obj->loadCertificado(AlmacenCertificado::ClaveBusquedaNumSerie, &serie);
	try
	{
		VALIDAR( cert != NULL );
		validarCertificado(cert);
	}
	__finally {
		delete cert;
	}
}


void AlmacenCertificadoCAPITest::testEnumerarCertificados()
{
	Certificado *cert;
	int	total, totalReal;

	VALIDAR( obj->open() );
	VALIDAR( obj->getNumeroCertificados() > 0 );

	cert = obj->enumCertificados(NULL);
	total = 0;
	while (cert)
	{
		total++;
		cert = obj->enumCertificados(cert);
	}

	totalReal = getNumeroCertificados(obj->getHandle());
	VALIDAR_IGUALES(totalReal, total);
}


void AlmacenCertificadoCAPITest::testAddCRL()
{
	AlmacenCertificadoCAPI	*almacenCRL;
	int			total, newTotal;
	void*		datos;
	DWORD		size, lenDatos;
	HANDLE		hFile;

	total = -1;

	almacenCRL = new AlmacenCertificadoCAPI(ALMACEN_CRL);
	try
	{
		VALIDAR( almacenCRL->open() );
		try
		{
			almacenCRL->eliminarCRL(EMISOR_CRL);

			total = getNumeroCRL(almacenCRL->getHandle());

			SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
			hFile = CreateFile(FICHERO_CRL, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			VALIDAR(hFile != INVALID_HANDLE_VALUE);
			try
			{
				size = GetFileSize(hFile, NULL);

				datos = new BYTE[size];
				try
				{
					ReadFile(hFile, datos, size, &lenDatos, NULL);
					VALIDAR( almacenCRL->almacenarCRL(FICHERO_CRL, datos, lenDatos) );
					try
					{
						newTotal = getNumeroCRL(almacenCRL->getHandle());

						VALIDAR_IGUALES(total + 1, newTotal);
					}
					__finally {
						almacenCRL->eliminarCRL(EMISOR_CRL);
					}
					VALIDAR_IGUALES(getNumeroCRL(almacenCRL->getHandle()), total);
				}
				__finally {
					delete [] datos;
				}
			}
			__finally {
				CloseHandle(hFile);
			}
		}
		__finally {
			VALIDAR( almacenCRL->close() );
		}
	}
	__finally {
		delete almacenCRL;
	}

	return;

}


