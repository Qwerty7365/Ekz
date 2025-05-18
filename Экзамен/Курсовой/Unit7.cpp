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
    if (ComboBox1->ItemIndex != -1) // Проверяем, что выбрано поле
    {
        // Получаем выбранное поле
        String selectedField = ComboBox1->Text;
        // Обновляем запрос с сортировкой
        ADOQuery1->Close(); // Закрываем текущий запрос
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add(
            "SELECT * FROM OrderDetails ORDER BY " + selectedField);
        ADOQuery1->Open(); // Открываем обновлённый запрос
    } else {
        ShowMessage("Выберите поле для сортировки!");
    }
}
//---------------------------------------------------------------------------
void __fastcall TOrderDetails::FormCreate(TObject* Sender)
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

void __fastcall TOrderDetails::Edit2Change(TObject* Sender)
{
    // Получаем введенный текст в поле для поиска
    String searchText = Edit2->Text;
    // Если поле пустое, показываем все данные
    if (searchText.IsEmpty()) {
        ADOQuery1->SQL->Text =
            "SELECT * FROM OrderDetails";
        ADOQuery1->Open();
    } else {
        // Строим SQL-запрос с проверкой по всем полям таблицы
        String filterQuery = "SELECT * FROM OrderDetails WHERE ";
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

void __fastcall TOrderDetails::Button2Click(TObject* Sender)
{
    // Закрываем текущий запрос
    ADOQuery1->Close();
    // Очищаем текущий SQL и создаём новый запрос с фильтром
    ADOQuery1->SQL->Clear();
    ADOQuery1->SQL->Add("SELECT * FROM OrderDetails");

    // Выполняем запрос
    ADOQuery1->Open();
}
//---------------------------------------------------------------------------

void __fastcall TOrderDetails::Edit1KeyDown(
    TObject* Sender, WORD &Key, TShiftState Shift)

{
    // Проверяем, была ли нажата клавиша Enter
    if (Key == VK_RETURN) {
        String filterText = Edit1->Text.Trim(); // Получаем текст из поля ввода
        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM OrderDetails");
        if (!filterText.IsEmpty()) { // Если текст не пустой, добавляем фильтр
            try {
                // Преобразуем строку в число (если это возможно)
                Integer quan = StrToInt(filterText);
                // Добавляем условие фильтрации по полю INT
                ADOQuery1->SQL->Add(
					"WHERE OrderID = :Filter");
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

void __fastcall TOrderDetails::FormActivate(TObject* Sender)
{
    try {
        // Настройка SQL-запроса
        ADOQuery2->SQL->Text = "SELECT DishName FROM Menu";
        ADOQuery2->Open();

        // Очистка ComboBox перед добавлением записей
        ComboBox2->Items->Clear();

        // Заполнение ComboBox
        while (!ADOQuery2->Eof) {
            ComboBox2->Items->Add(ADOQuery2->FieldByName("DishName")->AsString);
            ADOQuery2->Next();
        }

        // Закрытие запроса
        ADOQuery2->Close();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TOrderDetails::Button3Click(TObject* Sender)
{
    try {
        // Проверка на заполненность полей
        if (Edit3->Text.IsEmpty() || ComboBox2->Text.IsEmpty() ||
            Edit4->Text.IsEmpty()) {
            ShowMessage("Все поля должны быть заполнены!");
            return;
        }

        // Добавление данных в таблицу
        String sql =
            "INSERT INTO OrderDetails (OrderID, DishName, Quantity) VALUES (:OrderID, :DishName, :Quantity)";
        ADOQuery1->SQL->Text = sql;
        ADOQuery1->Parameters->ParamByName("OrderID")->Value = Edit3->Text;
        ADOQuery1->Parameters->ParamByName("DishName")->Value = ComboBox2->Text;
        ADOQuery1->Parameters->ParamByName("Quantity")->Value =
            Edit4->Text.ToInt();
        ADOQuery1->ExecSQL();

        // Сообщение об успешной вставке
        ShowMessage("Данные успешно добавлены!");

        // Закрываем текущий запрос
        ADOQuery1->Close();
        // Очищаем текущий SQL и создаём новый запрос с фильтром
        ADOQuery1->SQL->Clear();
        ADOQuery1->SQL->Add("SELECT * FROM OrderDetails");

        // Выполняем запрос
        ADOQuery1->Open();

        // Очистка полей
        Edit3->Clear();
        ComboBox2->Text = "";
        Edit4->Clear();
    } catch (Exception &E) {
        ShowMessage("Ошибка: " + E.Message);
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

