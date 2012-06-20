#if !defined(_TARJETA_CL_SIEMENS_TEST_H_)
#define _TARJETA_CL_SIEMENS_TEST_H_

#include "../../h/TarjetaCLSiemens.h"

#include "TestCase.h"
#include "TestCaller.h"


class TarjetaCLSiemensTest : public TestCase
{
public:
	TarjetaCLSiemensTest(std::string name);
	virtual ~TarjetaCLSiemensTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testConectar();
	void testPedirPIN();
	void testCambioTarjeta();

private:
	TarjetaCLSiemens  *obj;
};


typedef TestCaller<TarjetaCLSiemensTest> TarjetaCLSiemensTestCaller;


#endif
 