//+++===================================================================================
// BufferDinamicoTest
//
// Descripcion:
//		Pruebas unitarias de la clase BufferDinamico
//
// Autor   Fecha       Observaciones
// ------  ----------  -----------------------------------------------------------------
// JM      21/11/2003  Versión inicial
//
//======================================================================================
#ifndef BufferDinamicoTestH
#define BufferDinamicoTestH

#include "TestCase.h"
#include "TestCaller.h"
#include "../h/BufferDinamico.h"
//--------------------------------------------------------------------------------------

#define NUM_BYTES	30
#define POTENCIA_2	32

//--------------------------------------------------------------------------------------

class BufferDinamicoPublico;


class BufferDinamicoTest : public TestCase
{
public:
	BufferDinamicoTest(std::string name);
	virtual ~BufferDinamicoTest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testCrear();
	void testLiberar();
	void testAmpliar();
	void testReducir();
	void testSetSize();
	void testBufferNulo();
	void testOperadorAsignacion();
	void testOperadorComparacion();

private:
	static const BYTE DATOS[NUM_BYTES];

	BufferDinamicoPublico	*obj;
};

// inicializadores estáticos
const BYTE BufferDinamicoTest::DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D
	};

//--------------------------------------------------------------------------------------

//
// Esta clase sirve para publicar los métodos protected de "BufferDinamico" y así
// poder consultarlos durante las comprobaciones.
//
class BufferDinamicoPublico : public BufferDinamico
{
public:
	DWORD getSizeBuffer() const
	{
		return BufferDinamico::getSizeBuffer();
	}

	const BufferDinamico& operator = (const BufferDinamico &newData)
	{
		return BufferDinamico::operator = (newData);
	}

	bool operator == (const BufferDinamico &dataComparacion) const
	{
		return BufferDinamico::operator == (dataComparacion);
	}

	bool operator != (const BufferDinamico &dataComparacion) const
	{
		return BufferDinamico::operator != (dataComparacion);
	}

};


typedef TestCaller<BufferDinamicoTest> BufferDinamicoTestCaller;



#endif
