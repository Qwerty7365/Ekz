//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button1Click(TObject *Sender)
{
    try {
        // Проверка на заполненность полей
        if (Edit1->Text.IsEmpty() || Edit2->Text.IsEmpty() ||
            Edit3->Text.IsEmpty() || Edit4->Text.IsEmpty() ||
            Edit5->Text.IsEmpty() || Edit6->Text.IsEmpty() ||
            Edit7->Text.IsEmpty() || Edit8->Text.IsEmpty())
        {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

        // Добавление данных в таблицу
        String sql =
            "INSERT INTO Partners (Type_partner, Name_partner, Director, Email, Phone_number, Partners_address, INN, Rating) VALUES (:Type_partner, :Name_partner, :Director, :Email, :Phone_number, :Partners_address, :INN, :Rating)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("Type_partner")->Value = Edit1->Text;
        ADOQuery1->Parameters->ParamByName("Name_partner")->Value = Edit2->Text;
        ADOQuery1->Parameters->ParamByName("Director")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("Email")->Value = Edit4->Text;
        ADOQuery1->Parameters->ParamByName("Phone_number")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("Partners_address")->Value =
            Edit6->Text;
        ADOQuery1->Parameters->ParamByName("INN")->Value = Edit7->Text;
        ADOQuery1->Parameters->ParamByName("Rating")->Value =
			Edit8->Text.ToInt();

        	ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Partners");
        // Выполняем запрос
        ADOQuery1->Open();

        // Очистка полей
        Edit1->Clear();
        Edit2->Clear();
        Edit3->Clear();
        Edit4->Clear();
        Edit5->Clear();
        Edit6->Clear();
        Edit7->Clear();
        Edit8->Clear();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button2Click(TObject *Sender)
{
    Edit1->Clear();
    Edit2->Clear();
    Edit3->Clear();
    Edit4->Clear();
    Edit5->Clear();
    Edit6->Clear();
    Edit7->Clear();
    Edit8->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button3Click(TObject *Sender)
{
    Edit1->Clear();
    Edit2->Clear();
    Edit3->Clear();
    Edit4->Clear();
    Edit5->Clear();
    Edit6->Clear();
    Edit7->Clear();
    Edit8->Clear();

    Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm2::FormClose(TObject *Sender, TCloseAction &Action)
{
	ADOQuery1->ExecSQL();
    ADOQuery1->SQL->Text =
		"SELECT ID_partner, Type_partner, Name_partner, Director, Phone_number, Rating, Discount FROM Partners";

    try {
        ADOQuery1->Open(); // Выполняем запрос
        int currentTop = 0; // Начальная позиция для первой панели
        // Перебираем результаты запроса

        while (!ADOQuery1->Eof) {
            int id = ADOQuery1->FieldByName("ID_partner")->AsInteger;
            String type = ADOQuery1->FieldByName("Type_partner")->AsString;
            String name = ADOQuery1->FieldByName("Name_partner")->AsString;
            String dir = ADOQuery1->FieldByName("Director")->AsString;
			String phone = ADOQuery1->FieldByName("Phone_number")->AsString;
			int discount = ADOQuery1->FieldByName("Discount")->AsInteger;
			int rating = ADOQuery1->FieldByName("Rating")->AsInteger;
            Form1->AddPartnerPanel(id, type, name, dir, phone, rating, discount, currentTop);
            currentTop += 100; // Высота панели
            ADOQuery1->Next();
        }

    } catch (Exception &e) {
        ShowMessage("Ошибка выполнения запроса: " + e.Message);
	}
}
//---------------------------------------------------------------------------

