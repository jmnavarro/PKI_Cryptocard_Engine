#ifndef AlmacenCLTestH
#define AlmacenCLTestH


#include "TestCase.h"
#include "TestCaller.h"


class Tarjeta;
class AlmacenCertificado;


class AlmacenCertificadoCLTest : public TestCase
{
public:
	AlmacenCertificadoCLTest(std::string name);
	virtual ~AlmacenCertificadoCLTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testOpenAlmacen();
	void testLoadFirstCertificado();
	void testEnumerarCertificados();
	void testAddCRL();
	void abrirAlmacenCRL();

private:
	void ValidarCertificado(Certificado* cert);
	bool conectarTarjeta();
	Tarjeta* createTarjeta();

	AlmacenCertificado	*obj;
	Tarjeta				*tarjeta;
};


typedef TestCaller<AlmacenCertificadoCLTest> AlmacenCertificadoCLTestCaller;


#endif
