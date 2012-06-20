//--------------------------------------------------------------------------------------
#include "BufferDinamicoTest.h"
#include "TestSuite.h"
//--------------------------------------------------------------------------------------

BufferDinamicoTest::BufferDinamicoTest(std::string name)
	: TestCase(name)
{
}

BufferDinamicoTest::~BufferDinamicoTest()
{
}

//--------------------------------------------------------------------------------------

void BufferDinamicoTest::setUp()
{
	obj = new BufferDinamicoPublico();
}

void BufferDinamicoTest::tearDown()
{
	delete obj;
	obj = NULL;
}

//--------------------------------------------------------------------------------------

Test* BufferDinamicoTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (BufferDinamicoTest::*metodo)();
	} Entrada;

	Entrada pruebas[] =
	{
		{"testCrear",		&BufferDinamicoTest::testCrear},
		{"testLiberar",		&BufferDinamicoTest::testLiberar},
		{"testAmpliar",		&BufferDinamicoTest::testAmpliar},
		{"testReducir",		&BufferDinamicoTest::testReducir},
		{"testSetSize",		&BufferDinamicoTest::testSetSize},
		{"testBufferNulo",	&BufferDinamicoTest::testBufferNulo},
		{"testOperadorAsignacion",	&BufferDinamicoTest::testOperadorAsignacion},
		{"testOperadorComparacion",	&BufferDinamicoTest::testOperadorComparacion}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("BufferDinamico");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new BufferDinamicoTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}

//--------------------------------------------------------------------------------------

void BufferDinamicoTest::testCrear()
{
	obj->setBuffer(DATOS, NUM_BYTES);
	try
	{
		assertEquals(NUM_BYTES, obj->getSize());
		assertEquals(POTENCIA_2, obj->getSizeBuffer());
		assert( 0 == memcmp(DATOS, obj->getBuffer(), NUM_BYTES) );
	}
	__finally {
		obj->freeBuffer();
	}
}


