/**
 * Se implementa "creadora" de objetos "AlmacenCertificado", siguiendo el patrón "Builder".
 * Con esta clase se consigue evitar el acoplamiento entre el código cliente y
 * las clases de implementación concreta (CAPI y cryptLib).
 *
 * @author JM
 *
 * @date 27/01/2005 - JM  @li Versión inicial
 * @date 24/03/2005 - JM  @li Se renombra de AlmacenBuilder a AlmacenCertificadoBuilder
 * @date 06/05/2005 - JM  @li Se permite también crear objetos Certificado a través de esta clase
**/
#if !defined(_ALMACEN_BUILDER_H)
#define _ALMACEN_BUILDER_H


// 0j0: aquí no se puede hacer el include de AlmacenCertificado.h porque entra en CAPI entra en conflicto con cryptLib.
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
	// constructor privado para no permitir la creación de instancias.
	AlmacenCertificadoBuilder();
};


#endif
