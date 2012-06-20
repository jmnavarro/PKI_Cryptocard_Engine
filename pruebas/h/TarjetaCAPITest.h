#if !defined(_TARJETA_CAPI_TEST_H_)
#define _TARJETA_CAPI_TEST_H_

#include "TestCase.h"
#include "TestCaller.h"

#include "../../../LibUtils/h/ArsCadena.h"


class TarjetaCAPI;


class TarjetaCAPITest : public TestCase
{
public:
	TarjetaCAPITest(std::string name);
	virtual ~TarjetaCAPITest();

	void setUp();
	void tearDown();

protected:
	virtual TarjetaCAPI* createTarjeta() = 0;

	virtual ArsCadena getPINCorrecto() = 0;
	virtual ArsCadena getPINErroneo() = 0;
	virtual ArsCadena getPINErroneoCorto() = 0;
	virtual ArsCadena getPINErroneoLargo() = 0;

	void probarPIN();

private:
	TarjetaCAPI  *fixture;
};


#endif
 