#if !defined(_TARJETA_CAPI_SIEMENS_TEST_H_)
#define _TARJETA_CAPI_SIEMENS_TEST_H_

#include "TestCase.h"
#include "TestCaller.h"

#include "../h/TarjetaCAPITest.h"


class TarjetaCAPISiemensTest : public TarjetaCAPITest
{
public:
	TarjetaCAPISiemensTest(std::string name);
	virtual ~TarjetaCAPISiemensTest();

	static Test *suite();

protected:
	virtual TarjetaCAPI* createTarjeta();

	virtual ArsCadena getPINCorrecto();
	virtual ArsCadena getPINErroneo();
	virtual ArsCadena getPINErroneoCorto();
	virtual ArsCadena getPINErroneoLargo();

	void testPedirPIN();
};


typedef TestCaller<TarjetaCAPISiemensTest> TarjetaCAPISiemensTestCaller;


#endif
 
