#include "../h/ConversorDatos.h"

#include "../h/Base64.h"


ArsCadena ConversorDatos::ByteToStr(const BYTE* bytes, DWORD numBytes)
{
	ArsCadena  ret;

	if ( bytes && numBytes > 0)
	{
		// el buffer de salida es el doble que los datos porque son necesarios dos
		// caracteres para representar un único byte.
		char*	buff = new char[numBytes*2 + 1];
		char*	ptrBuff = buff;
		BYTE*	ptr = (BYTE*) bytes;
		BYTE	b;

		for (DWORD i = 0; i < numBytes; i++)
		{
		   b = (*ptr & 0xF0) >> 4;
		   *ptrBuff++ = (b <= 9) ? b + '0' : (b - 10) + 'A';
		   b = *ptr & 0x0F;
		   *ptrBuff++ = (b <= 9) ? b + '0' : (b - 10) + 'A';

		   ptr++;
		}
		*ptrBuff = '\0';

		ret.asignar(buff);

		delete [] buff;
	}

	return (ret);
}


ArsCadena ConversorDatos::ByteToBase64(const BYTE* byte, DWORD size)
{
	CBase64			conversor;
	BufferSalida	buffer;
	ArsCadena		ret;

	if (byte == NULL || size == 0 || byte[0] == '\0')
		ret = "";
	else
	{
		buffer = conversor.Encode(byte, size);
		ret = buffer.c_str();
	}

	return (ret);
}


void ConversorDatos::StrToByte(const ArsCadena &cad, BYTE* bytes, DWORD numBytes)
{
	DWORD i, copiado, len = cad.longitud();
	BYTE* ptr = bytes;
	unsigned char b1, b2;

	i = 0;
	copiado = 0;
	while (i < len && copiado < numBytes)
	{
		if (cad[i] != ' ')
		{
			b1 = cad[i++];
			b1 = (b1>='0' && b1<='9') ? (b1 - '0') : ((b1 + 10) - 'A');

			b2 = cad[i++];
			b2 = (b2>='0' && b2<='9') ? (b2 - '0') : ((b2 + 10) - 'A');

			*ptr++ = (b1 << 4) | b2;
			copiado++;
		}
		else
			i++;
	}
}

void ConversorDatos::StrToByte(const char* cad, BYTE* bytes, DWORD numBytes)
{
	char *ptr = (char*) cad;
	unsigned char b1, b2;

	for (DWORD i=0; i < numBytes && *ptr; i++)
	{
		if (' ' != (b1 = *ptr++))
		{
			b1 = (b1>='0' && b1<='9') ? (b1 - '0') : ((b1 + 10) - 'A');

			b2 = *ptr++;
			b2 = (b2>='0' && b2<='9') ? (b2 - '0') : ((b2 + 10) - 'A');

			*bytes++ = (b1 << 4) | b2;
		}
		else
			i--;
	}
}

void ConversorDatos::StrToByte(const wchar_t* cad, BYTE* bytes, DWORD numBytes)
{
	BYTE *ptr = (BYTE*) cad;
	BYTE b1, b2;

	// los bytes pares son nulos para caracteres ANSI.
	for (DWORD i=0; i < numBytes && *ptr; i++)
	{
		if (' ' != (b1 = *ptr++))
		{
			b1 = (b1>='0' && b1<='9') ? (b1 - '0') : ((b1 + 10) - 'A');
			ptr++;	// nulo

			b2 = *ptr++;
			b2 = (b2>='0' && b2<='9') ? (b2 - '0') : ((b2 + 10) - 'A');
			ptr++;	// nulo

			*bytes++ = (b1 << 4) | b2;
		}
		else
		{
			i--;
			ptr++;
		}
	}
}


void ConversorDatos::Base64ToByte(const ArsCadena &cad, BYTE* byte, DWORD size)
{
	CBase64		conversor;

	if (byte != NULL && size > 0 && cad != "")
		conversor.DecodeBuffer(cad.cadena(), cad.longitud(), byte, &(unsigned int)size);
	else if (byte != NULL && size > 0)
		byte[0] = '\0';
}


void ConversorDatos::Base64ToByte(const char* cad, BYTE* byte, DWORD size)
{
	CBase64		conversor;

	if (byte != NULL && size > 0 && cad != NULL && cad[0] != '\0')
		conversor.DecodeBuffer(cad, strlen(cad), byte, &(unsigned int)size);
	else if (byte != NULL && size > 0)
		byte[0] = '\0';
}


wchar_t* ConversorDatos::ConvertirCadenaAUnicode (char *cadena)
{
	wchar_t *retorno;
	int i, lenCadena;

	if (cadena != NULL)
	{
		lenCadena = strlen (cadena);
		retorno = new wchar_t[lenCadena + 1];
		for (i = 0; i <= lenCadena; i++)
		{
			retorno[i] = (unsigned char) cadena[i];
		}
	}
	else
		retorno = NULL;

	return (retorno);
}



char* ConversorDatos::ConvertirUnicodeACadena(wchar_t *cadena)
{
	char	*retorno;
	int		i, lenCadena;

	if (cadena != NULL)
	{
		lenCadena = wcslen(cadena);
		retorno = new char[lenCadena + 1];
		for (i = 0; i <= lenCadena; i++)
		{
			retorno[i] = (unsigned char) cadena[i];
		}
	}
	else
		retorno = NULL;

	return (retorno);
}

