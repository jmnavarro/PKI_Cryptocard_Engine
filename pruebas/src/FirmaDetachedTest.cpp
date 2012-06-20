//---------------------------------------------------------------------------
#include <vcl.h>

#include "FirmaDetachedTest.h"
#include "TestSuite.h"
//---------------------------------------------------------------------------

FirmaDetachedTest::FirmaDetachedTest(std::string name)
	: TestCase(name)
{
}

FirmaDetachedTest::~FirmaDetachedTest()
{
}

//--------------------------------------------------------------------------------------

void FirmaDetachedTest::setUp()
{
	obj = new FirmaDetached();
}

void FirmaDetachedTest::tearDown()
{
	delete obj;
	obj = NULL;
}

//--------------------------------------------------------------------------------------

Test* FirmaDetachedTest::suite()
{
	typedef struct _Entrada
	{
		char *nombre;
		void  (FirmaDetachedTest::*metodo)();
	} Entrada;

	Entrada pruebas[] =
	{
		{"testSerializar",	 &FirmaDetachedTest::testSerializar}
		,{"testDeserializar", &FirmaDetachedTest::testDeserializar}
		,{"testLoadFromFile", &FirmaDetachedTest::testLoadFromFile}
		,{"testSaveToFile",	 &FirmaDetachedTest::testSaveToFile}
		,{"testLoadFromBinaryFile",  &FirmaDetachedTest::testLoadFromBinaryFile}
		,{"testSaveToBinaryFile",	 &FirmaDetachedTest::testSaveToBinaryFile}
	};

	const int TotalEntradas = sizeof(pruebas)/sizeof(Entrada);
	TestSuite *suite = new TestSuite("FirmaDetached");

	for (int i=0; i<TotalEntradas; i++)
		suite->addTest( new FirmaDetachedTestCaller(pruebas[i].nombre, pruebas[i].metodo) );

	return suite;
}

//--------------------------------------------------------------------------------------

bool FirmaDetachedTest::compararArchivos(const char *a1, const char *a2)
{
	bool ret;

	TFileStream	*fs1 = new TFileStream(a1, fmOpenRead);
	TFileStream *fs2 = new TFileStream(a2, fmOpenRead);

	char *buff1 = new char[fs1->Size];
	char *buff2 = new char[fs2->Size];

	try {
		assert( fs1->Size == fs2->Size );

		fs1->Read(buff1, fs1->Size);
		fs2->Read(buff2, fs2->Size);

		ret = (0 == memcmp(buff1, buff2, fs1->Size));
	}
	__finally {
		delete [] buff1;
		delete [] buff2;

		delete fs1;
		delete fs2;
	}

	return ret;
}

//--------------------------------------------------------------------------------------

void FirmaDetachedTest::testSerializar()
{
	const int NUM_BYTES	= 32;
	const BYTE DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F };
	const char CADENA[NUM_BYTES*2+1] = {
		'0','0','0','1','0','2','0','3','0','4','0','5','0','6','0','7','0','8','0','9',
		'0','A','0','B','0','C','0','D','0','E','0','F',
		'1','0','1','1','1','2','1','3','1','4','1','5','1','6','1','7','1','8','1','9',
		'1','A','1','B','1','C','1','D','1','E','1','F', '\0' };

	ArsCadena  result;

	obj->setBuffer(DATOS, NUM_BYTES);

	result = obj->serializar();
	try {
		assert( 0 == strncmp(result.cadena(), CADENA, result.longitud()) );
	}
	__finally {
		obj->freeBuffer();
	}
}

void FirmaDetachedTest::testDeserializar()
{
	const int NUM_BYTES	= 32;
	const BYTE DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F };
	const char* CADENA = "000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F";
	const ArsCadena  serial(CADENA);

	BufferDinamico	buff(DATOS, NUM_BYTES);

	obj->setBuffer(buff);
	try
	{
		assert( obj->deserializar(serial) );
		assert( buff == (*obj) );
	}
	__finally {
		obj->freeBuffer();
	}
}

void FirmaDetachedTest::testLoadFromFile()
{
	const int NUM_BYTES	= 32;
	const BYTE DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F };
	const char* TEST_FILE = ".\\testData\\firma.txt";

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	assert( ERROR_LOAD_FILE != obj->loadFromFile(TEST_FILE) );

	BufferDinamico	buff;
	buff.setBuffer(DATOS, NUM_BYTES);

	assert( buff == (*obj) );
}

void FirmaDetachedTest::testSaveToFile()
{
	const int NUM_BYTES	= 32;
	const BYTE DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F };
	const char* TMP_FILE = ".\\testData\\firma.tmp";
	const char* TEST_FILE = ".\\testData\\firma.txt";

	obj->setBuffer(DATOS, NUM_BYTES);

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	assert( ERROR_SAVE_FILE != obj->saveToFile(TMP_FILE) );
	try {
		assert( compararArchivos(TEST_FILE, TMP_FILE) );
	}
	__finally {
		DeleteFile(TMP_FILE);
	}
}


void FirmaDetachedTest::testLoadFromBinaryFile()
{
	const int NUM_BYTES	= 32;
	const BYTE DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F };
	const char* TEST_FILE = ".\\testData\\firma.bin";

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	assert( ERROR_LOAD_FILE != obj->loadFromBinaryFile(TEST_FILE) );

	BufferDinamico	buff;
	buff.setBuffer(DATOS, NUM_BYTES);

	assert( buff == (*obj) );
}


void FirmaDetachedTest::testSaveToBinaryFile()
{
	const int NUM_BYTES	= 32;
	const BYTE DATOS[NUM_BYTES] = {
		0x00,	0x01,	0x02,	0x03,	0x04,	0x05,	0x06,	0x07,	0x08,	0x09,
		0x0A,	0x0B,	0x0C,	0x0D,	0x0E,	0x0F,
		0x10,	0x11,	0x12,	0x13,	0x14,	0x15,	0x16,	0x17,	0x18,	0x19,
		0x1A,	0x1B,	0x1C,	0x1D,	0x1E,	0x1F };
	const char* TMP_FILE = ".\\testData\\firma.tmp";
	const char* TEST_FILE = ".\\testData\\firma.bin";

	obj->setBuffer(DATOS, NUM_BYTES);

	SetCurrentDirectory(ExtractFilePath(Application->ExeName).c_str());
	assert( ERROR_SAVE_FILE != obj->saveToBinaryFile(TMP_FILE) );
	try {
		assert( compararArchivos(TEST_FILE, TMP_FILE) );
	}
	__finally {
		DeleteFile(TMP_FILE);
	}
}


