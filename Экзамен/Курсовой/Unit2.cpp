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
    if (ComboBox1->ItemIndex != -1) // Проверяем, что выбрано поле
    {
        // Получаем выбранное поле
        String selectedField = ComboBox1->Text;
        // Обновляем запрос с сортировкой
        ADOQuery1->Close(); // Закрываем текущий запрос
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM Employees ORDER BY " + selectedField);
        ADOQuery1->Open(); // Открываем обновлённый запрос
    } else {
        ShowMessage("Выберите поле для сортировки!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TEmployees::FormCreate(TObject* Sender)
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
void __fastcall TEmployees::Edit1Change(TObject* Sender)
{
    // Получаем текст из Edit1
    String filterText = Edit1->Text.Trim();
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Employees");
    if (!filterText.IsEmpty()) // Если текст не пустой, добавляем фильтр
    {
        ADOQuery1->SQL->Add("WHERE Position LIKE :Filter");
        ADOQuery1->Parameters->ParamByName("Filter")->Value =
            "%" + filterText + "%"; // Частичное совпадение
    }
    // Выполняем запрос
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TEmployees::Edit2Change(TObject* Sender)
{
    // Получаем введенный текст в поле для поиска
    String searchText = Edit2->Text;
    // Если поле пустое, показываем все данные
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Employees";
        ADOQuery1->Open();
    } else {
        // Строим SQL-запрос с проверкой по всем полям таблицы
        String filterQuery = "SELECT * FROM Employees WHERE ";
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

void __fastcall TEmployees::Button2Click(TObject* Sender)
{
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Employees");

    // Выполняем запрос
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TEmployees::Button4Click(TObject* Sender)
{
    try {
        // Проверка на заполненность полей
        if (Edit3->Text.IsEmpty() || Edit4->Text.IsEmpty() ||
            Edit5->Text.IsEmpty() || Edit6->Text.IsEmpty() ||
            Edit7->Text.IsEmpty() || Edit8->Text.IsEmpty() ||
            Edit9->Text.IsEmpty() || Edit10->Text.IsEmpty())
        {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

		/*double salary =
            StrToFloat(Edit8->Text); // Преобразуем строку в число для Salary */

        // Проверка формата даты в Edit4 (формат DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit6->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("Дата должна быть в формате DD.MM.YYYY!");
            return;
        }

        // Проверка формата даты в Edit4 (формат DD.MM.YYYY)
        if (!TRegEx::IsMatch(
				Edit9->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("Дата должна быть в формате DD.MM.YYYY!");
            return;
        }

        // Преобразуем строку в дату для HireDate
        // Добавление данных в таблицу
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
        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");
        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Employees");
        // Выполняем запрос
        ADOQuery1->Open();
        // Очистка полей
        Edit3->Clear();
        Edit4->Clear();
        Edit5->Clear();
        Edit6->Clear();
        Edit7->Clear();
        Edit8->Clear();
        Edit9->Clear();
        Edit10->Clear();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
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

