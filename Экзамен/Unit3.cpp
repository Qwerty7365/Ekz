//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm3::FormCreate(TObject *Sender)
{
    try {
        // Настройка SQL-запроса
        ADOQuery1->SQL->Text = "SELECT Name_product FROM Product";
        ADOQuery1->Open();

        // Очистка ComboBox перед добавлением записей
        ComboBox1->Items->Clear();

        // Заполнение ComboBox
        while (!ADOQuery1->Eof) {
            ComboBox1->Items->Add(
                ADOQuery1->FieldByName("Name_product")->AsString);
            ADOQuery1->Next();
        }

        // Закрытие запроса
        ADOQuery1->Close();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button1Click(TObject *Sender)
{
    try {
        String sql =
			"INSERT INTO Partners_product (Products, ID_partner, Quantity, Date_sale)  VALUES (:Products, :ID_partner, :Quantity, :Date_sale)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("Products")->Value = ComboBox1->Text;
        ADOQuery1->Parameters->ParamByName("ID_partner")->Value = Edit1->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value =
            Edit2->Text.ToInt();
        ADOQuery1->Parameters->ParamByName("Date_sale")->Value = Edit3->Text;
        ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Partners_product");
        // Выполняем запрос
        ADOQuery1->Open();

        // Очистка полей
        ComboBox1->Text = "";
        Edit1->Clear();
        Edit2->Clear();
        Edit3->Clear();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button2Click(TObject *Sender)
{
    ComboBox1->Text = "";
    Edit1->Clear();
    Edit2->Clear();
    Edit3->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button3Click(TObject *Sender)
{
    ComboBox1->Text = "";
    Edit1->Clear();
    Edit2->Clear();
    Edit3->Clear();

    Close();
}
//---------------------------------------------------------------------------
