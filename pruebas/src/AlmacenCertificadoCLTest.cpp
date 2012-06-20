#include "../../h/AlmacenCertificadoCL.h"
#include "../../h/CertCL.h"

#include "../h/AlmacenCertificadoCLTest.h"

#include "TestSuite.h"

#include "../../h/Certificado.h"
#include "../../h/AlmacenCertificado.h"


#undef  TEST_TOOLKIT_CAPI
#define TEST_TOOLKIT_CL

#undef  TEST_CERTIFICADO_CAMERFIRMA
#undef  TEST_CERTIFICADO_CAMERFIRMA_PRUEBAS
#define TEST_CERTIFICADO_GVA_1
#undef  TEST_CERTIFICADO_GVA_PRUEBA_1

#define	TEST_TARJETA_CERES
#undef	TEST_TARJETA_ETOKEN
#undef	TEST_TARJETA_SIEMENS
#undef  TEST_TARJETA_STARCOS

#include "../h/DatosTarjeta.h"
#include "../h/DatosCertificado.h"
#include "../h/DatosFirma.h"



#if defined(TEST_TARJETA_CERES)
#	include "../../h/TarjetaCLCeres.h"
#elif defined(TEST_TARJETA_SIEMENS)
#	include "../../h/TarjetaCLSiemens.h"
#elif defined(TEST_TARJETA_STARCOS)
#	include "../../h/TarjetaCLStarcos.h"
#endif


AlmacenCertificadoCLTest::AlmacenCertificadoCLTest(std::string name)
	: TestCase(name)
{
}

AlmacenCertificadoCLTest::~AlmacenCertificadoCLTest()
{
}



void AlmacenCertificadoCLTest::setUp()
{
	tarjeta = NULL;
	obj = NULL;
}

void AlmacenCertificadoCLTest::tearDown()
{
	if (tarjeta != NULL)
	{
		if (obj != NULL)
			tarjeta->cerrarAlmacen(obj);

		delete tarjeta;
	}
}



Test* AlmacenCertificadoCLTest::suite()
{
	TestSuite *suite = new TestSuite("AlmacenCL (requiere Tarjeta B)");

	suite->addTest( new AlmacenCertificadoCLTestCaller("OpenAlmacen",			&AlmacenCertificadoCLTest::testOpenAlmacen) );
	suite->addTest( new AlmacenCertificadoCLTestCaller("LoadFirstCertificado",	&AlmacenCertificadoCLTest::testLoadFirstCertificado) );
	suite->addTest( new AlmacenCertificadoCLTestCaller("EnumerarCertificados",	&AlmacenCertificadoCLTest::testEnumerarCertificados) );
	suite->addTest( new AlmacenCertificadoCLTestCaller("Añadir CRL",			&AlmacenCertificadoCLTest::testAddCRL) );
	suite->addTest( new AlmacenCertificadoCLTestCaller("abrirAlmacenCRL",		&AlmacenCertificadoCLTest::abrirAlmacenCRL) );

	return (suite);
}



