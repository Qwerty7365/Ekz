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
    if (ComboBox1->ItemIndex != -1) // Проверяем, что выбрано поле
    {
        // Получаем выбранное поле
        String selectedField = ComboBox1->Text;
        // Обновляем запрос с сортировкой
        ADOQuery1->Close(); // Закрываем текущий запрос
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Orders ORDER BY " + selectedField);
        ADOQuery1->Open(); // Открываем обновлённый запрос
    } else {
        ShowMessage("Выберите поле для сортировки!");
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrders::FormCreate(TObject* Sender)
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

void __fastcall TOrders::Edit1Change(TObject* Sender)
{
    // Получаем текст из Edit1
    String filterText = Edit1->Text.Trim();
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Orders");
    if (!filterText.IsEmpty()) // Если текст не пустой, добавляем фильтр
    {
        ADOQuery1->SQL->Add("WHERE Waiter LIKE :Filter");
        ADOQuery1->Parameters->ParamByName("Filter")->Value =
            "%" + filterText + "%";
    }
    // Выполняем запрос
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Edit2Change(TObject* Sender)
{
    // Получаем введенный текст в поле для поиска
    String searchText = Edit2->Text;
    // Если поле пустое, показываем все данные
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text = "SELECT * FROM Orders";
        ADOQuery1->Open();
    } else {
        // Строим SQL-запрос с проверкой по всем полям таблицы
        String filterQuery = "SELECT * FROM Orders WHERE ";
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

void __fastcall TOrders::Button2Click(TObject* Sender)
{
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM Orders");

    // Выполняем запрос
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button3Click(TObject* Sender)
{
    // Создаем экземпляр второй формы
    TOrderDetails* formDetails = new TOrderDetails(this);

    // Показываем вторую форму
    formDetails->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOrders::FormActivate(TObject* Sender)
{
    Edit5->Text = DateToStr(Now());
    try {
        // Настройка SQL-запроса
        ADOQuery2->SQL->Text =
            "SELECT LastName FROM Employees WHERE Position = 'Официант'";
        ADOQuery2->Open();

        // Очистка ComboBox перед добавлением записей
        ComboBox2->Items->Clear();

        // Заполнение ComboBox
        while (!ADOQuery2->Eof) {
            ComboBox2->Items->Add(ADOQuery2->FieldByName("LastName")->AsString);
            ADOQuery2->Next();
        }

        // Закрытие запроса
        ADOQuery2->Close();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrders::Button4Click(TObject* Sender)
{
	try {
        // Проверка на заполненность полей
        if (Edit3->Text.IsEmpty() || Edit5->Text.IsEmpty() ||
            ComboBox2->Text.IsEmpty() || Edit4->Text.IsEmpty())
        {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

        // Проверка формата даты в Edit5 (формат DD.MM.YYYY)
        if (!TRegEx::IsMatch(
                Edit5->Text, L"^([0-3][0-9])\\.([0-1][0-9])\\.([0-9]{4})$"))
        {
            ShowMessage("Дата должна быть в формате DD.MM.YYYY!");
            return;
        }

        // Добавление данных в таблицу
        String sql =
            "INSERT INTO Orders (OrderID, OrderDate, Waiter, TotalAmount) VALUES (:OrderID, :OrderDate, :Waiter, :TotalAmount)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("OrderID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("OrderDate")->Value = Edit5->Text;
        ADOQuery1->Parameters->ParamByName("Waiter")->Value = ComboBox2->Text;
        ADOQuery1->Parameters->ParamByName("TotalAmount")->Value =
            Edit4->Text.ToInt();
        ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM Orders");

        // Выполняем запрос
        ADOQuery1->Open();

        // Очистка полей
        Edit3->Clear();
        Edit5->Clear();
        ComboBox2->Text = "";
        Edit4->Clear();
    }

    catch (Exception &E)
    {
        ShowMessage("Ошибка: " + E.Message);
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

