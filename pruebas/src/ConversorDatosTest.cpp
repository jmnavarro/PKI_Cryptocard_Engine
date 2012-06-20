#include "../h/ConversorDatosTest.h"
#include "TestSuite.h"


ConversorDatosTest::ConversorDatosTest(std::string name)
	: TestCase(name)
{
}

ConversorDatosTest::~ConversorDatosTest()
{
}

//--------------------------------------------------------------------------------------

Test* ConversorDatosTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (ConversorDatosTest::*metodo)();
	} Entrada;

	Entrada pruebas[] =
	{
		  {"testByteToStr",		&ConversorDatosTest::testByteToStr}
		, {"testStrToByte",		&ConversorDatosTest::testStrToByte}
		, {"testStrToBytePtr",	&ConversorDatosTest::testStrToBytePtr}
		, {"testStrToByteUnicode",&ConversorDatosTest::testStrToByteUnicode}
		, {"testStrToByteEspacios",&ConversorDatosTest::testStrToByteEspacios}
		, {"testByteToBase64",	&ConversorDatosTest::testByteToBase64}
		, {"testBase64ToByte",	&ConversorDatosTest::testBase64ToByte}
		, {"testCadenaAUnicode",&ConversorDatosTest::testCadenaAUnicode}
		, {"testUnicodeACadena",&ConversorDatosTest::testUnicodeACadena}
	};

	int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("ConversorDatos");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new ConversorDatosTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}

//--------------------------------------------------------------------------------------

void ConversorDatosTest::testByteToStr()
{
	const int NUM_BYTES = 32;
	const int NUM_CHARS = NUM_BYTES * 2;
	BYTE datos[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F
	};
	char cadena[NUM_CHARS+1] = {
		'0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
		'0','A','0','B','0','C','0','D','0','E','0','F',
		'1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
		'1','A','1','B','1','C','1','D','1','E','1','F', '\0'
	};

	ArsCadena ret = ConversorDatos::ByteToStr(datos, NUM_BYTES);

	assertEquals(NUM_CHARS, ret.longitud());

	assert( 0 == strcmp(ret.cadena(), cadena) );
}


void ConversorDatosTest::testStrToByte()
{
	const int NUM_BYTES = 32;

	BYTE datos_ok[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F
	};
	ArsCadena cadena("000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F");
	BYTE datos[NUM_BYTES];

	ZeroMemory(datos, NUM_BYTES);

	ConversorDatos::StrToByte(cadena, datos, NUM_BYTES);

	assert( 0 == memcmp(datos, datos_ok, NUM_BYTES) );
}


void ConversorDatosTest::testStrToBytePtr()
{
	const int NUM_BYTES = 32;

	BYTE datos_ok[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F
	};
	char *cadena = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F";
	BYTE datos[NUM_BYTES];

	ZeroMemory(datos, NUM_BYTES);

	ConversorDatos::StrToByte(cadena, datos, NUM_BYTES);

	assert( 0 == memcmp(datos, datos_ok, NUM_BYTES) );
}


void ConversorDatosTest::testStrToByteUnicode()
{
	const int NUM_BYTES = 30;

	BYTE datos_ok[NUM_BYTES] = {
		0x00,0x01,	0x00,0x02,	0x00,0x03,	0x00,0x04,	0x00,0x05,	0x00,0x06,
		0x00,0x07,	0x00,0x08,	0x00,0x09,	0x00,0x0A,	0x00,0x0B,	0x00,0x0C,
		0x00,0x0D,	0x00,0x0E,	0x00,0x0F
	};
	wchar_t *cadena = L"000100020003000400050006000700080009000A000B000C000D000E000F";
	BYTE datos[NUM_BYTES];

	ZeroMemory(datos, NUM_BYTES);

	ConversorDatos::StrToByte(cadena, datos, NUM_BYTES);

	assert( 0 == memcmp(datos, datos_ok, NUM_BYTES) );
}


