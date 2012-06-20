#ifndef TarjetaCAPIeTTestH
#define TarjetaCAPIeTTestH

#include "TestCase.h"
#include "TestCaller.h"
#include "../h/TarjetaCAPIeT.h"


class TarjetaCAPIeTTest : public TestCase
{
public:
	TarjetaCAPIeTTest(std::string name);
	virtual ~TarjetaCAPIeTTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testPedirPIN();

private:
	TarjetaCAPIeT  *obj;
};


typedef TestCaller<TarjetaCAPIeTTest> TarjetaCAPIeTTestCaller;


#endif

 