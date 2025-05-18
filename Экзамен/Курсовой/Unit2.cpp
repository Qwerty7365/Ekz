//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEmployees* Employees;
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------
__fastcall TEmployees::TEmployees(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TEmployees::FormClose(TObject* Sender, TCloseAction &Action)
{
    delete this;
}
//---------------------------------------------------------------------------
void __fastcall TEmployees::Button1Click(TObject* Sender)
{
    if (ComboBox1->ItemIndex != -1) // ���������, ��� ������� ����
    {
        // �������� ��������� ����
        String selectedField = ComboBox1->Text;
        // ��������� ������ � �����������
        ADOQuery1->Close(); // ��������� ������� ������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM Employees ORDER BY " + selectedField);
        ADOQuery1->Open(); // ��������� ���������� ������
    } else {
        ShowMessage("�������� ���� ��� ����������!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TEmployees::FormCreate(TObject* Sender)
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
void __fastcall TEmployees::Edit1Change(TObject* Sender)
{
    // �������� ����� �� Edit1
    String filterText = Edit1->Text.Trim();
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Employees");
    if (!filterText.IsEmpty()) // ���� ����� �� ������, ��������� ������
    {
        ADOQuery1->SQL->Add("WHERE Position LIKE :Filter");
        ADOQuery1->Parameters->ParamByName("Filter")->Value =
            "%" + filterText + "%"; // ��������� ����������
    }
    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TEmployees::Edit2Change(TObject* Sender)
{
    // �������� ��������� ����� � ���� ��� ������
    String searchText = Edit2->Text;
    // ���� ���� ������, ���������� ��� ������
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Employees";
        ADOQuery1->Open();
    } else {
        // ������ SQL-������ � ��������� �� ���� ����� �������
        String filterQuery = "SELECT * FROM Employees WHERE ";
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

void __fastcall TEmployees::Button2Click(TObject* Sender)
{
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Employees");

    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TEmployees::Button4Click(TObject* Sender)
{
    try {
        // �������� �� ������������� �����
        if (Edit3->Text.IsEmpty() || Edit4->Text.IsEmpty() ||
            Edit5->Text.IsEmpty() || Edit6->Text.IsEmpty() ||
            Edit7->Text.IsEmpty() || Edit8->Text.IsEmpty() ||
            Edit9->Text.IsEmpty() || Edit10->Text.IsEmpty())
        {
            ShowMessage("��� ���� ������ ���� ���������!");
            return;
        }

		/*double salary =
            StrToFloat(Edit8->Text); // ����������� ������ � ����� ��� Salary */

        // �������� ������� ���� � Edit4 (������ DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit6->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("���� ������ ���� � ������� DD.MM.YYYY!");
            return;
        }

        // �������� ������� ���� � Edit4 (������ DD.MM.YYYY)
        if (!TRegEx::IsMatch(
				Edit9->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("���� ������ ���� � ������� DD.MM.YYYY!");
            return;
        }

        // ����������� ������ � ���� ��� HireDate
        // ���������� ������ � �������
        String sql =
            "INSERT INTO Employees (LastName, FirstName, MiddleName, BirthDate, Position, Salary, HireDate, PhoneNumber) VALUES (:LastName, :FirstName, :MiddleName, :BirthDate, :Position, :Salary, :HireDate, :PhoneNumber)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("LastName")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("FirstName")->Value = Edit4->Text;
        ADOQuery1->Parameters->ParamByName("MiddleName")->Value = Edit5->Text;
		ADOQuery1->Parameters->ParamByName("BirthDate")->Value = Edit6->Text;
        ADOQuery1->Parameters->ParamByName("Position")->Value = Edit7->Text;
		ADOQuery1->Parameters->ParamByName("Salary")->Value = Edit8->Text;
		ADOQuery1->Parameters->ParamByName("HireDate")->Value = Edit9->Text;
        ADOQuery1->Parameters->ParamByName("PhoneNumber")->Value = Edit10->Text;
        ADOQuery1->ExecSQL();
        // ��������� �� �������� �������
        ShowMessage("������ ������� ���������!");
        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Employees");
        // ��������� ������
        ADOQuery1->Open();
        // ������� �����
        Edit3->Clear();
        Edit4->Clear();
        Edit5->Clear();
        Edit6->Clear();
        Edit7->Clear();
        Edit8->Clear();
        Edit9->Clear();
        Edit10->Clear();
    } catch (Exception &E) {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TEmployees::Button3Click(TObject* Sender)
{
    ADOQuery1->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TEmployees::Button5Click(TObject* Sender)
{
    Edit3->Clear();
    Edit4->Clear();
    Edit5->Clear();
    Edit6->Clear();
    Edit7->Clear();
    Edit8->Clear();
    Edit9->Clear();
    Edit10->Clear();
}
//---------------------------------------------------------------------------

