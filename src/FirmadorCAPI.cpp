#include "../h/FirmadorCAPI.h"
#include "../h/CertCAPI.h"


FirmadorCAPI::FirmadorCAPI(HCRYPTPROV  _csp)
	: Firmador()
{
	csp = _csp;
}

FirmadorCAPI::~FirmadorCAPI()
{
}


bool FirmadorCAPI::calcularFirma(const BufferDinamico &doc, const Certificado &cert, BufferDinamico &firma)
{
	const CertCAPI	&certCAPI = dynamic_cast<const CertCAPI&> (cert);

	if (certCAPI.getHandle() == NULL)
		return false;


	// Se configura una estructura que contiene los datos del firmante
	//
	CMSG_SIGNER_ENCODE_INFO firmante;

	memset(&firmante, 0, sizeof(firmante));
	firmante.cbSize = sizeof(firmante);
	firmante.pCertInfo = certCAPI.getHandle()->pCertInfo;
	firmante.hCryptProv = csp;
	firmante.dwKeySpec = AT_SIGNATURE;
	firmante.HashAlgorithm.pszObjId = szOID_OIWSEC_sha1;

	// Se configura una estructura que contiene los datos del certificado
	//
	CERT_BLOB   blobCertificado;

	blobCertificado.cbData = certCAPI.getHandle()->cbCertEncoded;
	blobCertificado.pbData = certCAPI.getHandle()->pbCertEncoded;

	// Se configura una estructura que contiene los datos de lo que queremos firmar
	//
	CMSG_SIGNED_ENCODE_INFO firmado;

	memset(&firmado, 0, sizeof(firmado));
	firmado.cbSize = sizeof(firmado);
	firmado.cSigners = 1;		// un firmante
	firmado.rgSigners = &firmante;
	firmado.cCertEncoded = 1;	// un certificado
	firmado.rgCertEncoded = &blobCertificado;
	firmado.cCrlEncoded = 0;	// sin CRL
	firmado.rgCrlEncoded = NULL;

	// Se abre el mensaje PKCS#7 para codificarlo
	//
	HCRYPTMSG msg;
	LPBYTE buffFirma;
	DWORD lenFirma;
	bool ret;

	ret = false;

	msg = CryptMsgOpenToEncode(TIPO_CODIFICACION,
						CMSG_DETACHED_FLAG,
						CMSG_SIGNED,
						&firmado,
						NULL, NULL);

	if (msg != 0)
	{
		// Se meten los datos en el mensaje
		//
		if ( CryptMsgUpdate(msg, doc.getBuffer(), doc.getSize(), TRUE) )
		{
			// Se obtiene el tamaño de la firma
			if ( CryptMsgGetParam(msg,
								CMSG_CONTENT_PARAM, 0,
								NULL, &lenFirma) )
			{
				// Se reserva un buffer para almacenar la firma y se calcula esta.
				//
				buffFirma = new BYTE[lenFirma];

				if ( CryptMsgGetParam(msg,
									CMSG_CONTENT_PARAM, 0,
									buffFirma, &lenFirma) )
				{
					firma.setBuffer(buffFirma, lenFirma);
					ret = true;
				}

				delete [] buffFirma;
			}
		}

	}

	return (ret);
}


bool FirmadorCAPI::calcularResumen(const BufferDinamico &doc, BufferDinamico &hash)
{
	bool ret = false;

	// Si no hay nada que resumir...
	//
	if ( doc.getSize() == 0 )
		return (ret);

	// Se crea un objeto hash
	//
	HCRYPTHASH	hHash = 0;

	if( ::CryptCreateHash(csp, CALG_MD5, 0, 0, &hHash) )
	{
		// Meter datos en el hash
		//
		if( ::CryptHashData(hHash, doc.getBuffer(), doc.getSize(), 0) )
		{
			// Leer el tamaño del hash
			//
			DWORD sizeHash;
			DWORD dummy = sizeof(DWORD);

			if( ::CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&sizeHash, &dummy, 0) )
			{
				// Obtener hash
				//
				hash.setSizeData(sizeHash); // reservar dentro de BufferDinamico
				LPBYTE	ptr = (LPBYTE) hash.getBuffer();

				ret = ::CryptGetHashParam(hHash, HP_HASHVAL, ptr, &sizeHash, 0);
			}
		}
	}

	if (hHash != 0)
		::CryptDestroyHash(hHash);

	return (ret);
}


bool FirmadorCAPI::validarFirma(const BufferDinamico &data, const Firma &firma, const Certificado *cert /*= NULL*/)
{
	const CertCAPI	*firmante;
	HCRYPTMSG	msg;
	bool		ret;
	bool		liberarCert;

	if (data.getSize() == 0 || firma.getSize() == 0)
		return (false);

	ret = false;

	msg = CryptMsgOpenToDecode(TIPO_CODIFICACION,
								CMSG_DETACHED_FLAG,
								0,			// CMSG_SIGNED
								NULL, NULL, // certificado->pCertInfo
								NULL);
	if (msg != 0)
	{
		// meter los datos de la firma
		if( CryptMsgUpdate(msg, firma.getBuffer(), firma.getSize(), TRUE) )
		{
			// meter los datos del mensaje original
			if ( CryptMsgUpdate(msg, data.getBuffer(), data.getSize(), TRUE) )
			{
				liberarCert = (cert == NULL);

				if (cert == NULL)
				{
					// si no han pasado certificado, se comprueba contra el que tenga incrustado el PKCS#7
					cert = getCertificadoDeMensajeFirmado(msg);
				}

				if (cert != NULL)
				{
					firmante = dynamic_cast<const CertCAPI*> (cert);

					// verificar firma
					ret = CryptMsgControl(msg, 0,
										CMSG_CTRL_VERIFY_SIGNATURE,
										firmante->getInfo());

					if (liberarCert)
						delete firmante;
				}
			}
		}

		CryptMsgClose(msg);
	}

	return (ret);
}


Certificado* FirmadorCAPI::getCertificadoDeMensajeFirmado(HCRYPTMSG	msg)
{
	PCERT_INFO		signerInfo;
	DWORD			lenSignerInfo;
	HCERTSTORE		memStore;
	PCCERT_CONTEXT	signerContext;

	signerContext = NULL;

	if ( CryptMsgGetParam(msg,
					CMSG_SIGNER_CERT_INFO_PARAM,
					0,
					NULL, &lenSignerInfo) )
	{
		signerInfo = (PCERT_INFO) malloc(lenSignerInfo);
		if (signerInfo != NULL)
		{
			memset(signerInfo, 0, lenSignerInfo);

			if ( CryptMsgGetParam(msg,
							CMSG_SIGNER_CERT_INFO_PARAM,
							0,
							signerInfo, &lenSignerInfo) )
			{
				memStore = CertOpenStore(
								CERT_STORE_PROV_MSG,
								TIPO_CODIFICACION,
								NULL,		// csp
								0,			// Flags
								msg);

				if ( memStore != NULL )
				{

					signerContext = CertGetSubjectCertificateFromStore(
														memStore,
														TIPO_CODIFICACION,
														signerInfo);
					CertCloseStore(memStore, 0);
				}
			}
			free(signerInfo);
		}
	}

	return (signerContext == NULL) ?
				NULL
			:
				new CertCAPI(signerContext);
}

