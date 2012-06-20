//+++===================================================================================
// FirmaDetachedTest
//
// Descripcion:
//		Pruebas unitarias de la clase FirmaDetached.
//
// Autor   Fecha       Observaciones
// ------  ----------  -----------------------------------------------------------------
// JM      24/11/2003  Versión inicial
//
//======================================================================================
#ifndef FirmaDetachedTestH
#define FirmaDetachedTestH

#include "TestCase.h"
#include "TestCaller.h"
#include "../h/FirmaDetached.h"
//--------------------------------------------------------------------------------------

class FirmaDetachedTest : public TestCase
{
public:
	FirmaDetachedTest(std::string name);
	virtual ~FirmaDetachedTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testSerializar();
	void testDeserializar();
	void testLoadFromFile();
	void testSaveToFile();
	void testLoadFromBinaryFile();
	void testSaveToBinaryFile();

private:
	bool compararArchivos(const char *a1, const char *a2);

	Firma  *obj;
};


typedef TestCaller<FirmaDetachedTest> FirmaDetachedTestCaller;

#endif
