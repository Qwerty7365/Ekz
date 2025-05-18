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
	// ������� ��������� ������ �����
	TEmployees *formSotr = new TEmployees(this);

	// ���������� ������ �����
	formSotr->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button2Click(TObject *Sender)
{
	// ������� ��������� ������ �����
	TWarehouse *formSklad = new TWarehouse(this);

	// ���������� ������ �����
	formSklad->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button3Click(TObject *Sender)
{
	// ������� ��������� ������ �����
	TMenuRes *formMenu = new TMenuRes(this);

	// ���������� ������ �����
	formMenu->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button4Click(TObject *Sender)
{
	// ������� ��������� ������ �����
	TOrders *formZakazy = new TOrders(this);

	// ���������� ������ �����
	formZakazy->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button5Click(TObject *Sender)
{
	// ������� ��������� ������ �����
	TSupplies *formPrihod = new TSupplies(this);

	// ���������� ������ �����
	formPrihod->Show();
}
//---------------------------------------------------------------------------

void __fastcall TOsnMenu::Button6Click(TObject *Sender)
{
	Application->Terminate();
}
//---------------------------------------------------------------------------

