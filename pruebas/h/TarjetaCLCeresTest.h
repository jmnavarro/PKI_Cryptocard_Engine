#ifndef TarjetaCLCERESTestH
#define TarjetaCLCERESTestH

#include "../../h/TarjetaCLCeres.h"

#include "TestCase.h"
#include "TestCaller.h"


class TarjetaCLCeresTest : public TestCase
{
public:
	TarjetaCLCeresTest(std::string name);
	virtual ~TarjetaCLCeresTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testConectar();
	void testPedirPIN();
	void testCambioTarjeta();

private:
	TarjetaCLCeres  *obj;
};


typedef TestCaller<TarjetaCLCeresTest> TarjetaCLCeresTestCaller;


#endif
 