void BufferDinamicoTest::testLiberar()
{
	obj->setBuffer(DATOS, NUM_BYTES);
	try
	{
		assertEquals(NUM_BYTES, obj->getSize());
		assertEquals(POTENCIA_2, obj->getSizeBuffer());
		assert( 0 == memcmp(DATOS, obj->getBuffer(), NUM_BYTES) );
	}
	__finally
	{
		obj->freeBuffer();

		assert(obj->getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}


void BufferDinamicoTest::testAmpliar()
{
	const int NUM_MAS_BYTES = NUM_BYTES + 10;

	BYTE masDatos[NUM_MAS_BYTES] = {
		0xFF,	0xFE,	0xFD,	0xFC,	0xFB,	0xFA,	0xF9,	0xF8,	0xF7,	0xF6,
		0xFF,	0xFE,	0xFD,	0xFC,	0xFB,	0xFA,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19
	};

	obj->setBuffer(DATOS, NUM_BYTES);
	try
	{
		assertEquals(NUM_BYTES, obj->getSize());
		assertEquals(POTENCIA_2, obj->getSizeBuffer());
		assert( 0 == memcmp(DATOS, obj->getBuffer(), NUM_BYTES) );

		obj->setBuffer(masDatos, NUM_MAS_BYTES);

		// el tamaño de los datos cambia y el tamaño del buffer se duplica 
		assertEquals(NUM_MAS_BYTES, obj->getSize());
		assertEquals(POTENCIA_2 * 2, obj->getSizeBuffer());
		assert( 0 == memcmp(masDatos, obj->getBuffer(), NUM_MAS_BYTES) );
	}
	__finally
	{
		obj->freeBuffer();

		assert(obj->getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}


void BufferDinamicoTest::testReducir()
{
	const int NUM_MENOS_BYTES = NUM_BYTES - 6;

	BYTE menosDatos[NUM_MENOS_BYTES] = {
		0xFF,	0xFE,	0xFD,	0xFC,	0xFB,	0xFA,	0xF9,	0xF8,	0xF7,	0xF6,
		0xFF,	0xFE,	0xFD,	0xFC,	0xFB,	0xFA,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17
	};

	obj->setBuffer(DATOS, NUM_BYTES);
	try
	{
		assertEquals(NUM_BYTES, obj->getSize());
		assert( 0 == memcmp(DATOS, obj->getBuffer(), NUM_BYTES) );

		obj->setBuffer(menosDatos, NUM_MENOS_BYTES);

		// el tamaño de los datos cambia, pero el tamaño del buffer se mantiene
		assertEquals(NUM_MENOS_BYTES, obj->getSize());
		assertEquals(POTENCIA_2, obj->getSizeBuffer());
		assert( 0 == memcmp(menosDatos, obj->getBuffer(), NUM_MENOS_BYTES) );
	}
	__finally
	{
		obj->freeBuffer();

		assert(obj->getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}


void BufferDinamicoTest::testSetSize()
{
	BYTE unDato[1] = {0xFF};

	obj->setSize(NUM_BYTES);
	try
	{
		// ojo: el tamaño de los datos permanece inalterado pero el del buffer cambia
		assertEquals(0, obj->getSize());
		assertEquals(NUM_BYTES, obj->getSizeBuffer());
		assert( obj->getBuffer() != NULL );

		obj->setBuffer(unDato, 1);

		// ahora ha cambiado el tamaño, porque la longitud de los datos ha cambiado
		// el buffer permanece igual, porque no ha necesitado crecer
		assertEquals(1, obj->getSize());
		assertEquals(NUM_BYTES, obj->getSizeBuffer());
		assert( obj->getBuffer() != NULL );
		assert( 0 == memcmp(unDato, obj->getBuffer(), obj->getSize()) );

		// el tamaño de los datos debe permanecer inalterado pero el del buffer debe
		// cambiar al indicado
		obj->setSize(NUM_BYTES*2 + 99);

		assertEquals(1, obj->getSize());
		assertEquals(NUM_BYTES*2 + 99, obj->getSizeBuffer());
		assert( obj->getBuffer() != NULL );
		assert( 0 == memcmp(unDato, obj->getBuffer(), obj->getSize()) );

		obj->setSize(NUM_BYTES/2);

		assertEquals(1, obj->getSize());
		assertEquals(NUM_BYTES/2, obj->getSizeBuffer());
		assert( obj->getBuffer() != NULL );
		assert( 0 == memcmp(unDato, obj->getBuffer(), obj->getSize()) );
	}
	__finally
	{
		obj->freeBuffer();

		assert(obj->getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}


void BufferDinamicoTest::testBufferNulo()
{
	// pasar NULL en setBuffer
	obj->setBuffer(DATOS, NUM_BYTES);
	try
	{
		obj->setBuffer(NULL, NUM_BYTES);

		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
		assert( obj->getBuffer() == NULL );
	}
	__finally
	{
		obj->freeBuffer();

		assert(obj->getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}

	// pasar 0 en setBuffer
	obj->setBuffer(DATOS, NUM_BYTES);
	try
	{
		obj->setBuffer(DATOS, 0);

		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
		assert( obj->getBuffer() == NULL );
	}
	__finally
	{
		obj->freeBuffer();

		assert(obj->getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}


void BufferDinamicoTest::testOperadorAsignacion()
{
	const int NUM_MENOS_BYTES = 10;
	const int MENOS_POTENCIA_2 = 16;

	BufferDinamicoPublico	obj2;
	BYTE menosDatos[NUM_MENOS_BYTES] = {
		0xFF,	0xFE,	0xFD,	0xFC,	0xFB,	0xFA,	0xF9,	0xF8,	0xF7,	0xF6
	};

	obj2.setBuffer(menosDatos, NUM_MENOS_BYTES);
	try
	{
		assertEquals(NUM_MENOS_BYTES, obj2.getSize());
		assertEquals(MENOS_POTENCIA_2, obj2.getSizeBuffer());
		assert( 0 == memcmp(menosDatos, obj2.getBuffer(), obj2.getSize()) );

		obj->setBuffer(DATOS, NUM_BYTES);
		try
		{
			assertEquals(NUM_BYTES, obj->getSize());
			assertEquals(POTENCIA_2, obj->getSizeBuffer());
			assert( 0 == memcmp(DATOS, obj->getBuffer(), NUM_BYTES) );

			// hay que deshacer la referencia de "obj" para que actúe el operador asignación,
			// si no, se haría una asignación de punteros corriente y molinete.
			(*obj) = obj2;

			assertEquals(NUM_MENOS_BYTES, obj->getSize());
			assertEquals(MENOS_POTENCIA_2, obj->getSizeBuffer());
			assert( 0 == memcmp(menosDatos, obj->getBuffer(), obj2.getSize()) );
		}
		__finally
		{
			obj->freeBuffer();

			assert(obj->getBuffer() == NULL);
			assertEquals(0, obj->getSize());
			assertEquals(0, obj->getSizeBuffer());
		}
	}
	__finally
	{
		obj2.freeBuffer();

		assert(obj2.getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}


void BufferDinamicoTest::testOperadorComparacion()
{
	const int NUM_MENOS_BYTES = 10;
	const int MENOS_POTENCIA_2 = 16;

	BufferDinamicoPublico	obj2;
	BYTE menosDatos[NUM_MENOS_BYTES] = {
		0xFF,	0xFE,	0xFD,	0xFC,	0xFB,	0xFA,	0xF9,	0xF8,	0xF7,	0xF6
	};
	BYTE otrosDatos[NUM_BYTES] = { /* sólo cambia el primer byte */
		0xFF,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D
	};

	obj2.setBuffer(DATOS, NUM_BYTES);
	try
	{
		assertEquals(NUM_BYTES, obj2.getSize());
		assertEquals(POTENCIA_2, obj2.getSizeBuffer());
		assert( 0 == memcmp(DATOS, obj2.getBuffer(), obj2.getSize()) );

		//
		// comparar cuando son iguales
		//
		obj->setBuffer(DATOS, NUM_BYTES);
		try
		{
			assertEquals(NUM_BYTES, obj->getSize());
			assertEquals(POTENCIA_2, obj->getSizeBuffer());
			assert( 0 == memcmp(DATOS, obj->getBuffer(), obj->getSize()) );

			// hay que deshacer la referencia de "obj" para que actúe el operador, si no,
			// se haría una comparación de punteros corriente y molinete.
			assert( (*obj) == obj2 );
		}
		__finally
		{
			obj->freeBuffer();

			assert(obj->getBuffer() == NULL);
			assertEquals(0, obj->getSize());
			assertEquals(0, obj->getSizeBuffer());
		}


		//
		// comparar cuando los datos son distintos
		//
		obj->setBuffer(otrosDatos, NUM_BYTES);
		try
		{
			assertEquals(NUM_BYTES, obj->getSize());
			assertEquals(POTENCIA_2, obj->getSizeBuffer());
			assert( 0 == memcmp(otrosDatos, obj->getBuffer(), obj->getSize()) );

			// hay que deshacer la referencia de "obj" para que actúe el operador, si no,
			// se haría una comparación de punteros corriente y molinete.
			assert( !((*obj) == obj2) );
			assert(   (*obj) != obj2 );
		}
		__finally {
			obj->freeBuffer();

			assert(obj->getBuffer() == NULL);
			assertEquals(0, obj->getSize());
			assertEquals(0, obj->getSizeBuffer());
		}


		//
		// comparar cuando las longitudes son distintas
		//
		obj->setBuffer(menosDatos, NUM_MENOS_BYTES);
		try
		{
			assertEquals(NUM_MENOS_BYTES, obj->getSize());
			assertEquals(MENOS_POTENCIA_2, obj->getSizeBuffer());
			assert( 0 == memcmp(menosDatos, obj->getBuffer(), obj->getSize()) );

			// hay que deshacer la referencia de "obj" para que actúe el operador, si no,
			// se haría una comparación de punteros corriente y molinete.
			assert( !((*obj) == obj2) );
			assert(   (*obj) != obj2 );
		}
		__finally {
			obj->freeBuffer();

			assert(obj->getBuffer() == NULL);
			assertEquals(0, obj->getSize());
			assertEquals(0, obj->getSizeBuffer());
		}


		//
		// comparar contra sí mismo
		//
		assert( obj2 == obj2 );

	}
	__finally
	{
		obj2.freeBuffer();

		assert(obj2.getBuffer() == NULL);
		assertEquals(0, obj->getSize());
		assertEquals(0, obj->getSizeBuffer());
	}
}

