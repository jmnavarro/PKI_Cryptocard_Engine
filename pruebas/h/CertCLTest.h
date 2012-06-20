#ifndef CertCLTestH
#define CertCLTestH


#include "../../h/CertCL.h"

#include "TestCase.h"
#include "TestCaller.h"


class Certificado;
class Tarjeta;


class CertCLTest : public TestCase
{
public:
	CertCLTest(std::string name);
	virtual ~CertCLTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testSerializar();
	void testDeserializar();
	void testCrearDesdeBuffer();
	void testClonar();

private:
	void ValidarCertificado(Certificado* cert);
	Tarjeta* createTarjeta();
};


typedef TestCaller<CertCLTest> CertCLTestCaller;


#endif
