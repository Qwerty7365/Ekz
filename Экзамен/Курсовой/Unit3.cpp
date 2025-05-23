//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TWarehouse* Warehouse;
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------
__fastcall TWarehouse::TWarehouse(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TWarehouse::FormClose(TObject* Sender, TCloseAction &Action)
{
    delete this;
}
//---------------------------------------------------------------------------
void __fastcall TWarehouse::Button1Click(TObject* Sender)
{
    if (ComboBox1->ItemIndex != -1) // ���������, ��� ������� ����
    {
        // �������� ��������� ����
        String selectedField = ComboBox1->Text;
        // ��������� ������ � �����������
        ADOQuery1->Close(); // ��������� ������� ������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM Warehouse ORDER BY " + selectedField);
        ADOQuery1->Open(); // ��������� ���������� ������
    } else {
        ShowMessage("�������� ���� ��� ����������!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TWarehouse::FormCreate(TObject* Sender)
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
void __fastcall TWarehouse::Edit1KeyDown(
    TObject* Sender, WORD &Key, TShiftState Shift)

{
    // ���������, ���� �� ������ ������� Enter (��� 13)
    if (Key == VK_RETURN) {
        String filterText = Edit1->Text.Trim(); // �������� ����� �� ���� �����
        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM Warehouse"); // �������� �� ���� �������
        if (!filterText.IsEmpty()) { // ���� ����� �� ������, ��������� ������
            try {
                // ����������� ������ � ����� (���� ��� ��������)
                Integer quan = StrToInt(filterText);
                // ��������� ������� ���������� �� ���� INT
                ADOQuery1->SQL->Add("WHERE Quantity = :Filter");
                ADOQuery1->Parameters->ParamByName("Filter")->Value = quan;
            } catch (...) {
                // ���� �������� �������� �� ����� ���� ������������� � �����, ��������� ����������
                ADOQuery1->SQL->Add(
                    "WHERE 1=0"); // ����������� ������, ����� ������� ������ ������
            }
        }
        // ��������� ������
        ADOQuery1->Open();
    }
}
//---------------------------------------------------------------------------

void __fastcall TWarehouse::Edit2Change(TObject* Sender)
{
    // �������� ��������� ����� � ���� ��� ������
    String searchText = Edit2->Text;
    // ���� ���� ������, ���������� ��� ������
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Warehouse";
        ADOQuery1->Open();
    } else {
        // ������ SQL-������ � ��������� �� ���� ����� �������
        String filterQuery = "SELECT * FROM Warehouse WHERE ";
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

void __fastcall TWarehouse::Button2Click(TObject* Sender)
{
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Warehouse");

    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TWarehouse::Button7Click(TObject* Sender)
{
    ADOQuery1->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TWarehouse::Button3Click(TObject* Sender)
{
    try {
        // �������� �� ������������� �����
        if (Edit3->Text.IsEmpty() || Edit5->Text.IsEmpty() ||
            Edit6->Text.IsEmpty() || Edit4->Text.IsEmpty())
        {
            ShowMessage("��� ���� ������ ���� ���������!");
            return;
        }

        // �������� ������� ���� � Edit6 (������ DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit6->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("���� ������ ���� � ������� DD.MM.YYYY!");
            return;
        }

        // ���������� ������ � �������
        String sql =
            "INSERT INTO Warehouse (ProductID, ProductName, Quantity, SupplyDate) VALUES (:ProductID, :ProductName, :Quantity, :SupplyDate)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("ProductID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("ProductName")->Value = Edit4->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("SupplyDate")->Value = Edit6->Text;
        ADOQuery1->ExecSQL();

        // ��������� �� �������� �������
        ShowMessage("������ ������� ���������!");

        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Warehouse");

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

void __fastcall TWarehouse::Button4Click(TObject* Sender)
{
    Edit3->Clear();
    Edit5->Clear();
    Edit6->Clear();
    Edit4->Clear();
}
//---------------------------------------------------------------------------

