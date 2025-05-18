//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1* Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner) : TForm(Owner) {}
//---------------------------------------------------------------------------
int SelectedPartnerId = -1; // Для другой формы
void TForm1::AddPartnerPanel(int id, String type, String name, String dir,
    String phone, int rating, int discount, int top)
{
    // Создаем панель
	TPanel* panel = new TPanel(ScrollBox1);
    panel->Parent = ScrollBox1;
    panel->Width = ScrollBox1->Width - 20;
    panel->Height = 100;
	panel->Top = top;
    panel->Left = 10;
    panel->Caption = "";
    panel->Tag = id; // Сохраняем ID партнера в Tag
    SelectedPartnerId = panel->Tag;
    // Добавляем тип и имя партнера
    TLabel* labelName = new TLabel(panel);
    labelName->Parent = panel;
    labelName->Caption = type + " | " + name;
    labelName->Left = 10;
    labelName->Top = 10;
    labelName->Font->Size = 12;
    // Добавляем Директора
    TLabel* labelDirector = new TLabel(panel);
    labelDirector->Parent = panel;
    labelDirector->Caption = "Директор: " + dir;
    labelDirector->Left = 10;
    labelDirector->Top = 35;
    labelDirector->Font->Size = 10;
    // Добавляем контактные данные
    TLabel* labelPhone = new TLabel(panel);
    labelPhone->Parent = panel;
    labelPhone->Caption = "Телефон: " + phone;
    labelPhone->Left = 10;
    labelPhone->Top = 58;
    labelPhone->Font->Size = 9;
    // Добавляем контактные данные
    TLabel* labelRating = new TLabel(panel);
    labelRating->Parent = panel;
    labelRating->Caption = "Рейтинг: " + IntToStr(rating);
    labelRating->Left = 10;
    labelRating->Top = 76;
    labelRating->Font->Size = 9;
    // Добавляем рейтинг
	TLabel* labelDiscount = new TLabel(panel);
	labelDiscount->Parent = panel;
	labelDiscount->Caption = "Скидка: " + IntToStr(discount) + "%";
	labelDiscount->Left = panel->Width - 80;
	labelDiscount->Top = 10;
	labelDiscount->Font->Size = 9;
	labelDiscount->Alignment = taRightJustify;
	labelDiscount->Anchors << akRight;
	//Добавляем событие OnClick для панели
    panel->OnClick = PanelClick;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PanelClick(TObject* Sender)
{
    // Получаем панель, на которой произошло событие
    TPanel* clickedPanel = static_cast<TPanel*>(Sender);
    // Извлекаем ID из Tag
    SelectedPartnerId = clickedPanel->Tag;
    // Показываем ID выбранного партнера
    ShowMessage("Выбран ID партнера: " + IntToStr(SelectedPartnerId));
	Form4->LoadPartnerData(SelectedPartnerId);
	Form4->SelectedId = SelectedPartnerId;
    Form4->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject* Sender)


// МОЖНО ЗАБИТЬ


{
    try {
    ADOQuery1->SQL->Text = R"(
	IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME = 'Partners' AND COLUMN_NAME = 'Discount')
				BEGIN
				ALTER TABLE partners ADD Discount INT DEFAULT 0
				END)";
	ADOQuery1->ExecSQL(); // Выполняем команду добавления колонки
    } catch (const Exception &e) {
        ShowMessage("Ошибка при добавлении колонки: " + e.Message);
    }
    ADOQuery1->SQL->Text = R"(
	  UPDATE Partners
	  SET Discount = CASE
			  WHEN Total_quantity > 300000 THEN 15
			  WHEN Total_quantity > 50000 THEN 10
			  WHEN Total_quantity > 10000 THEN 5
			  ELSE 0
			 END
	  FROM (
		SELECT ID_partner, SUM(Quantity) AS Total_quantity
		FROM Partners_product
		GROUP BY ID_partner
	  ) AS Partners_product
	  WHERE Partners.ID_partner = Partners_product.ID_partner
	)";
	
	
// До сюда


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
            AddPartnerPanel(id, type, name, dir, phone, rating, discount, currentTop);
            currentTop += 100; // Высота панели
            ADOQuery1->Next();
        }

    } catch (Exception &e) {
        ShowMessage("Ошибка выполнения запроса: " + e.Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject* Sender)
{
    // Создаем экземпляр второй формы
    TForm2* formAdd = new TForm2(this);

    // Показываем вторую форму
    formAdd->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject* Sender)
{
    // Создаем экземпляр второй формы
    TForm3* formAdd = new TForm3(this);

    // Показываем вторую форму
    formAdd->ShowModal();
}
//---------------------------------------------------------------------------

