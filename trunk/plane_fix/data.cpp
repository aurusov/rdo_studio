#include "data.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TPlanning *Planning;

__fastcall TPlanning::TPlanning(TComponent* Owner): TDataModule(Owner)
{
}

