#if !defined(_CERT_CAPI_TEST_H_)
#define _CERT_CAPI_TEST_H_

#include "TestCase.h"
#include "TestCaller.h"


class Certificado;


class CertCAPITest : public TestCase
{
public:
	CertCAPITest(std::string name);
	virtual ~CertCAPITest();

	void setUp();
	void tearDown();

	static Test *suite();

protected:
	void testSerializar();
	void testDeserializar();
	void testCrearDesdeBuffer();

private:
	Certificado	*obj;
};


typedef TestCaller<CertCAPITest> CertCAPITestCaller;


#endif
