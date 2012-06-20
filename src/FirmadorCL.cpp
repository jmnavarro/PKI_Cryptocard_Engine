#include "../h/FirmadorCL.h"
#include "../h/CertCL.h"


FirmadorCL::FirmadorCL(CL_DEVICE _dev)
	: Firmador()
{
	dev = _dev;
}

FirmadorCL::~FirmadorCL()
{
}


bool FirmadorCL::calcularFirma(const BufferDinamico &doc, const Certificado &cert, BufferDinamico &firma)
{
	const CertCL	*certCL;
	CL_CONTEXT	hashCtx;
	int		status;
	bool	ok;
	LPBYTE	blobFirma;
	int		lenFirma;


	certCL = dynamic_cast<const CertCL*> (&cert);
	if ( certCL == 0 || !certCL->tieneClavePrivada() )
		return (false);


	status = CL->createContext(&hashCtx, CRYPT_UNUSED, CL_ALGO_SHA);
	ok = cryptStatusOK(status);
	if (ok)
	{
		status = CL->encrypt(hashCtx, (void*)doc.getBuffer(), doc.getSize());
		ok = cryptStatusOK(status);
		if (ok)
		{
			status = CL->encrypt(hashCtx, (void*)doc.getBuffer(), 0);
			ok = cryptStatusOK(status);
			if (ok)
			{
				status = CL->createSignatureEx(NULL, &lenFirma, CL_FORMAT_PKCS7, certCL->getClavePrivada(), hashCtx, CRYPT_USE_DEFAULT);
				ok = cryptStatusOK(status);
				if (ok)
				{
					blobFirma = new BYTE[lenFirma];

					status = CL->createSignatureEx(blobFirma, &lenFirma, CL_FORMAT_PKCS7, certCL->getClavePrivada(), hashCtx, CRYPT_USE_DEFAULT);
					ok = cryptStatusOK(status);
					if (ok)
					{
						firma.setBuffer(blobFirma, lenFirma);
					}

					delete [] blobFirma;
				}
			}
		}
		CL->destroyContext(hashCtx);
	}

	return (ok);
}


bool FirmadorCL::calcularResumen(const BufferDinamico &doc, BufferDinamico &hash)
{
	CL_CONTEXT	hashCtx;
	int		status;
	bool	ok;
	BYTE	blobHash[CRYPT_MAX_HASHSIZE];
	int		lenHash = CRYPT_MAX_HASHSIZE;


	if (doc.getSize() == 0)
		return (false);


	status = CL->createContext(&hashCtx, CRYPT_UNUSED, CL_ALGO_SHA);
	ok = cryptStatusOK(status);
	if (ok)
	{
		status = CL->encrypt(hashCtx, (void*)doc.getBuffer(), doc.getSize());
		ok = cryptStatusOK(status);
		if (ok)
		{
			status = CL->encrypt(hashCtx, (void*)doc.getBuffer(), 0);
			ok = cryptStatusOK(status);
			if (ok)
			{
				status = CL->getAttributeString(hashCtx, CL_CTXINFO_HASHVALUE, blobHash, &lenHash);
				ok = cryptStatusOK(status);
				if (ok)
				{
					hash.setBuffer(blobHash, lenHash);
				}
			}
		}
		CL->destroyContext(hashCtx);
	}

	return (ok);
}


bool FirmadorCL::validarFirma(const BufferDinamico &data, const Firma &firma, const Certificado *cert /*= NULL*/)
{
	const CertCL	*certCL;
	CL_CONTEXT	hashCtx;
	int		status;
	bool	ok;


	// Por ahora no sé cómo incrustar el certificado del firmante dentro del PKCS#7
	// y luego utilizarlo para verificar la firma.
	// Por eso si pasan un certificado nulo (indicando que se valide con el incrustado),
	// retorno error porque no puedo verificar la firma.
	if (cert == NULL)
		return (false);

	certCL = dynamic_cast<const CertCL*> (cert);
	if (certCL == 0)
		return (false);

	if (data.getSize() == 0 || firma.getSize() == 0)
		return (false);


	status = CL->createContext(&hashCtx, CRYPT_UNUSED, CL_ALGO_SHA);
	ok = cryptStatusOK(status);
	if (ok)
	{
		// Hash the data
		status = CL->encrypt(hashCtx, (void*)data.getBuffer(), data.getSize());
		ok = cryptStatusOK(status);
		if (ok)
		{
			status = CL->encrypt(hashCtx, (void*)data.getBuffer(), 0);
			ok = cryptStatusOK(status);
			if (ok)
			{
				// Check the signature using the signature blob
				status = CL->checkSignatureEx((void*)firma.getBuffer(), certCL->getClavePublica(), hashCtx);
				ok = cryptStatusOK(status);
			}
		}
		CL->destroyContext(hashCtx);
	}

	return (ok);
}


