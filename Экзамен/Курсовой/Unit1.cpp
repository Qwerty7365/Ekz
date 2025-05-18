//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOsnMenu *OsnMenu;
//---------------------------------------------------------------------------
__fastcall TOsnMenu::TOsnMenu(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOsnMenu::Button1Click(TObject *Sender)
{
	// Создаем экземпляр второй формы
	TEmployees *formSotr = new TEmployees(this);

	// Показываем вторую форму
	formSotr->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button2Click(TObject *Sender)
{
	// Создаем экземпляр второй формы
	TWarehouse *formSklad = new TWarehouse(this);

	// Показываем вторую форму
	formSklad->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button3Click(TObject *Sender)
{
	// Создаем экземпляр второй формы
	TMenuRes *formMenu = new TMenuRes(this);

	// Показываем вторую форму
	formMenu->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button4Click(TObject *Sender)
{
	// Создаем экземпляр второй формы
	TOrders *formZakazy = new TOrders(this);

	// Показываем вторую форму
	formZakazy->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button5Click(TObject *Sender)
{
	// Создаем экземпляр второй формы
	TSupplies *formPrihod = new TSupplies(this);

	// Показываем вторую форму
	formPrihod->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button6Click(TObject *Sender)
{
	Application->Terminate();
}
//---------------------------------------------------------------------------

