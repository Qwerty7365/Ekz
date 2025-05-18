//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit7.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOrderDetails* OrderDetails;
//---------------------------------------------------------------------------
__fastcall TOrderDetails::TOrderDetails(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
void __fastcall TOrderDetails::FormClose(TObject* Sender, TCloseAction &Action)
{
    delete this;
}
//---------------------------------------------------------------------------
void __fastcall TOrderDetails::Button1Click(TObject* Sender)
{
    if (ComboBox1->ItemIndex != -1) // ���������, ��� ������� ����
    {
        // �������� ��������� ����
        String selectedField = ComboBox1->Text;
        // ��������� ������ � �����������
        ADOQuery1->Close(); // ��������� ������� ������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM OrderDetails ORDER BY " + selectedField);
        ADOQuery1->Open(); // ��������� ���������� ������
    } else {
        ShowMessage("�������� ���� ��� ����������!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TOrderDetails::FormCreate(TObject* Sender)
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

void __fastcall TOrderDetails::Edit2Change(TObject* Sender)
{
    // �������� ��������� ����� � ���� ��� ������
    String searchText = Edit2->Text;
    // ���� ���� ������, ���������� ��� ������
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text =
            "SELECT * FROM OrderDetails";
        ADOQuery1->Open();
    } else {
        // ������ SQL-������ � ��������� �� ���� ����� �������
        String filterQuery = "SELECT * FROM OrderDetails WHERE ";
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

void __fastcall TOrderDetails::Button2Click(TObject* Sender)
{
    // ��������� ������� ������
    ADOQuery1->Close();
    // ������� ������� SQL � ������ ����� ������ � ��������
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM OrderDetails");

    // ��������� ������
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TOrderDetails::Edit1KeyDown(
    TObject* Sender, WORD &Key, TShiftState Shift)

{
    // ���������, ���� �� ������ ������� Enter
    if (Key == VK_RETURN) {
        String filterText = Edit1->Text.Trim(); // �������� ����� �� ���� �����
        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM OrderDetails");
        if (!filterText.IsEmpty()) { // ���� ����� �� ������, ��������� ������
            try {
                // ����������� ������ � ����� (���� ��� ��������)
                Integer quan = StrToInt(filterText);
                // ��������� ������� ���������� �� ���� INT
                ADOQuery1->SQL->Add(
					"WHERE OrderID = :Filter");
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

void __fastcall TOrderDetails::FormActivate(TObject* Sender)
{
    try {
        // ��������� SQL-�������
        ADOQuery2->SQL->Text = "SELECT DishName FROM Menu";
        ADOQuery2->Open();

        // ������� ComboBox ����� ����������� �������
        ComboBox2->Items->Clear();

        // ���������� ComboBox
        while (!ADOQuery2->Eof) {
            ComboBox2->Items->Add(ADOQuery2->FieldByName("DishName")->AsString);
            ADOQuery2->Next();
        }

        // �������� �������
        ADOQuery2->Close();
    } catch (Exception &E) {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrderDetails::Button3Click(TObject* Sender)
{
    try {
        // �������� �� ������������� �����
        if (Edit3->Text.IsEmpty() || ComboBox2->Text.IsEmpty() ||
            Edit4->Text.IsEmpty()) {
            ShowMessage("��� ���� ������ ���� ���������!");
            return;
        }

        // ���������� ������ � �������
        String sql =
            "INSERT INTO OrderDetails (OrderID, DishName, Quantity) VALUES (:OrderID, :DishName, :Quantity)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("OrderID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("DishName")->Value = ComboBox2->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value =
            Edit4->Text.ToInt();
        ADOQuery1->ExecSQL();

        // ��������� �� �������� �������
        ShowMessage("������ ������� ���������!");

        // ��������� ������� ������
        ADOQuery1->Close();
        // ������� ������� SQL � ������ ����� ������ � ��������
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM OrderDetails");

        // ��������� ������
        ADOQuery1->Open();

        // ������� �����
        Edit3->Clear();
        ComboBox2->Text = "";
        Edit4->Clear();
    } catch (Exception &E) {
        ShowMessage("������: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrderDetails::Button5Click(TObject* Sender)
{
    Edit3->Clear();
    ComboBox2->Text = "";
    Edit4->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TOrderDetails::Button4Click(TObject* Sender)
{
    ADOQuery1->Delete();
}
//---------------------------------------------------------------------------

