//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMenuRes* MenuRes;
//---------------------------------------------------------------------------
__fastcall TMenuRes::TMenuRes(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TMenuRes::FormClose(TObject* Sender, TCloseAction &Action)
{
    delete this;
}
//---------------------------------------------------------------------------
void __fastcall TMenuRes::Button1Click(TObject* Sender)
{
    if (ComboBox1->ItemIndex != -1) // ���������, ��� ������� ����
    {
        // �������� ��������� ����
        String selectedField = ComboBox1->Text;
        // ��������� ������ � �����������
        ADOQuery1->Close(); // ��������� ������� ������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Menu ORDER BY " + selectedField);
        ADOQuery1->Open(); // ��������� ���������� ������
    } else {
        ShowMessage("�������� ���� ��� ����������!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::FormCreate(TObject* Sender)
{
    // ��������� ������
    ADOQuery1->Open();
    // ������� ComboBox
    ComboBox1->Clear();
    // ��������� ComboBox ������� �����
    for (int i = 0; i < ADOQuery1->FieldCount; i++) {
        ComboBox1->Items->Add(ADOQuery1->Fields->Fields[i]->FieldName);
    }
    // ������������� ������ ������� ��� ���������
    if (ComboBox1->Items->Count > 0)
        ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Edit2Change(TObject* Sender)
{
    // �������� ��������� ����� � ���� ��� ������
    String searchText = Edit2->Text;
    // ���� ���� ������, ���������� ��� ������
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Menu";
        ADOQuery1->Open();
    } else {
        // ������ SQL-������ � ��������� �� ���� ����� �������
        String filterQuery = "SELECT * FROM Menu WHERE ";
        // �������� ��������� ������� � ������ ���� �����
        TField* field;
        bool firstField = true;
        for (int i = 0; i < ADOQuery1->Fields->Count; i++) {
            field = ADOQuery1->Fields->Fields[i];
            // ��� ������� ���� ������� ������� ������, ���� ��� �� ������ ����
            if (!firstField)
                filterQuery += " OR ";
            else
                firstField = false;
            // ��������� ������� LIKE ��� ������� ����
            filterQuery += field->FieldName + " LIKE '%" + searchText + "%'";
        }
        // ��������� ������ � ��������
        ADOQuery1->SQL->Text = filterQuery;
        ADOQuery1->Open();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Button2Click(TObject* Sender)
{
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Menu");

    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Button3Click(TObject* Sender)
{
    try {
        // �������� �� ������������� �����
        if (Edit3->Text.IsEmpty() || Edit4->Text.IsEmpty()) {
            ShowMessage("��� ���� ������ ���� ���������!");
            return;
        }

        // ���������� ������ � �������
        String sql =
            "INSERT INTO Menu (DishName, Price) VALUES (:DishName, :Price)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("DishName")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("Price")->Value =
            Edit4->Text.ToInt();
        ADOQuery1->ExecSQL();

        // ��������� �� �������� �������
        ShowMessage("������ ������� ���������!");

        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Menu");

        // ��������� ������
        ADOQuery1->Open();

        // ������� �����
        Edit3->Clear();
        Edit4->Clear();
    } catch (Exception &E) {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Button4Click(TObject* Sender)
{
    ADOQuery1->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Button5Click(TObject* Sender)
{
    Edit3->Clear();
    Edit4->Clear();
}
//---------------------------------------------------------------------------

