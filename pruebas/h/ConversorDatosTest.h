//+++===================================================================================
// ConversorDatosTest
//
// Descripcion:
//		Pruebas unitarias de la clase ConversorDatos
//		Las pruebas utilizan los ficheros auxiliares datos_cod.txt y datos.txt
//
// Autor   Fecha       Observaciones
// ------  ----------  -----------------------------------------------------------------
// JM      18/11/2003  Versión inicial
//
//======================================================================================
#ifndef ConversorDatosTestH
#define ConversorDatosTestH


#include "TestCase.h"
#include "TestCaller.h"
#include "../h/ConversorDatos.h"


class ConversorDatosTest : public TestCase
{
public:
	ConversorDatosTest(std::string name);
	virtual ~ConversorDatosTest();

	static Test *suite();

protected:
	void testByteToStr();

	void testStrToByte();
	void testStrToBytePtr();
	void testStrToByteUnicode();
	void testStrToByteEspacios();
	void testByteToBase64();
	void testBase64ToByte();
	void testCadenaAUnicode();
	void testUnicodeACadena();
};

typedef TestCaller<ConversorDatosTest> ConversorDatosTestCaller;

#endif
