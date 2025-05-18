//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOrders* Orders;
#include <System.RegularExpressions.hpp>
//---------------------------------------------------------------------------
__fastcall TOrders::TOrders(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TOrders::FormClose(TObject* Sender, TCloseAction &Action)
{
    delete this;
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button1Click(TObject* Sender)
{
    if (ComboBox1->ItemIndex != -1) // ���������, ��� ������� ����
    {
        // �������� ��������� ����
        String selectedField = ComboBox1->Text;
        // ��������� ������ � �����������
        ADOQuery1->Close(); // ��������� ������� ������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Orders ORDER BY " + selectedField);
        ADOQuery1->Open(); // ��������� ���������� ������
    } else {
        ShowMessage("�������� ���� ��� ����������!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrders::FormCreate(TObject* Sender)
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

void __fastcall TOrders::Edit1Change(TObject* Sender)
{
    // �������� ����� �� Edit1
    String filterText = Edit1->Text.Trim();
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Orders");
    if (!filterText.IsEmpty()) // ���� ����� �� ������, ��������� ������
    {
        ADOQuery1->SQL->Add("WHERE Waiter LIKE :Filter");
        ADOQuery1->Parameters->ParamByName("Filter")->Value =
            "%" + filterText + "%";
    }
    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Edit2Change(TObject* Sender)
{
    // �������� ��������� ����� � ���� ��� ������
    String searchText = Edit2->Text;
    // ���� ���� ������, ���������� ��� ������
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Orders";
        ADOQuery1->Open();
    } else {
        // ������ SQL-������ � ��������� �� ���� ����� �������
        String filterQuery = "SELECT * FROM Orders WHERE ";
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

void __fastcall TOrders::Button2Click(TObject* Sender)
{
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Orders");

    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button3Click(TObject* Sender)
{
    // ������� ��������� ������ �����
    TOrderDetails* formDetails = new TOrderDetails(this);

    // ���������� ������ �����
    formDetails->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::FormActivate(TObject* Sender)
{
    Edit5->Text = DateToStr(Now());
    try {
        // ��������� SQL-�������
        ADOQuery2->SQL->Text =
            "SELECT LastName FROM Employees WHERE Position = '��������'";
        ADOQuery2->Open();

        // ������� ComboBox ����� ����������� �������
        ComboBox2->Items->Clear();

        // ���������� ComboBox
        while (!ADOQuery2->Eof) {
            ComboBox2->Items->Add(ADOQuery2->FieldByName("LastName")->AsString);
            ADOQuery2->Next();
        }

        // �������� �������
        ADOQuery2->Close();
    } catch (Exception &E) {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button4Click(TObject* Sender)
{
	try {
        // �������� �� ������������� �����
        if (Edit3->Text.IsEmpty() || Edit5->Text.IsEmpty() ||
            ComboBox2->Text.IsEmpty() || Edit4->Text.IsEmpty())
        {
            ShowMessage("��� ���� ������ ���� ���������!");
            return;
        }

        // �������� ������� ���� � Edit5 (������ DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit5->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("���� ������ ���� � ������� DD.MM.YYYY!");
            return;
        }

        // ���������� ������ � �������
        String sql =
            "INSERT INTO Orders (OrderID, OrderDate, Waiter, TotalAmount) VALUES (:OrderID, :OrderDate, :Waiter, :TotalAmount)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("OrderID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("OrderDate")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("Waiter")->Value = ComboBox2->Text;
        ADOQuery1->Parameters->ParamByName("TotalAmount")->Value =
            Edit4->Text.ToInt();
        ADOQuery1->ExecSQL();

        // ��������� �� �������� �������
        ShowMessage("������ ������� ���������!");

        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Orders");

        // ��������� ������
        ADOQuery1->Open();

        // ������� �����
        Edit3->Clear();
        Edit5->Clear();
        ComboBox2->Text = "";
        Edit4->Clear();
    }

    catch (Exception &E)
    {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button5Click(TObject* Sender)
{
    ADOQuery1->Delete();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button6Click(TObject* Sender)
{
    Edit3->Clear();
    Edit5->Clear();
    ComboBox2->Text = "";
    Edit4->Clear();
}
//---------------------------------------------------------------------------