void ConversorDatosTest::testStrToByteEspacios()
{
	const int NUM_BYTES = 32;
	const int NUM_BYTES_UNICODE = 30;

	BYTE datos_ok[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F
	};
	BYTE datos_ok_unicode[NUM_BYTES_UNICODE] = {
		0x00,0x01,	0x00,0x02,	0x00,0x03,	0x00,0x04,	0x00,0x05,	0x00,0x06,
		0x00,0x07,	0x00,0x08,	0x00,0x09,	0x00,0x0A,	0x00,0x0B,	0x00,0x0C,
		0x00,0x0D,	0x00,0x0E,	0x00,0x0F
	};
	ArsCadena cadena("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F");
	wchar_t *cadena_unicode = L"0001 0002 0003 0004 0005 0006 0007 0008 0009 000A 000B 000C 000D 000E 000F";
	BYTE datos[NUM_BYTES];

	ZeroMemory(datos, NUM_BYTES);
	ConversorDatos::StrToByte(cadena, datos, NUM_BYTES);
	assert( 0 == memcmp(datos, datos_ok, NUM_BYTES) );

	ZeroMemory(datos, NUM_BYTES);
	ConversorDatos::StrToByte(cadena.cadena(), datos, NUM_BYTES);
	assert( 0 == memcmp(datos, datos_ok, NUM_BYTES) );

	ZeroMemory(datos, NUM_BYTES);
	ConversorDatos::StrToByte(cadena_unicode, datos, NUM_BYTES_UNICODE);
	assert( 0 == memcmp(datos, datos_ok_unicode, NUM_BYTES_UNICODE) );
}


void ConversorDatosTest::testByteToBase64()
{
	char*	DATOS = "Esta es la cadena";
	char*	B64   = "RXN0YSBlcyBsYSBjYWRlbmE=";
	ArsCadena	resultado;

	resultado = ConversorDatos::ByteToBase64(DATOS, 0);
	assertEquals("", resultado.cadena());

	resultado = ConversorDatos::ByteToBase64(NULL, 0);
	assertEquals("", resultado.cadena());

	resultado = ConversorDatos::ByteToBase64("", 0);
	assertEquals("", resultado.cadena());

	resultado = ConversorDatos::ByteToBase64("", 1);
	assertEquals("", resultado.cadena());

	resultado = ConversorDatos::ByteToBase64("", 10);
	assertEquals("", resultado.cadena());

	resultado = ConversorDatos::ByteToBase64(DATOS, strlen(DATOS));
	assertEquals(B64, resultado.cadena());
}


void ConversorDatosTest::testBase64ToByte()
{
	char*		DATOS = "Esta es la cadena";
	char*		B64   = "RXN0YSBlcyBsYSBjYWRlbmE=";
	ArsCadena	resultado;
	BYTE		bytes[256];

	bytes[0] = '!';
	ConversorDatos::Base64ToByte(B64, bytes, 0);
	assert(bytes[0] == '!');

	bytes[0] = '!';
	ConversorDatos::Base64ToByte(B64, NULL, 0);
	assert(bytes[0] == '!');

	ConversorDatos::Base64ToByte("", bytes, 256);
	assert(bytes[0] == '\0');

	ConversorDatos::Base64ToByte("", bytes, 0);
	assert(bytes[0] == '\0');

	ConversorDatos::Base64ToByte("", NULL, 0);
	assert(bytes[0] == '\0');

	ConversorDatos::Base64ToByte(B64, bytes, strlen(B64));
	assertEquals(strlen(DATOS), strlen(bytes));
	assert(memcmp(bytes, DATOS, strlen(DATOS)) == 0);
}


void ConversorDatosTest::testCadenaAUnicode()
{
	wchar_t*	UNICODE = L"Esta es la cadena";
	char*		ASCII   = "Esta es la cadena";

	wchar_t *aux;

	aux = ConversorDatos::ConvertirCadenaAUnicode(ASCII);

	try {
		assert(0 == wcscmp(aux, UNICODE));
	}
	__finally {
		delete [] aux;
	}

	aux = ConversorDatos::ConvertirCadenaAUnicode("");

	try {
		assert(0 == wcscmp(aux, L""));
	}
	__finally {
		delete [] aux;
	}

	aux = ConversorDatos::ConvertirCadenaAUnicode(NULL);
	assert(aux == NULL);
}


void ConversorDatosTest::testUnicodeACadena()
{
	wchar_t*	UNICODE = L"Esta es la cadena";
	char*		ASCII   = "Esta es la cadena";

	char *aux;

	aux = ConversorDatos::ConvertirUnicodeACadena(UNICODE);

	try {
		assertEquals(aux, ASCII);
	}
	__finally {
		delete [] aux;
	}

	aux = ConversorDatos::ConvertirUnicodeACadena(L"");

	try {
		assert(aux[0] == '\0');
		assertEquals(aux, "");
	}
	__finally {
		delete [] aux;
	}

	aux = ConversorDatos::ConvertirUnicodeACadena(NULL);

	assert(aux == NULL);

}




