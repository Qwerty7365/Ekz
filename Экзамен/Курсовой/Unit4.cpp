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
    if (ComboBox1->ItemIndex != -1) // Проверяем, что выбрано поле
    {
        // Получаем выбранное поле
        String selectedField = ComboBox1->Text;
        // Обновляем запрос с сортировкой
        ADOQuery1->Close(); // Закрываем текущий запрос
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Menu ORDER BY " + selectedField);
        ADOQuery1->Open(); // Открываем обновлённый запрос
    } else {
        ShowMessage("Выберите поле для сортировки!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::FormCreate(TObject* Sender)
{
    // Открываем запрос
    ADOQuery1->Open();
    // Очищаем ComboBox
    ComboBox1->Clear();
    // Заполняем ComboBox именами полей
    for (int i = 0; i < ADOQuery1->FieldCount; i++) {
        ComboBox1->Items->Add(ADOQuery1->Fields->Fields[i]->FieldName);
    }
    // Устанавливаем первый элемент как выбранный
    if (ComboBox1->Items->Count > 0)
        ComboBox1->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Edit2Change(TObject* Sender)
{
    // Получаем введенный текст в поле для поиска
    String searchText = Edit2->Text;
    // Если поле пустое, показываем все данные
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Menu";
        ADOQuery1->Open();
    } else {
        // Строим SQL-запрос с проверкой по всем полям таблицы
        String filterQuery = "SELECT * FROM Menu WHERE ";
        // Получаем структуру таблицы и список всех полей
        TField* field;
        bool firstField = true;
        for (int i = 0; i < ADOQuery1->Fields->Count; i++) {
            field = ADOQuery1->Fields->Fields[i];
            // Для каждого поля создаем условие поиска, если это не первое поле
            if (!firstField)
                filterQuery += " OR ";
            else
                firstField = false;
            // Добавляем условие LIKE для каждого поля
            filterQuery += field->FieldName + " LIKE '%" + searchText + "%'";
        }
        // Выполняем запрос с фильтром
        ADOQuery1->SQL->Text = filterQuery;
        ADOQuery1->Open();
    }
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Button2Click(TObject* Sender)
{
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Menu");

    // Выполняем запрос
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TMenuRes::Button3Click(TObject* Sender)
{
    try {
        // Проверка на заполненность полей
        if (Edit3->Text.IsEmpty() || Edit4->Text.IsEmpty()) {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

        // Добавление данных в таблицу
        String sql =
            "INSERT INTO Menu (DishName, Price) VALUES (:DishName, :Price)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("DishName")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("Price")->Value =
            Edit4->Text.ToInt();
        ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Menu");

        // Выполняем запрос
        ADOQuery1->Open();

        // Очистка полей
        Edit3->Clear();
        Edit4->Clear();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
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

