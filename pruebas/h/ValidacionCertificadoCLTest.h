#ifndef ValidacionCertificadoCLTestH
#define ValidacionCertificadoCLTestH

#include "../../h/cryptLibAPI.h"

#include "TestCase.h"
#include "TestCaller.h"


class Tarjeta;
class AlmacenCertificado;
class ValidacionCertificado;


class ValidacionCertificadoCLTest : public TestCase
{
public:
	ValidacionCertificadoCLTest(std::string name);
	virtual ~ValidacionCertificadoCLTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testCertificadoValido();
	void testCertificadoRevocado();
	void testCertificadoCaducado();
	void testValidarAlmacenCerrado();

private:
	bool conectarTarjeta();
	void desconectarTarjeta();

	Tarjeta					*tarjeta;
	AlmacenCertificado		*almacenValidacion;
	AlmacenCertificado		*almacenCertificados;
	ValidacionCertificado	*obj;
};


typedef TestCaller<ValidacionCertificadoCLTest> ValidacionCertificadoCLTestCaller;


#endif
