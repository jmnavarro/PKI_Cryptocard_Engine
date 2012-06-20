#include "../h/ValidacionCertificadoCAPI.h"
#include "../h/CertCAPI.h"
#include "../h/AlmacenCertificadoCAPI.h"


ValidacionCertificadoCAPI::ValidacionCertificadoCAPI(AlmacenCertificadoCAPI *store)
	: ValidacionCertificadoLocal()
{
	almacen = store;
}

ValidacionCertificadoCAPI::~ValidacionCertificadoCAPI()
{
}


ValidacionCertificado::EstadoRevocacion ValidacionCertificadoCAPI::estaRevocado(const Certificado *cert)
{
	const CertCAPI	*certCAPI = dynamic_cast<const CertCAPI*> (cert);

	PCCRL_CONTEXT	crl;
	PCRL_ENTRY		entry;
	EstadoRevocacion ret;
	bool			encontrado;

	ret = EstadoRevError;

	if (cert != NULL && almacen != NULL && almacen->getHandle() != 0)
	{
		// cargar y recorrer las CRLs disponibles dentro del almacén
		crl = CertEnumCRLsInStore(almacen->getHandle(), NULL);

		encontrado = false;
		while(crl != NULL && !encontrado)
		{
			// también se puede usar esta: CertVerifyCRLRevocation
			entry = NULL;
			if ( CertFindCertificateInCRL(certCAPI->getHandle(), crl, 0, 0, &entry) && entry != NULL)
				encontrado = true;

			crl = CertEnumCRLsInStore(almacen->getHandle(), crl);
		}

		if (encontrado)
			ret = EstadoRevRevocado;
		else
			ret = EstadoRevValido;

		if (crl != NULL)
			::CertFreeCRLContext(crl);
	}

	return (ret);
}


ValidacionCertificado::EstadoCaducidad ValidacionCertificadoCAPI::estaCaducado(const Certificado *cert)
{
	const CertCAPI  *certCAPI = dynamic_cast<const CertCAPI*>(cert);

	EstadoCaducidad  ret;

	ret = EstadoCadError;

	if (certCAPI != NULL)
	{
		if (CertVerifyTimeValidity(NULL, certCAPI->getInfo()) != 0)
		{
			ret = EstadoCadCaducado;
		}
		else
		{
			ret = EstadoCadValido;
		}
	}

	return (ret);
}

