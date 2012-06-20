/* Generated by Together */

#ifndef FIRMADORCAPI_H
#define FIRMADORCAPI_H
#include "ConversorDatos.h"
#include "Certificado.h"
#include "Firmador.h"
class AlmacenCAPI;
class Almacen;
class BufferDinamico;
class Firma;

/**
 * @stereotype toolkit 
 */
class FirmadorCAPI : public Firmador {


public:

public:
protected:    

    virtual bool calcularFirma(const BufferDinamico&, BufferDinamico&);

    virtual bool calcularOriginal(BufferDinamico& doc, Firma&);

    virtual bool calcularResumen(const BufferDinamico&, BufferDinamico&);

protected:
    
private:    

    /** @link dependency 
     * @supplierRole Product
     * @clientRole Creator*/
    /*# Certificado lnkCertificado; */
};
#endif //FIRMADORCAPI_H