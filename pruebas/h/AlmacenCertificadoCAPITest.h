#ifndef AlmacenCAPITestH
#define AlmacenCAPITestH

#include "TestCase.h"
#include "TestCaller.h"

#include "../h/AlmacenCertificadoCAPI.h"


class Certificado;


class AlmacenCertificadoCAPITest : public TestCase
{
public:
	AlmacenCertificadoCAPITest(std::string name);
	virtual ~AlmacenCertificadoCAPITest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testOpenAlmacen();
	void testLoadCertificadoFromAsunto();
	void testLoadCertificadoFromNumSerie();
	void testEnumerarCertificados();
	void testAddCRL();

private:
	int getNumeroCRL(HCERTSTORE hAlmacen);
	int getNumeroCertificados(HCERTSTORE hAlmacen);
	void validarCertificado(Certificado* cert);

	AlmacenCertificadoCAPI  *obj;
};


typedef TestCaller<AlmacenCertificadoCAPITest> AlmacenCertificadoCAPITestCaller;


#endif
