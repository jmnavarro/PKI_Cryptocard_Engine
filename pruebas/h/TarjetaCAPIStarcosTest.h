#if !defined(_TARJETA_CAPI_STARCOS_TEST_H_)
#define _TARJETA_CAPI_STARCOS_TEST_H_

#include "TestCase.h"
#include "TestCaller.h"

#include "../h/TarjetaCAPITest.h"


class TarjetaCAPIStarcosTest : public TarjetaCAPITest
{
public:
	TarjetaCAPIStarcosTest(std::string name);
	virtual ~TarjetaCAPIStarcosTest();

	static Test *suite();

protected:
	virtual TarjetaCAPI* createTarjeta();

	virtual ArsCadena getPINCorrecto();
	virtual ArsCadena getPINErroneo();
	virtual ArsCadena getPINErroneoCorto();
	virtual ArsCadena getPINErroneoLargo();

	void testPedirPIN();
};


typedef TestCaller<TarjetaCAPIStarcosTest> TarjetaCAPIStarcosTestCaller;


#endif


