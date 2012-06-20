//---------------------------------------------------------------------------
#ifndef mainH
#define mainH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <ToolWin.hpp>
#include <ImgList.hpp>
#include <Dialogs.hpp>

//---------------------------------------------------------------------------

class Almacen;
class Certificado;
class Firma;
class Tarjeta;

//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TMemo *Memo1;
	TLabel *Label1;
	TLabel *Label2;
	TMemo *Memo2;
	TButton *Button1;
	TButton *Button2;
	TBevel *Bevel1;
	TCoolBar *CoolBar1;
	TToolBar *ToolBar1;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolBar *ToolBar2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TImageList *ImageList1;
	TOpenDialog *od;
	TSaveDialog *sd;
	TButton *Button3;
	void __fastcall ToolButton1Click(TObject *Sender);
	void __fastcall ToolButton3Click(TObject *Sender);
	void __fastcall ToolButton2Click(TObject *Sender);
	void __fastcall ToolButton4Click(TObject *Sender);
	void __fastcall Memo1Change(TObject *Sender);
	void __fastcall Memo2Change(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:
	Tarjeta		*tarjeta;
	Almacen		*almacen;
	Certificado	*certificado;
	Firma		*firma;

	bool AbrirTarjeta();
	bool AbrirAlmacen();
	bool AbrirCertificado();

public:
	__fastcall TMainForm(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------

#endif
