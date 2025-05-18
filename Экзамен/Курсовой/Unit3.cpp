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
    if (ComboBox1->ItemIndex != -1) // Проверяем, что выбрано поле
    {
        // Получаем выбранное поле
        String selectedField = ComboBox1->Text;
        // Обновляем запрос с сортировкой
        ADOQuery1->Close(); // Закрываем текущий запрос
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM Warehouse ORDER BY " + selectedField);
        ADOQuery1->Open(); // Открываем обновлённый запрос
    } else {
        ShowMessage("Выберите поле для сортировки!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TWarehouse::FormCreate(TObject* Sender)
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
void __fastcall TWarehouse::Edit1KeyDown(
    TObject* Sender, WORD &Key, TShiftState Shift)

{
    // Проверяем, была ли нажата клавиша Enter (код 13)
    if (Key == VK_RETURN) {
        String filterText = Edit1->Text.Trim(); // Получаем текст из поля ввода
        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM Warehouse"); // Замените на вашу таблицу
        if (!filterText.IsEmpty()) { // Если текст не пустой, добавляем фильтр
            try {
                // Преобразуем строку в число (если это возможно)
                Integer quan = StrToInt(filterText);
                // Добавляем условие фильтрации по полю INT
                ADOQuery1->SQL->Add("WHERE Quantity = :Filter");
                ADOQuery1->Parameters->ParamByName("Filter")->Value = quan;
            } catch (...) {
                // Если введённое значение не может быть преобразовано в число, отключаем фильтрацию
                ADOQuery1->SQL->Add(
                    "WHERE 1=0"); // Невозможный фильтр, чтобы вернуть пустые данные
            }
        }
        // Выполняем запрос
        ADOQuery1->Open();
    }
}
//---------------------------------------------------------------------------

void __fastcall TWarehouse::Edit2Change(TObject* Sender)
{
    // Получаем введенный текст в поле для поиска
    String searchText = Edit2->Text;
    // Если поле пустое, показываем все данные
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Warehouse";
        ADOQuery1->Open();
    } else {
        // Строим SQL-запрос с проверкой по всем полям таблицы
        String filterQuery = "SELECT * FROM Warehouse WHERE ";
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

void __fastcall TWarehouse::Button2Click(TObject* Sender)
{
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Warehouse");

    // Выполняем запрос
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
        // Проверка на заполненность полей
        if (Edit3->Text.IsEmpty() || Edit5->Text.IsEmpty() ||
            Edit6->Text.IsEmpty() || Edit4->Text.IsEmpty())
        {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

        // Проверка формата даты в Edit6 (формат DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit6->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("Дата должна быть в формате DD.MM.YYYY!");
            return;
        }

        // Добавление данных в таблицу
        String sql =
            "INSERT INTO Warehouse (ProductID, ProductName, Quantity, SupplyDate) VALUES (:ProductID, :ProductName, :Quantity, :SupplyDate)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("ProductID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("ProductName")->Value = Edit4->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("SupplyDate")->Value = Edit6->Text;
        ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Warehouse");

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

void __fastcall TWarehouse::Button4Click(TObject* Sender)
{
    Edit3->Clear();
    Edit5->Clear();
    Edit6->Clear();
    Edit4->Clear();
}
//---------------------------------------------------------------------------

