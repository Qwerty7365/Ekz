//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
  __published: // IDE-managed Components
    TScrollBox* ScrollBox1;
    TButton* Button1;
    TButton* Button2;
    TADOConnection* ADOConnection1;
    TADOQuery* ADOQuery1;
    TDataSource* DataSource1;
    void __fastcall FormCreate(TObject* Sender);
    void __fastcall Button1Click(TObject* Sender);
    void __fastcall Button2Click(TObject* Sender);
  private: // User declarations
  public: // User declarations
	__fastcall TForm1(TComponent* Owner);
    void AddPartnerPanel(int id, String type, String name, String dir,
        String phone, int rating,int discount, int top);
	void __fastcall PanelClick(TObject* Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1* Form1;
//---------------------------------------------------------------------------
#endif

