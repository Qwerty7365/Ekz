//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------

void TForm1::FillComboBoxWithFields(TADOTable* table, TComboBox* ComboBox1)
{
    ComboBox1->Clear();
    // Проверяем, открыта ли таблица
    if (table->Active) {
        // Добавляем имена полей в ComboBox
        for (int i = 0; i < table->FieldCount; i++) {
            ComboBox1->Items->Add(table->Fields->Fields[i]->FieldName);
        }
    } else {
        ShowMessage("Таблица не активна.");
    }
}

void TForm1::FilterFunction()
{
    String custNo;
    switch (RadioGroup2->ItemIndex) {
        case 0:
			Form2->ADOTable1->Filtered = false;
			break;
		case 1:
			Form2->ADOTable1->Open();
            custNo = Edit1->Text;
            if (Edit1->Text == "") {
                return;
            }
            Form2->ADOTable1->Filter =
                "CustNo = '" + custNo + "' "; // Фильтруем по значению
            Form2->ADOTable1->Filtered = true;
            break;
        case 2:
            TDateTime StartDate = StrToDate(Edit2->Text);
            TDateTime EndDate = StrToDate(Edit3->Text);
            Form2->ADOTable1->Filter =
                "SaleDate >= #" + FormatDateTime("dd.mm.yyyy", StartDate) +
                "# AND SaleDate <= #" + FormatDateTime("dd.mm.yyyy", EndDate) +
                "#";
            Form2->ADOTable1->Filtered = true;
            break;
    }
}

void __fastcall TForm1::Button5Click(TObject* Sender)
{
    Form2->ADOTable1->Next();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject* Sender)
{
    Form2->ADOTable1->Prior();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button3Click(TObject* Sender)
{
    Form2->ADOTable1->First();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button6Click(TObject* Sender)
{
    Form2->ADOTable1->Last();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject* Sender)
{
    Form2->ADOTable1->Insert();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject* Sender)
{
    Form2->ADOTable1->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject* Sender)
{
    Form2->ADOTable1->Post();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject* Sender)
{
    Form2->ADOTable1->Edit();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject* Sender)
{
    Form2->ADOTable1->Cancel();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject* Sender)
{
    switch (RadioGroup1->ItemIndex) {
        case 0:
            Form2->ADOTable1->IndexFieldNames = ComboBox1->Text;
            break;
        case 1:
            Form2->ADOTable2->IndexFieldNames = ComboBox1->Text;
            break;
        case 2:
            Form2->ADOTable3->IndexFieldNames = ComboBox1->Text;
            break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup1Click(TObject* Sender)
{
    // Указатель на активную таблицу
    TADOTable* activeTable = nullptr;

    switch (RadioGroup1->ItemIndex) {
        case 0:
            activeTable = Form2->ADOTable1;
            break;
        case 1:
            activeTable = Form2->ADOTable2;
            break;
        case 2:
			activeTable = Form2->ADOTable3;
            break;
        default:
            return;
    }

    FillComboBoxWithFields(activeTable, ComboBox1);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit4Change(TObject* Sender)
{
	if (Edit4->Text == "") {
		Form2->ADOTable1->Filtered = false;
		return;
	}
	TLocateOptions SearchOptions;
	SearchOptions << loPartialKey << loCaseInsensitive;
	String MF = Form2->ADOTable3->MasterFields;
	Form2->ADOTable3->Active = false;
	Form2->ADOTable3->MasterFields = "";
	Form2->ADOTable3->Active = true;
	Form2->ADOTable3->Locate("LastName", Edit4->Text, SearchOptions);
	String Id = (DBGrid3->SelectedField->AsString);
	Form2->ADOTable3->MasterFields = MF;
	Form2->ADOTable3->Active = true;
	Form2->ADOTable1->Filter = "EmpNo = '"+Id+"'";
	Form2->ADOTable1->Filtered = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Change(TObject* Sender)
{
    FilterFunction();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
    FilterFunction();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioGroup2Click(TObject *Sender)
{
	switch(RadioGroup2->ItemIndex){
		case 0:
			FilterFunction();
            break;
	}
}
//---------------------------------------------------------------------------

