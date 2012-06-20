#if !defined(_TARJETA_CAPI_CERES_TEST_H_)
#define _TARJETA_CAPI_CERES_TEST_H_

#include "TestCase.h"
#include "TestCaller.h"

#include "../h/TarjetaCAPITest.h"


class TarjetaCAPICeresTest : public TarjetaCAPITest
{
public:
	TarjetaCAPICeresTest(std::string name);
	virtual ~TarjetaCAPICeresTest();

	static Test *suite();

protected:
	virtual TarjetaCAPI* createTarjeta();

	virtual ArsCadena getPINCorrecto();
	virtual ArsCadena getPINErroneo();
	virtual ArsCadena getPINErroneoCorto();
	virtual ArsCadena getPINErroneoLargo();

	void testPedirPIN();
};


typedef TestCaller<TarjetaCAPICeresTest> TarjetaCAPICeresTestCaller;


#endif
