#if !defined(_TARJETA_CL_STARCOS_TEST_H_)
#define _TARJETA_CL_STARCOS_TEST_H_

#include "../../h/TarjetaCLStarcos.h"

#include "TestCase.h"
#include "TestCaller.h"


class TarjetaCLStarcosTest : public TestCase
{
public:
	TarjetaCLStarcosTest(std::string name);
	virtual ~TarjetaCLStarcosTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testConectar();
	void testPedirPIN();
	void testCambioTarjeta();

private:
	TarjetaCLStarcos  *obj;
};


typedef TestCaller<TarjetaCLStarcosTest> TarjetaCLStarcosTestCaller;


#endif
 