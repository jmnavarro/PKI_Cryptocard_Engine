//--------------------------------------------------------------------------------------
#include "../h/Firma.h"
//--------------------------------------------------------------------------------------

Firma::Firma()
	: BufferDinamico()
{
}

Firma::Firma(const Firma &copy)
	: BufferDinamico(copy)
{
}

Firma::Firma(const void* buff, DWORD size)
	: BufferDinamico(buff, size)
{
}

Firma::~Firma()
{
}



DWORD Firma::saveToFile(const char* filename)
{
	ArsCadena serial = serializar();

	return saveData(filename, serial.cadena(), serial.longitud());
}


DWORD Firma::loadFromFile(const char* filename)
{
	BufferDinamico  buff;
	DWORD size = loadData(filename, buff);

	if (size > 0 && size != ERROR_LOAD_FILE)
		deserializar( (char*)buff.getBuffer(), buff.getSize() );

	return (size);
}


DWORD Firma::saveToBinaryFile(const char* filename)
{
	return saveData(filename, getBuffer(), getSize());
}


DWORD Firma::loadFromBinaryFile(const char* filename)
{
	BufferDinamico  buff;
	DWORD size = loadData(filename, buff);

	if (size > 0 && size != ERROR_LOAD_FILE)
		this->setBuffer(buff);

	return (size);
}


DWORD Firma::saveData(const char* filename, LPCBYTE data, DWORD len)
{
	HANDLE	fich = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == fich)
		return ERROR_SAVE_FILE;

	DWORD  escrito = 0;

	if ( !WriteFile(fich, data, len, &escrito, NULL) || escrito != len )
		escrito = ERROR_SAVE_FILE;

	CloseHandle(fich);

	return escrito;
}


DWORD Firma::loadData(const char* filename, BufferDinamico &datos)
{
	HANDLE	fich = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == fich)
		return ERROR_LOAD_FILE;

	// La firma nunca va a tener más de 2GB (supongo yo :)
	DWORD sourceLen = GetFileSize(fich, NULL);
	DWORD leido;

	if (sourceLen != 0xFFFFFFFF) {
		// se lee todo de una tacada, aunque sería más eficiente leerlo en bloques
		LPBYTE sourceData = new BYTE[sourceLen];

		if ( ReadFile(fich, sourceData, sourceLen, &leido, NULL) && sourceLen == leido )
			datos.setBuffer(sourceData, leido);
		else
			leido = ERROR_LOAD_FILE;

		delete [] sourceData;
	}
	else
		leido = ERROR_LOAD_FILE;

	CloseHandle(fich);

	return leido;
}

