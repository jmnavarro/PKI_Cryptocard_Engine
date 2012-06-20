/**
 * Pruebas unitarias de la clase CertCAPI.
 * Para ejecutar estas pruebas es necesario tener instalado el certificado de
 * demostración de Camerfirma.
 * Este certificado se puede descargar desde la web de Carmerfirma.
 *
 * @author JM
 *
 * @date 09/01/2004  - <i>JM</i> - Versión inicial
 * @date 22/12/2004  - <i>JM</i> - Método builder para crear la tarjeta
 * @date 08/03/2005  - <i>JM</i> - Nueva prueba para validar firmas serializadas
**/
#if !defined(_FIRMADOR_CAPI_TEST_H_)
#define _FIRMADOR_CAPI_TEST_H_

#include "TestCase.h"
#include "TestCaller.h"


class Firma;
class Certificado;
class TarjetaCAPI;


class FirmadorCAPITest : public TestCase
{
public:
	FirmadorCAPITest(std::string name);
	virtual ~FirmadorCAPITest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testResumir();
	void testFirmarDetached();
	void testValidarFirma();
	void testValidarFirmaCertificadosGVA();
	void testValidarFirmaSerializada();

private:
	Firma* generarFirma(Certificado** cert = NULL);
	TarjetaCAPI* createTarjeta();
	bool validarFirmasGVA(TarjetaCAPI* tarjeta, Certificado* cert);

};


typedef TestCaller<FirmadorCAPITest> FirmadorCAPITestCaller;


#endif
