/**
 * Se define la clase ConversorDatos que implementa funciones de conversión.
 *
 * @author JM
 *
 * @date 17/11/2003 - JM  @li Versión inicial
 * @date 13/09/2004 - JM  @li Añadido el método ConvertirCadenaAUnicode
 * @date 29/09/2004 - JM  @li Añadido el método ConvertirUnicodeACadena
**/
#ifndef ConversorDatos_H
#define ConversorDatos_H

#include <windows.h>

#include "../../LibUtils/h/ArsCadena.h"


class ConversorDatos
{
public:
	static ArsCadena ByteToStr(const BYTE* bytes, DWORD size);
	static ArsCadena ByteToBase64(const BYTE* byte, DWORD size);

	static void StrToByte(const ArsCadena &cad, BYTE* bytes, DWORD size);
	static void StrToByte(const char* cad, BYTE* bytes, DWORD size);
	static void StrToByte(const wchar_t* cad, BYTE* bytes, DWORD size);

	static void Base64ToByte(const ArsCadena &cad, BYTE* byte, DWORD size);
	static void Base64ToByte(const char* cad, BYTE* byte, DWORD size);


	/** Convierte una cadena de caracteres de un byte, a caracteres Unicode.
	 *  Crea un nuevo buffer de tipo wchar_t con new[], por lo que debe ser liberado con delete []
	**/
	static wchar_t* ConvertirCadenaAUnicode(char *cadena);

	/** Convierte una cadena de caracteres unicode a caracteres de un byte.
	 *  Crea un nuevo buffer de tipo char con new[], por lo que debe ser liberado con delete []
	**/
	static char* ConvertirUnicodeACadena(wchar_t *cadena);
};


#endif

