#ifndef TarjetaBuilderCLTestH
#define TarjetaBuilderCLTestH

#include "../../h/TarjetaCL.h"

#include "TestCase.h"
#include "TestCaller.h"


class TarjetaBuilderCLTest : public TestCase
{
public:
	TarjetaBuilderCLTest(std::string name);
	virtual ~TarjetaBuilderCLTest();

	static Test *suite();

protected:
	void testCrearTarjetaCERES();
	void testCrearTarjetaSTARCOS();
	void testCrearTarjetaSIEMENS();

	void testAutodetectarTarjetasSoportadas();
	void testAutodetectarListaTarjetas();

	void testConvertirTipoTarjeta();
};


typedef TestCaller<TarjetaBuilderCLTest> TarjetaBuilderCLTestCaller;


#endif
 