void AlmacenCertificadoCLTest::ValidarCertificado(Certificado* cert)
{
	SYSTEMTIME	st;

	VALIDAR( cert != NULL );
	VALIDAR(dynamic_cast<CertCL*> (cert)->getClavePublica() != 0);

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


Tarjeta* AlmacenCertificadoCLTest::createTarjeta()
{
	Tarjeta* ret;

#if defined(TEST_TARJETA_CERES)
	ret = new TarjetaCLCeres();
#elif defined(TEST_TARJETA_SIEMENS)
	ret = new TarjetaCLSiemens();
#elif defined(TEST_TARJETA_STARCOS)
	ret = new TarjetaCLStarcos();
#else
	VALIDAR_CON_MENSAJE(false, "No se ha configurado ninguna tarjeta");
#endif

	return (ret);
}

bool AlmacenCertificadoCLTest::conectarTarjeta()
{
	tarjeta = createTarjeta();
	if ( tarjeta->conectar() )
		obj = tarjeta->abrirAlmacen("");
	else
		obj = NULL;

	return (tarjeta != NULL && obj != NULL);
}


void AlmacenCertificadoCLTest::testOpenAlmacen()
{
	VALIDAR( conectarTarjeta() );

	VALIDAR( obj->open() );
	VALIDAR( obj->open() );

	VALIDAR_IGUALES(1, obj->getNumeroCertificados());

	VALIDAR( obj->close() );
	VALIDAR( obj->close() );
}


void AlmacenCertificadoCLTest::testLoadFirstCertificado()
{
	ArsCadena	aux, msg, label(ETIQUETA_CERTIFICADO);
	Certificado	*cert;
	bool pinOK;

	VALIDAR( conectarTarjeta() );

	VALIDAR_IGUALES(1, obj->getNumeroCertificados());

	// antes de establecer el PIN debe encontrar el certificado, pero sin cargar la
	// clave privada.
	cert = obj->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
	if (cert == NULL && label != "")
		cert = obj->loadCertificado(AlmacenCertificado::ClaveBusquedaNombre, &label);

	VALIDAR(NULL != cert);
	try
	{
		VALIDAR(   dynamic_cast<CertCL*> (cert)->getClavePublica() != 0 );
		VALIDAR( ! dynamic_cast<CertCL*> (cert)->tieneClavePrivada() );
	}
	__finally {
		delete cert;
	}

	pinOK = tarjeta->establecerPIN(PIN_CORRECTO, msg);
	VALIDAR_CON_MENSAJE(pinOK, msg.cadena());

	// ahora sí que debe cargar tanto la clave privada como la pública
	cert = obj->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
	if (cert == NULL && label != "")
		cert = obj->loadCertificado(AlmacenCertificado::ClaveBusquedaNombre, &label);
		
	VALIDAR(cert != NULL);
	try
	{
		VALIDAR( dynamic_cast<CertCL*> (cert)->tieneClavePrivada() );
		ValidarCertificado(cert);
	}
	__finally {
		delete cert;
	}
}


void AlmacenCertificadoCLTest::testEnumerarCertificados()
{
	Certificado	*cert;
	int			total;
	bool		pinOK;
	ArsCadena	msg;

	VALIDAR( conectarTarjeta() );

	// debe funcionar antes de establecer el PIN
	VALIDAR_IGUALES(1, obj->getNumeroCertificados());

	pinOK = tarjeta->establecerPIN(PIN_CORRECTO, msg);
	VALIDAR_CON_MENSAJE(pinOK, msg.cadena());

	total = 0;
	cert = NULL;
	do
	{
		cert = obj->enumCertificados(cert);
		if (cert != NULL)
		{
			ValidarCertificado(cert);
			total++;
		}
	}
	while (cert != NULL);

	VALIDAR_IGUALES(1, total);
	VALIDAR_IGUALES(1, obj->getNumeroCertificados());
}


void AlmacenCertificadoCLTest::abrirAlmacenCRL()
{
	AlmacenCertificado *almacenCRL;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	almacenCRL = new AlmacenCertificadoCL(".\\testData\\no_existe");
	try
	{
		// debe retornar -1 porque está cerrado
		VALIDAR_IGUALES(-1, almacenCRL->getNumeroCertificados() );

		// no debe fallar porque la ruta del almacén no exista
		VALIDAR( almacenCRL->open() );

		// debe retornar 0 porque no hay crls (la ruta no existe)
		VALIDAR_IGUALES(0, almacenCRL->getNumeroCertificados() );

		VALIDAR( almacenCRL->open() );
		VALIDAR( almacenCRL->close() );

		// debe retornar -1 porque está cerrado
		VALIDAR_IGUALES(-1, almacenCRL->getNumeroCertificados() );

		VALIDAR( almacenCRL->close() );
	}
	__finally {
		delete almacenCRL;
	}

	almacenCRL = new AlmacenCertificadoCL(".\\testData");
	try
	{
		// debe retornar -1 porque está cerrado
		VALIDAR_IGUALES(-1, almacenCRL->getNumeroCertificados() );

		// no debe fallar porque la ruta del almacén no exista
		VALIDAR( almacenCRL->open() );

		// debe retornar 1 porque hay una crls (gva.crl)
		VALIDAR_IGUALES(1, almacenCRL->getNumeroCertificados() );

		VALIDAR( almacenCRL->open() );
		VALIDAR( almacenCRL->close() );

		// debe retornar -1 porque está cerrado
		VALIDAR_IGUALES(-1, almacenCRL->getNumeroCertificados() );

		VALIDAR( almacenCRL->close() );
	}
	__finally {
		delete almacenCRL;
	}

}

void AlmacenCertificadoCLTest::testAddCRL()
{
	AlmacenCertificado *almacenCRL;
	void*		datos;
	DWORD		size, lenDatos;
	HANDLE		hFile;

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	almacenCRL = new AlmacenCertificadoCL(".\\testData\\");
	try
	{
		VALIDAR( almacenCRL->open() );
		try
		{
			VALIDAR_IGUALES(1, almacenCRL->getNumeroCertificados());

			// CRL que no exista
			almacenCRL->eliminarCRL("perico.crl");
			VALIDAR_IGUALES(1, almacenCRL->getNumeroCertificados());


			hFile = CreateFile(FICHERO_CRL, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
			VALIDAR(hFile != INVALID_HANDLE_VALUE);
			try
			{
				size = GetFileSize(hFile, NULL);

				datos = new BYTE[size];
				try
				{
					ReadFile(hFile, datos, size, &lenDatos, NULL);
					VALIDAR( almacenCRL->almacenarCRL("gva_new.crl", datos, lenDatos) );
					try
					{
						VALIDAR_IGUALES(2, almacenCRL->getNumeroCertificados());
						VALIDAR( FileExists(".\\testData\\gva_new.crl") );
					}
					__finally
					{
						almacenCRL->eliminarCRL(".\\testData\\gva_new.crl");
					}

					VALIDAR_IGUALES(1, almacenCRL->getNumeroCertificados());
					VALIDAR( !FileExists("gva_new.crl") );
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


