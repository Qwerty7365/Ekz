//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4* Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
// Метод для загрузки данных партнера
void TForm4::LoadPartnerData(int partnerId)
{
    ADOQuery1->SQL->Text = "SELECT * FROM Partners WHERE ID_partner = :id";
    ADOQuery1->Parameters->ParamByName("id")->Value = partnerId;
    ADOQuery1->Open();
    if (!ADOQuery1->IsEmpty()) {
        Edit1->Text = ADOQuery1->FieldByName("Type_partner")->AsString;
        Edit2->Text = ADOQuery1->FieldByName("Name_partner")->AsString;
        Edit3->Text = ADOQuery1->FieldByName("Director")->AsString;
        Edit4->Text = ADOQuery1->FieldByName("Email")->AsString;
        Edit5->Text = ADOQuery1->FieldByName("Phone_number")->AsString;
        Edit6->Text = ADOQuery1->FieldByName("Partners_address")->AsString;
        Edit7->Text = ADOQuery1->FieldByName("INN")->AsString;
        Edit8->Text = ADOQuery1->FieldByName("Rating")->AsString;
    }
}
void __fastcall TForm4::Button4Click(TObject* Sender)
{
	ShowMessage(SelectedId);
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject* Sender)
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

		// Обновление записи в таблице
        String sql = "UPDATE Partners SET "
					 "Type_partner = :Type_partner, "
                     "Name_partner = :Name_partner, "
                     "Director = :Director, "
					 "Email = :Email, "
                     "Phone_number = :Phone_number, "
                     "Partners_address = :Partners_address, "
					 "INN = :INN, "
                     "Rating = :Rating "
                     "WHERE ID_partner = :ID";

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
		ADOQuery1->Parameters->ParamByName("ID")->Value =
			SelectedId; // Используем выбранный ID

		// Выполнение запроса
		ADOQuery1->ExecSQL();

		// Сообщение об успешном обновлении
		ShowMessage("Запись успешно обновлена!");

		// Обновляем данные в таблице
		ADOQuery1->Close();
        ADOQuery1->SQL->Clear();
		ADOQuery1->SQL->Add("SELECT * FROM Partners");
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

    Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm4::FormClose(TObject *Sender, TCloseAction &Action)
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

