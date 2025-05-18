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
    if (ComboBox1->ItemIndex != -1) // Проверяем, что выбрано поле
    {
        // Получаем выбранное поле
        String selectedField = ComboBox1->Text;
        // Обновляем запрос с сортировкой
        ADOQuery1->Close(); // Закрываем текущий запрос
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Supplies ORDER BY " + selectedField);
        ADOQuery1->Open(); // Открываем обновлённый запрос
    } else {
        ShowMessage("Выберите поле для сортировки!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TSupplies::FormCreate(TObject* Sender)
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
void __fastcall TSupplies::Edit1KeyDown(
    TObject* Sender, WORD &Key, TShiftState Shift)
{
    if (Key == VK_RETURN) { // Проверяем, что нажата клавиша Enter
        String dateText = Edit1->Text; // Получаем текст из поля ввода
        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Supplies");
        // Если поле ввода пустое, отключаем фильтрацию
        if (dateText.IsEmpty()) {
            ADOQuery1->Open(); // Выполняем запрос без фильтра
            return;
        }
        // Проверяем, что дата введена в формате YYYY-MM-DD
        if (dateText.Length() == 10 && dateText[5] == '-' && dateText[8] == '-')
        {
            // Преобразуем строку в TDateTime, используя формат YYYY-MM-DD
            try {
                int year = StrToInt(dateText.SubString(1, 4));
                int month = StrToInt(dateText.SubString(6, 2));
                int day = StrToInt(dateText.SubString(9, 2));
                // Проверяем, является ли дата действительной
                TDateTime dt(year, month, day);
                // Добавляем условие фильтрации для поля типа DATETIME
                ADOQuery1->SQL->Add("WHERE SupplyDate = :FilterDate");
                ADOQuery1->Parameters->ParamByName("FilterDate")->Value = dt;
                // Выполняем запрос
                ADOQuery1->Open();
            } catch (...) {
                // Если произошла ошибка при преобразовании даты
                ShowMessage(
                    "Ошибка применения фильтра. Проверьте правильность даты.");
            }
        } else {
            // Если формат даты не соответствует
            ShowMessage("Дата должна быть в формате YYYY-MM-DD.");
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TSupplies::Edit2Change(TObject* Sender)
{
    // Получаем введенный текст в поле для поиска
    String searchText = Edit2->Text;
    // Если поле пустое, показываем все данные
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Supplies";
        ADOQuery1->Open();
    } else {
        // Строим SQL-запрос с проверкой по всем полям таблицы
        String filterQuery = "SELECT * FROM Supplies WHERE ";
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

void __fastcall TSupplies::Button2Click(TObject* Sender)
{
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Supplies");

    // Выполняем запрос
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
        // Проверка на заполненность полей
        if (Edit3->Text.IsEmpty() || Edit5->Text.IsEmpty() ||
            Edit6->Text.IsEmpty() || Edit4->Text.IsEmpty())
        {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

        // Проверка формата даты в Edit4 (формат DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit4->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("Дата должна быть в формате DD.MM.YYYY!");
            return;
        }

        // Добавление данных в таблицу
        String sql =
            "INSERT INTO Supplies (SupplyID, SupplyDate, ProductID, Quantity) VALUES (:SupplyID, :SupplyDate, :ProductID, :Quantity)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("SupplyID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("SupplyDate")->Value = Edit4->Text;
        ADOQuery1->Parameters->ParamByName("ProductID")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value =
            Edit6->Text.ToInt();
        ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Supplies");

        // Выполняем запрос
        ADOQuery1->Open();

        // Очистка полей
        Edit3->Clear();
        Edit5->Clear();
        Edit6->Clear();
        Edit4->Clear();
    }

    catch (Exception &E)
    {
        ShowMessage("Ошибка: " + E.Message);
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

