#ifndef ValidacionCertificadoCAPITestH
#define ValidacionCertificadoCAPITestH

#include "TestCase.h"
#include "TestCaller.h"

										   
class AlmacenCertificado;
class ValidacionCertificado;

class ValidacionCertificadoCAPITest : public TestCase
{
public:
	ValidacionCertificadoCAPITest(std::string name);
	virtual ~ValidacionCertificadoCAPITest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testEstaRevocado();
	void testEstaCaducado();
	void testValidarAlmacenCerrado();

private:
	void RegistrarCRL();

	AlmacenCertificado	  *almacenValidacion;
	AlmacenCertificado	  *almacenCertificados;
	ValidacionCertificado *obj;
};


typedef TestCaller<ValidacionCertificadoCAPITest> ValidacionCertificadoCAPITestCaller;


#endif
