//--------------------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include "../h/Almacen.h"
#include "../h/Certificado.h"
#include "../h/Firma.h"

#include "../h/Tarjeta.h"
#include "../h/TarjetaCERES.h"

#include "../h/FirmaDetached.h"
#include "../h/Firmador.h"

//--------------------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

//--------------------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
	firma = NULL;
	almacen = NULL;
	certificado = NULL;
	tarjeta = NULL;
}

//--------------------------------------------------------------------------------------
void __fastcall TMainForm::ToolButton1Click(TObject *Sender)
{
	if ( od->Execute() )
	{
		Memo1->Lines->LoadFromFile(od->FileName);
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::ToolButton3Click(TObject *Sender)
{
	if ( od->Execute() )
	{
		// demostración de cómo usar la abstracción con interfaces
		if (firma == NULL)
			firma = new FirmaDetached();

		Persistente	*persistente = firma;

		persistente->loadFromFile(od->FileName.c_str());
		Memo2->Lines->LoadFromFile(od->FileName.c_str());
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::ToolButton2Click(TObject *Sender)
{
	if ( sd->Execute() )
	{
		// demostración de cómo usar la abstracción con interfaces
		Persistente	*persistente = firma;

		persistente->saveToFile(sd->FileName.c_str());
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::ToolButton4Click(TObject *Sender)
{
	if ( sd->Execute() )
	{
		Memo2->Lines->SaveToFile(sd->FileName);
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::Memo1Change(TObject *Sender)
{
	Button1->Enabled = (certificado && Memo1->Lines->Count > 0);
	ToolButton4->Enabled = Button1->Enabled;
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::Memo2Change(TObject *Sender)
{
	Button2->Enabled = (Memo2->Lines->Count > 0 && certificado);
	ToolButton4->Enabled = Button2->Enabled;
}
//--------------------------------------------------------------------------------------


void __fastcall TMainForm::Button3Click(TObject *Sender)
{
	if ( AbrirAlmacen() )
	{
		if ( AbrirCertificado() )
			ShowMessage("Certificado cargado correctamente");
		else
			ShowMessage("No se ha encontrado ningún certificado que tenga ese texto en su campo \"Asunto\"");
	}
	else
		ShowMessage("No se ha podido abrir el almacén.");

	Button1->Enabled = (certificado != NULL && Memo1->Lines->Count > 0);
	Button2->Enabled = (certificado != NULL && Memo2->Lines->Count > 0);
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	Screen->Cursor = crHourGlass;
	try {
		if ( AbrirTarjeta() )
		{
			Firmador  *firmador = tarjeta->createFirmador();

			firma = firmador->firmar(Firmador::TipoFirmaDetached,
									 Memo1->Lines->Text.c_str(), *certificado);

			delete firmador;

			if (!firma)
				ShowMessage("Se ha producido un error firmando.");
			else
				Memo2->Lines->Text = firma->serializar().cadena();
		}
		else
			ShowMessage("No se ha podido abrir la tarjeta.");
	}
	__finally {
		Screen->Cursor = crDefault;
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::Button2Click(TObject *Sender)
{
	if (!firma)
		return;

	firma->freeBuffer();

	if (!firma->deserializar(Memo2->Lines->Text.c_str()))
		ShowMessage("Se ha producido un error recuperando la firma");
	else
	{
		Screen->Cursor = crHourGlass;

		if ( AbrirTarjeta() )
		{
			Firmador  *firmador = tarjeta->createFirmador();

			if ( firmador->validar(Memo1->Lines->Text.c_str(), *firma, *certificado) )
				MessageBox(Handle, "Firma y documento CORRECTOS", "Validar", MB_ICONINFORMATION);
			else
				MessageBox(Handle, "La firma NO coindice con el documento.", "Validar", MB_ICONERROR);

			delete firmador;
		}
		else
			ShowMessage("No se ha podido abrir la tarjeta");

		Screen->Cursor = crDefault;
	}
}
//--------------------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
	if (firma != NULL)
		delete firma;

	if (almacen != NULL)
	{
		if (tarjeta != NULL)
			tarjeta->cerrarAlmacen(almacen);
		else
		{
			almacen->close();
			delete almacen;
		}
	}

	if (tarjeta != NULL)
	{
		tarjeta->desconectar();
		delete tarjeta;
	}

	if (certificado != NULL)
		delete certificado;
}
//--------------------------------------------------------------------------------------


bool TMainForm::AbrirTarjeta()
{
	bool conectado;

	if (certificado == NULL)
		return false;

	if (tarjeta != NULL)
	{
		tarjeta->desconectar();
		delete tarjeta;
		tarjeta = NULL;
	}

	tarjeta = new TarjetaCERES();

	do
	{
		Screen->Cursor = crHourGlass;
		conectado = tarjeta->conectar(certificado);
		Screen->Cursor = crDefault;

		if ( !conectado )
		{
			if ( IDCANCEL == MessageBox(Handle, "No se ha podido conectar con la tarjeta criptográfica CERES", "Conectar", MB_RETRYCANCEL | MB_ICONQUESTION) )
			{
				delete tarjeta;
				tarjeta = NULL;
				return false;
			}
		}
	} while (!conectado);

	return true;
}


bool TMainForm::AbrirAlmacen()
{
	bool conectado;

	if (almacen != NULL)
	{
		delete almacen;
		almacen = NULL;
	}

	do
	{
		Tarjeta *factory = new TarjetaCERES();

		Screen->Cursor = crHourGlass;

		almacen = factory->abrirAlmacen("");
		conectado = (almacen != NULL);

		Screen->Cursor = crDefault;

		delete factory;

		if ( !conectado )
		{
			if (IDCANCEL == MessageBox(Handle, "No se ha podido conectar con la tarjeta criptográfica CERES", "Conectar", MB_RETRYCANCEL | MB_ICONQUESTION) )
				return false;
		}
	} while (!conectado);

	return true;
}


bool TMainForm::AbrirCertificado()
{
	if (almacen == NULL)
		return false;

	if (certificado != NULL)
	{
		delete certificado;
		certificado = NULL;
	}

	ArsCadena str = InputBox("Buscar el certificado",
							  "Introduce el valor para buscar el certificado:",
							  "123456789-Z"/* DEMO DE CAMERFIRMA */);

	certificado = almacen->loadCertificado(Almacen::ClaveBusquedaSubStrAsunto, &str);

	return (certificado != NULL);
}

