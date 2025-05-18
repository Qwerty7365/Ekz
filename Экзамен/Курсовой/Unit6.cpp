//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSupplies* Supplies;
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------
__fastcall TSupplies::TSupplies(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TSupplies::FormClose(TObject* Sender, TCloseAction &Action)
{
    delete this;
}
//---------------------------------------------------------------------------

void __fastcall TSupplies::Button1Click(TObject* Sender)
{
    if (ComboBox1->ItemIndex != -1) // ���������, ��� ������� ����
    {
        // �������� ��������� ����
        String selectedField = ComboBox1->Text;
        // ��������� ������ � �����������
        ADOQuery1->Close(); // ��������� ������� ������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Supplies ORDER BY " + selectedField);
        ADOQuery1->Open(); // ��������� ���������� ������
    } else {
        ShowMessage("�������� ���� ��� ����������!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TSupplies::FormCreate(TObject* Sender)
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
void __fastcall TSupplies::Edit1KeyDown(
    TObject* Sender, WORD &Key, TShiftState Shift)
{
    if (Key == VK_RETURN) { // ���������, ��� ������ ������� Enter
        String dateText = Edit1->Text; // �������� ����� �� ���� �����
        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Supplies");
        // ���� ���� ����� ������, ��������� ����������
        if (dateText.IsEmpty()) {
            ADOQuery1->Open(); // ��������� ������ ��� �������
            return;
        }
        // ���������, ��� ���� ������� � ������� YYYY-MM-DD
        if (dateText.Length() == 10 && dateText[5] == '-' && dateText[8] == '-')
        {
            // ����������� ������ � TDateTime, ��������� ������ YYYY-MM-DD
            try {
                int year = StrToInt(dateText.SubString(1, 4));
                int month = StrToInt(dateText.SubString(6, 2));
                int day = StrToInt(dateText.SubString(9, 2));
                // ���������, �������� �� ���� ��������������
                TDateTime dt(year, month, day);
                // ��������� ������� ���������� ��� ���� ���� DATETIME
                ADOQuery1->SQL->Add("WHERE SupplyDate = :FilterDate");
                ADOQuery1->Parameters->ParamByName("FilterDate")->Value = dt;
                // ��������� ������
                ADOQuery1->Open();
            } catch (...) {
                // ���� ��������� ������ ��� �������������� ����
                ShowMessage(
                    "������ ���������� �������. ��������� ������������ ����.");
            }
        } else {
            // ���� ������ ���� �� �������������
            ShowMessage("���� ������ ���� � ������� YYYY-MM-DD.");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TSupplies::Edit2Change(TObject* Sender)
{
    // �������� ��������� ����� � ���� ��� ������
    String searchText = Edit2->Text;
    // ���� ���� ������, ���������� ��� ������
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Supplies";
        ADOQuery1->Open();
    } else {
        // ������ SQL-������ � ��������� �� ���� ����� �������
        String filterQuery = "SELECT * FROM Supplies WHERE ";
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

void __fastcall TSupplies::Button2Click(TObject* Sender)
{
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Supplies");

    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TSupplies::Button7Click(TObject* Sender)
{
    ADOQuery1->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TSupplies::Button3Click(TObject* Sender)
{
    try {
        // �������� �� ������������� �����
        if (Edit3->Text.IsEmpty() || Edit5->Text.IsEmpty() ||
            Edit6->Text.IsEmpty() || Edit4->Text.IsEmpty())
        {
            ShowMessage("��� ���� ������ ���� ���������!");
            return;
        }

        // �������� ������� ���� � Edit4 (������ DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit4->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("���� ������ ���� � ������� DD.MM.YYYY!");
            return;
        }

        // ���������� ������ � �������
        String sql =
            "INSERT INTO Supplies (SupplyID, SupplyDate, ProductID, Quantity) VALUES (:SupplyID, :SupplyDate, :ProductID, :Quantity)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("SupplyID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("SupplyDate")->Value = Edit4->Text;
        ADOQuery1->Parameters->ParamByName("ProductID")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value =
            Edit6->Text.ToInt();
        ADOQuery1->ExecSQL();

        // ��������� �� �������� �������
        ShowMessage("������ ������� ���������!");

        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Supplies");

        // ��������� ������
        ADOQuery1->Open();

        // ������� �����
        Edit3->Clear();
        Edit5->Clear();
        Edit6->Clear();
        Edit4->Clear();
    }

    catch (Exception &E)
    {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TSupplies::Button4Click(TObject* Sender)
{
    Edit3->Clear();
    Edit5->Clear();
    Edit6->Clear();
    Edit4->Clear();
}
//---------------------------------------------------------------------------

