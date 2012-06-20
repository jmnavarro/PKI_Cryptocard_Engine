#ifndef FirmadorCLTestH
#define FirmadorCLTestH

#include "../../h/FirmadorCL.h"

#include "TestCase.h"
#include "TestCaller.h"


class Firma;
class Certificado;
class Tarjeta;


class FirmadorCLTest : public TestCase
{
public:
	FirmadorCLTest(std::string name);
	virtual ~FirmadorCLTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testResumir();
	void testFirmarDetached();
	void testValidarFirma();

private:
	Firma*		generarFirma();
	Tarjeta*	createTarjeta();

};


typedef TestCaller<FirmadorCLTest> FirmadorCLTestCaller;


#endif
