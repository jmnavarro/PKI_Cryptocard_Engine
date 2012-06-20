#include "../h/ValidacionCertificadoCL.h"
#include "../h/CertCL.h"
#include "../h/AlmacenCertificadoCL.h"

#include "../../../Base/LibUtilidades/h/Cadena.h"
#include "../../../Base/LibUtilidades/h/Buffer.h"
#include "../../../Base/LibUtilidades/h/BufferMemoria.h"


ValidacionCertificadoCL::ValidacionCertificadoCL(AlmacenCertificadoCL *store)
	: ValidacionCertificadoLocal()
{
	almacen = store;
}

ValidacionCertificadoCL::~ValidacionCertificadoCL()
{
}


ValidacionCertificado::EstadoRevocacion ValidacionCertificadoCL::estaRevocado(const Certificado *cert)
{
	Cadena 				ruta;
	HANDLE				hFile;
	BufferMemoria		buffCRL;
	CL_CERTIFICATE		hCRL;
	const CertCL		*certCL = dynamic_cast<const CertCL*> (cert);
	EstadoRevocacion	ret;
	DWORD 	len;
	DWORD 	leido;
	int 	status;

	if ( almacen->isCerrado() )
		ret = EstadoRevError;
	else
		ret = EstadoRevValido;

	for (int i = 0; i < almacen->getNumeroCertificados() && ret == EstadoRevValido; i++)
	{
		ruta = almacen->getCRL(i).cadena();

		// leer todo el fichero en memoria
		hFile = CreateFile(ruta.GetBuffer(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			len = GetFileSize(hFile, NULL);

			if ( len > buffCRL.GetLongitud() )
				buffCRL.SetBuffer(len);

			leido = 0;
			ReadFile(hFile, buffCRL.GetBuffer(), len, &leido, NULL);
			CloseHandle(hFile);

			// importar la CRL
			status = CL->importCert(buffCRL.GetBuffer(), buffCRL.GetLongitud(), CRYPT_UNUSED, &hCRL);
			if ( cryptStatusOK(status) )
			{
				// comprobar el certificado del parámetro contra la CRL actual
				status = CL->checkCert(certCL->getClavePublica(), hCRL);
				CL->destroyCert(hCRL);
				switch (status)
				{
					case CRYPT_ERROR_INVALID:
					{
						ret = EstadoRevRevocado;
						break;
					}
					case CRYPT_OK:
					{
						ret = EstadoRevValido;
						break;
					}
					default:
					{
						ret = EstadoRevError;
						break;
					}
				}
			}
			else
				ret = EstadoRevError;
		}
		else
			ret = EstadoRevError;
	}

	return (ret);
}

