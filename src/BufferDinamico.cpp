//--------------------------------------------------------------------------------------

#include "../h/BufferDinamico.h"

//--------------------------------------------------------------------------------------
BufferDinamico::BufferDinamico()
{
	buffer = NULL;
	sizeBuffer = 0;
	sizeData = 0;
}

BufferDinamico::BufferDinamico(const BufferDinamico &copy)
{
	buffer = NULL;
	sizeBuffer = 0;
	sizeData = 0;

	(*this) = copy;
}

BufferDinamico::BufferDinamico(const void* buff, DWORD size)
{
	buffer = NULL;
	sizeBuffer = 0;
	sizeData = 0;

	setBuffer( (LPCBYTE) buff, size );
}

BufferDinamico::~BufferDinamico()
{
	freeBuffer();
}


LPCBYTE BufferDinamico::getBuffer() const
{
	return buffer;
}


void BufferDinamico::setBuffer(LPCBYTE newBuffer, DWORD newSize)
{
	if (newBuffer == NULL || newSize == 0)
		freeBuffer();
	else
	{
		bool cambiar = (sizeBuffer < newSize);
		DWORD nuevo;

		// se crea un nuevo buffer sólo si los nuevos datos no caben
		if (cambiar)
		{
			// la primera vez se utiliza la siguiente potencia de 2 del tamaño que
			// necesitamos, y las siguientes se duplica el tamaño actual para evitar la
			// fragmentación del heap
			if (sizeBuffer == 0)
				nuevo = 2;
			else
				nuevo = sizeBuffer;

			while (nuevo < newSize)
				nuevo *= 2;

			// una vez calculado el nuevo tamaño, debería ajustarse a siguiente potencia
			// de dos. Por ahora no lo hago.

			setSize(nuevo);
		}

		CopyMemory(buffer, newBuffer, newSize);

		sizeData = newSize;
	}
}


void BufferDinamico::setBuffer(const BufferDinamico &copy)
{
	if (this != &copy)
	{
		// se hace una copia exacta, tanto en tamaño del buffer como de los datos
		this->setSize(copy.sizeBuffer);
		this->setBuffer(copy.getBuffer(), copy.getSize());
	}
}


DWORD BufferDinamico::getSize() const
{
	// retornamos el tamaño de los datos, ya que siempre va a ser menor que el tamaño del buffer y es
	// el dato que interesa en realidad al usuario.
	// El tamaño del buffer debe ser un dato interno y transparente al exterior, ya que el programador
	// no tiene porqué saber que, en realidad, el buffer interno puede ser mayor que los datos contenidos.
	return sizeData;
}


void BufferDinamico::setSize(DWORD newSize)
{
	// Este método establece un nuevo tamaño para el buffer interno.
	// Además, se mantienen los datos que contenía el objeto, truncándolos si no cabían
	// Como los métodos "getSize" y "setSize" se refieren a atributos distintos, es
	// posible que se nos de esta situación:
	//
	// 		obj.setSize(10);
	//		obj.getSize() retorna distinto de 10.
	//
	// Para establecer el tamaño a "sizeData", se debe hacer a través de "setBuffer".
	//
	if (newSize != sizeBuffer)
	{
		LPBYTE	bak = NULL;
		DWORD	bakSizeData = 0;

		// si hay datos, se hace una copia temporal para mantenerlos
		if (newSize > 0 && sizeData > 0 && sizeBuffer > 0 && buffer != NULL)
		{
			bakSizeData = (sizeData < newSize) ? sizeData : newSize;
			bak = new BYTE[bakSizeData];
			memcpy(bak, buffer, bakSizeData);
		}

		freeBuffer();

		if (newSize > 0)
		{
			buffer = new BYTE[newSize];

			// se restaura o se rellena
			if (bak)
			{
				memcpy(buffer, bak, bakSizeData);
				sizeData = bakSizeData;
			}

			// rellenar lo que sobra (o todo)
			memset(buffer + bakSizeData, 0, newSize - bakSizeData);

			sizeBuffer = newSize;
		}

		if (bak)
			delete [] bak;
	}
}


void BufferDinamico::setSizeData(DWORD newSize)
{
	if (newSize > sizeBuffer)
		setSize(newSize);

	sizeData = newSize;
}


DWORD BufferDinamico::getSizeBuffer() const
{
	return sizeBuffer;
}


void BufferDinamico::freeBuffer()
{
	if (buffer && sizeBuffer > 0)
	{
		delete [] buffer;
		buffer = NULL;
		
		sizeBuffer = 0;
		sizeData = 0;
	}
}


bool BufferDinamico::equals(const BufferDinamico &dataComparacion) const
{
	bool ret;

	// si es el mismo objeto, son iguales
	ret = (&dataComparacion == this);
	if (!ret)
	{
		// si tienen la misma longitud, pueden ser iguales
		if (dataComparacion.sizeBuffer == this->sizeBuffer)
		{
			// si la comparación da 0, sin iguales
			ret = ( 0 == memcmp(dataComparacion.getBuffer(),
								this->buffer, this->sizeBuffer) );
		}
		else
			ret = false;
	}

	return (ret);
}


const BufferDinamico& BufferDinamico::operator = (const BufferDinamico &newData)
{
	setBuffer(newData);
	return (*this);
}


bool BufferDinamico::operator == (const BufferDinamico &dataComparacion) const
{
	return this->equals(dataComparacion);
}


bool BufferDinamico::operator != (const BufferDinamico &dataComparacion) const
{
	return ( !this->equals(dataComparacion) );
}

