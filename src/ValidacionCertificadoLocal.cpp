#include "../h/ValidacionCertificadoLocal.h"


ValidacionCertificadoLocal::ValidacionCertificadoLocal()
	: ValidacionCertificado()
{
}

ValidacionCertificadoLocal::~ValidacionCertificadoLocal()
{
}


ValidacionCertificado::EstadoCaducidad ValidacionCertificadoLocal::estaCaducado(const Certificado *cert)
{
	SYSTEMTIME		tmp;
	FILETIME		ftAhora, ftDesde, ftHasta;
	EstadoCaducidad	ret;

	ret = EstadoCadError;

	GetSystemTime(&tmp);
	if (cert != NULL && SystemTimeToFileTime(&tmp, &ftAhora))
	{
		tmp = cert->getFechaDesde();
		if (SystemTimeToFileTime(&tmp, &ftDesde))
		{
			tmp = cert->getFechaHasta();
			if (SystemTimeToFileTime(&tmp, &ftHasta))
			{
				if (CompareFileTime(&ftAhora, &ftDesde) == -1 ||
					CompareFileTime(&ftAhora, &ftHasta) == +1)
				{
					ret = EstadoCadCaducado;
				}
				else
				{
					ret = EstadoCadValido;
				}
			}
		}
	}

	return (ret);
}
