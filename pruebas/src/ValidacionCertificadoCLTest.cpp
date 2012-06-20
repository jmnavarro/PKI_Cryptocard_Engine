#include "../h/ValidacionCertificadoCLTest.h"

#include "TestSuite.h"

#include "../../h/ValidacionCertificado.h"
#include "../../h/AlmacenCertificadoCL.h"
#include "../../h/Certificado.h"
#include "../../h/TarjetaCLCeres.h"

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

#undef  TEST_TOOLKIT_CAPI
#define TEST_TOOLKIT_CL
#include "../h/DatosFirma.h"



ValidacionCertificadoCLTest::ValidacionCertificadoCLTest(std::string name)
	: TestCase(name)
{
}

ValidacionCertificadoCLTest::~ValidacionCertificadoCLTest()
{
}



void ValidacionCertificadoCLTest::setUp()
{
	almacenValidacion = new AlmacenCertificadoCL(ALMACEN_VALIDACION);
	almacenValidacion->open();

	obj = almacenValidacion->createValidacionCertificado();

	tarjeta = NULL;
	almacenCertificados = NULL;
}

void ValidacionCertificadoCLTest::tearDown()
{
	if (obj != NULL)
	{
		delete obj;
		obj = NULL;
	}

	if (almacenValidacion != NULL)
	{
		almacenValidacion->close();
		delete almacenValidacion;
	}

	desconectarTarjeta();
}

//--------------------------------------------------------------------------------------

Test* ValidacionCertificadoCLTest::suite()
{
	TestSuite *suite = new TestSuite("ValidacionCertificadoCL (requiere tarjetas C, D y E)");

	suite->addTest( new ValidacionCertificadoCLTestCaller("ValidarAlmacenCerrado", &ValidacionCertificadoCLTest::testValidarAlmacenCerrado) );
	suite->addTest( new ValidacionCertificadoCLTestCaller("CertificadoValido", &ValidacionCertificadoCLTest::testCertificadoValido) );
	suite->addTest( new ValidacionCertificadoCLTestCaller("CertificadoRevocado", &ValidacionCertificadoCLTest::testCertificadoRevocado) );
	suite->addTest( new ValidacionCertificadoCLTestCaller("CertificadoCaducado", &ValidacionCertificadoCLTest::testCertificadoCaducado) );

	return (suite);
}


bool ValidacionCertificadoCLTest::conectarTarjeta()
{
	bool ret;

	// tarjeta y almacenCertificados se liberan en el tearDown
	tarjeta = new TarjetaCLCeres();
	ret = tarjeta->conectar();

	if (ret)
	{
		almacenCertificados = tarjeta->abrirAlmacen("");
		if (almacenCertificados == NULL)
		{
			desconectarTarjeta();
			ret = false;
		}
	}
	else
	{
		delete tarjeta;
		tarjeta = NULL;
	}

	return (ret);
}


void ValidacionCertificadoCLTest::desconectarTarjeta()
{
	if (tarjeta != NULL)
	{
		if (almacenCertificados != NULL)
			tarjeta->cerrarAlmacen(almacenCertificados);

		tarjeta->desconectar();
		delete tarjeta;
		tarjeta = NULL;
	}
}


void ValidacionCertificadoCLTest::testCertificadoValido()
{
	Certificado	*cert;
	int resp;

	resp = MessageBox(GetActiveWindow(), "Introduce la tarjeta \"D\"...", "Introducir tarjeta", MB_ICONINFORMATION + MB_OKCANCEL);
	if (resp == IDOK)
	{
		VALIDAR( conectarTarjeta() );

		cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
		try	{
			VALIDAR_IGUALES(ValidacionCertificado::EstadoRevValido, obj->estaRevocado(cert));
			VALIDAR_IGUALES(ValidacionCertificado::EstadoCadValido, obj->estaCaducado(cert));
		}
		__finally {
			delete cert;
		}
	}
}


void ValidacionCertificadoCLTest::testCertificadoRevocado()
{
	Certificado	*cert;
	int resp;

	resp = MessageBox(GetActiveWindow(), "Introduce la tarjeta \"E\"...", "Introducir tarjeta", MB_ICONINFORMATION + MB_OKCANCEL);
	if (resp == IDOK)
	{
		VALIDAR( conectarTarjeta() );

		cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
		try	{
			VALIDAR_IGUALES(ValidacionCertificado::EstadoRevRevocado, obj->estaRevocado(cert));
			VALIDAR_IGUALES(ValidacionCertificado::EstadoCadValido,   obj->estaCaducado(cert));
		}
		__finally {
			delete cert;
		}
	}
}


void ValidacionCertificadoCLTest::testCertificadoCaducado()
{
	Certificado	*cert;
	int resp;
	bool tarjetaConectada;

	resp = MessageBox(GetActiveWindow(), "Introduce la tarjeta \"C\"...", "Introducir tarjeta", MB_ICONINFORMATION + MB_OKCANCEL);
	if (resp == IDOK)
	{
		VALIDAR( conectarTarjeta() );

		cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
		try
		{
			VALIDAR_IGUALES(ValidacionCertificado::EstadoRevValido,	  obj->estaRevocado(cert));
			VALIDAR_IGUALES(ValidacionCertificado::EstadoCadCaducado, obj->estaCaducado(cert));
		}
		__finally {
			delete cert;
		}
	}
}


void ValidacionCertificadoCLTest::testValidarAlmacenCerrado()
{
	AlmacenCertificado		*almacen;
	ValidacionCertificado	*validador;
	Certificado				*cert;

	// debe estar disponible una tarjeta con certificado válido

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());

	almacen = new AlmacenCertificadoCL(ALMACEN_VALIDACION);
	try
	{
		validador = almacen->createValidacionCertificado();
		try
		{
			conectarTarjeta();

			cert = almacenCertificados->loadCertificado(AlmacenCertificado::ClaveBusquedaPrimero, NULL);
			try
			{
				// la revocación requiere que el almacén esté abierto. La caducidad no.
				VALIDAR_IGUALES( ValidacionCertificado::EstadoRevError, validador->estaRevocado(cert) );
				VALIDAR_IGUALES( ValidacionCertificado::EstadoCadValido, validador->estaCaducado(cert) );
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

