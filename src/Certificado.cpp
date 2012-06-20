//--------------------------------------------------------------------------------------
#include "../h/Certificado.h"
//--------------------------------------------------------------------------------------

Certificado::Certificado()
{
}


Certificado::Certificado(const ArsCadena &encoded)
{
	deserializar(encoded);
}


Certificado::Certificado(Certificado *copy)
{
	setFechaDesde(&copy->fechaDesde);
	setFechaHasta(&copy->fechaHasta);
	setNombre(copy->nombre);
	setEmisor(copy->emisor);
	setNombreEmisor(copy->nombreEmisor);
	setNumeroSerie(copy->numeroSerie);
	setKeyContainer(copy->keyContainer);
}


Certificado::~Certificado()
{
}


bool Certificado::equals(Certificado* comparar)
{
	return (0 == memcmp(&this->fechaDesde, &comparar->fechaDesde, sizeof(SYSTEMTIME)) &&
			0 == memcmp(&this->fechaHasta, &comparar->fechaHasta, sizeof(SYSTEMTIME)) &&
			this->nombre	   == comparar->nombre &&
			this->emisor	   == comparar->emisor &&
			this->numeroSerie  == comparar->numeroSerie &&
			this->keyContainer == comparar->keyContainer);
}


SYSTEMTIME Certificado::getFechaDesde() const
{
	SYSTEMTIME	ret = fechaDesde;
	return (ret);
}


SYSTEMTIME Certificado::getFechaHasta() const
{
	SYSTEMTIME	ret = fechaHasta;
	return (ret);
}


void Certificado::setFechaDesde(const LPSYSTEMTIME value)
{
	if (value)
		fechaDesde = *value;
	else
		ZeroMemory(&fechaDesde, sizeof(SYSTEMTIME));
}


void Certificado::setFechaHasta(const LPSYSTEMTIME value)
{
	if (value)
		fechaHasta = *value;
	else
		ZeroMemory(&fechaHasta, sizeof(SYSTEMTIME));
}


ArsCadena Certificado::getNombre() const
{
	return nombre;
}


ArsCadena Certificado::getEmisor() const
{
	return emisor;
}


ArsCadena Certificado::getNombreEmisor() const
{
	return nombreEmisor;
}


ArsCadena Certificado::getNumeroSerie() const
{
	return numeroSerie;
}


ArsCadena Certificado::getKeyContainer() const
{
	return keyContainer;
}


ArsCadena Certificado::getIdentificador() const
{
	return getNumeroSerie();
}


void Certificado::setNombre(const ArsCadena &value)
{
	nombre = value;
}


void Certificado::setEmisor(const ArsCadena &value)
{
	emisor = value;
}


void Certificado::setNombreEmisor(const ArsCadena &value)
{
	nombreEmisor = value;
}


void Certificado::setNumeroSerie(const ArsCadena &value)
{
	numeroSerie = value;
}


void Certificado::setKeyContainer(const ArsCadena &value)
{
	keyContainer = value;
}


void Certificado::resetData()
{
	setFechaDesde(NULL);
	setFechaHasta(NULL);
	setNombre("");
	setEmisor("");
	setNumeroSerie("");
	setKeyContainer("");
}

