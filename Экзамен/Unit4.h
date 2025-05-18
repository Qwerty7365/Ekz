//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
  __published: // IDE-managed Components
    TADOQuery* ADOQuery1;
    TDataSource* DataSource1;
    TEdit* Edit1;
    TEdit* Edit2;
    TEdit* Edit3;
    TEdit* Edit4;
    TEdit* Edit5;
    TEdit* Edit6;
    TEdit* Edit7;
    TEdit* Edit8;
    TLabel* Label1;
    TLabel* Label2;
    TLabel* Label3;
    TLabel* Label4;
    TLabel* Label5;
    TLabel* Label6;
    TLabel* Label7;
    TLabel* Label8;
    TLabel* Label9;
    TButton* Button1;
    TPanel* Panel1;
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  private: // User declarations
  public: // User declarations
    int SelectedId; // Публичная переменная
    __fastcall TForm4(TComponent* Owner);
    void LoadPartnerData(int partnerId);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4* Form4;
//---------------------------------------------------------------------------
#endif

