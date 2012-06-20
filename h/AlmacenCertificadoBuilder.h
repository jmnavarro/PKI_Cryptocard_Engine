/**
 * Se implementa "creadora" de objetos "AlmacenCertificado", siguiendo el patr�n "Builder".
 * Con esta clase se consigue evitar el acoplamiento entre el c�digo cliente y
 * las clases de implementaci�n concreta (CAPI y cryptLib).
 *
 * @author JM
 *
 * @date 27/01/2005 - JM  @li Versi�n inicial
 * @date 24/03/2005 - JM  @li Se renombra de AlmacenBuilder a AlmacenCertificadoBuilder
 * @date 06/05/2005 - JM  @li Se permite tambi�n crear objetos Certificado a trav�s de esta clase
**/
#if !defined(_ALMACEN_BUILDER_H)
#define _ALMACEN_BUILDER_H


// 0j0: aqu� no se puede hacer el include de AlmacenCertificado.h porque entra en CAPI entra en conflicto con cryptLib.
class AlmacenCertificado;
class Certificado;


#define NOMBRE_TOOLKIT_CRYPTLIB	"cryptLib"
#define NOMBRE_TOOLKIT_CAPI		"CryptoAPI"


class AlmacenCertificadoBuilder
{
public:
	typedef enum
	{
		ToolkitCryptLib,
		ToolkitCAPI
	} TipoToolkit;

public:
	static AlmacenCertificado* Crear(const char *_nombreToolkit, const char *_nombreAlmacen);
	static AlmacenCertificado* Crear(AlmacenCertificadoBuilder::TipoToolkit tipoToolkit, const char *_nombreAlmacen);

	static Certificado* CrearCertificado(const char *_nombreToolkit);
	static Certificado* CrearCertificado(AlmacenCertificadoBuilder::TipoToolkit tipoToolkit);

	static void Liberar(AlmacenCertificado* &obj);
	static void Liberar(Certificado* &obj);

private:
	// constructor privado para no permitir la creaci�n de instancias.
	AlmacenCertificadoBuilder();
};


#endif
