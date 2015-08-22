// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include "utils/src/common/warning_disable.h"
#include <sstream>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/plugins/game5/src/plugin_game5_model_generator.h"
// --------------------------------------------------------------------------------

QString PluginGame5ModelGenerator::modelRTP(const Board& gameBoard)
{
    std::stringstream RTPtabTextStream;
    RTPtabTextStream
        << "$Resource_type Фишка : permanent" << std::endl
        << "$Parameters" << std::endl
        << "    Номер          : integer[1.." << gameBoard.getQuantityOfTiles()     << "]" << std::endl
        << "    Местоположение : integer[1.." << gameBoard.getQuantityOfTiles() + 1 << "]" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Resource_type Дырка_t : permanent" << std::endl
        << "$Parameters" << std::endl
<<<<<<< HEAD
        << "    Место: integer[1.." << gameBoard.getQuantityOfTiles() + 1 << "]" << std::endl
=======
        << "\t    Место: integer[1.." << gameBoard.getQuantityOfTiles() + 1 << "]" << std::endl
>>>>>>> drobus/dev
        << "$End" << std::endl;
    return QString::fromStdString(RTPtabTextStream.str());
}

QString PluginGame5ModelGenerator::modelRSS(const Board& gameBoard)
{
    std::stringstream RSStabTextStream;
    RSStabTextStream << "$Resources" << std::endl;
    for (int i = 1; i < gameBoard.getQuantityOfTiles() + 1; i++)
    {
        RSStabTextStream << "\tФишка" << i <<" = Фишка(" << i << ", " << gameBoard.getTilePosition(i) << ");" << std::endl;
    }
    RSStabTextStream << "\tДырка = Дырка_t(" << gameBoard.getHolePosition() << ");" << std::endl;
    RSStabTextStream << "$End" << std::endl;
    return QString::fromStdString(RSStabTextStream.str());
}

QString PluginGame5ModelGenerator::modelPAT()
{
    std::stringstream PATtabTextStream;
    PATtabTextStream
        << "$Pattern Перемещение_фишки : rule" << std::endl
        << "$Parameters" << std::endl
        << "    Куда_перемещать: such_as Место_дырки" << std::endl
        << "    На_сколько_перемещать: integer" << std::endl
        << "$Relevant_resources" << std::endl
        << "    _Фишка: Фишка   Keep" << std::endl
        << "    _Дырка: Дырка_t Keep" << std::endl
        << "$Body" << std::endl
        << "    _Фишка:" << std::endl
        << "        Choice from Где_дырка(_Фишка.Местоположение) == Куда_перемещать" << std::endl
        << "        first" << std::endl
        << "            Convert_rule Местоположение = _Фишка.Местоположение + На_сколько_перемещать;" << std::endl
        << "    _Дырка:" << std::endl
        << "        Choice NoCheck" << std::endl
        << "        first" << std::endl
        << "            Convert_rule Место = _Дырка.Место - На_сколько_перемещать;" << std::endl
        << "$End" << std::endl;
    return QString::fromStdString(PATtabTextStream.str());
}

QString PluginGame5ModelGenerator::modelDPT(const Board& gameBoard, const std::string& evaluateBy, bool compareTops,
                                            const std::string& activityValueLeft, const std::string& activityValueRight,
                                            const std::string& activityValueDown, const std::string& activityValueUp)
{
    std::stringstream DPTtabTextStream;
    DPTtabTextStream
        << "$Decision_point Расстановка_фишек : search trace_all" << std::endl
        << "$Condition Exist(Фишка: Фишка.Номер <> Фишка.Местоположение)" << std::endl
        << "$Term_condition" << std::endl
        << "    For_All(Фишка: Фишка.Номер == Фишка.Местоположение)" << std::endl
        << "$Evaluate_by " << evaluateBy << std::endl
        << "$Compare_tops = " << (compareTops ? "YES" : "NO") << std::endl
        << "$Activities" << std::endl
<<<<<<< HEAD
        << "    Перемещение_вправо: Перемещение_фишки (справа,  1) value " << activityValueLeft << ";" << std::endl
        << "    Перемещение_влево : Перемещение_фишки (слева,  -1) value " << activityValueRight << ";" << std::endl
        << "    Перемещение_вверх : Перемещение_фишки (сверху, -" << gameBoard.m_tilesCountX << ") value " << activityValueDown << ";" << std::endl
        << "    Перемещение_вниз  : Перемещение_фишки (снизу,   " << gameBoard.m_tilesCountX << ") value " << activityValueUp << ";" << std::endl
=======
        << "    Перемещение_вправо: Перемещение_фишки справа  1 value " << activityValueLeft << std::endl
        << "    Перемещение_влево : Перемещение_фишки слева  -1 value " << activityValueRight << std::endl
        << "    Перемещение_вверх : Перемещение_фишки сверху -" << gameBoard.m_tilesCountX << " value " << activityValueDown << std::endl
        << "    Перемещение_вниз  : Перемещение_фишки снизу   " << gameBoard.m_tilesCountX << " value " << activityValueUp << std::endl
>>>>>>> drobus/dev
        << "$End";
    return QString::fromStdString(DPTtabTextStream.str());
}

QString PluginGame5ModelGenerator::modelFUN(const Board& gameBoard)
{
    std::stringstream FUNtabTextStream;
    FUNtabTextStream
        << "$Constant" << std::endl
        << "    Место_дырки: (справа, слева, сверху, снизу, дырки_рядом_нет) = дырки_рядом_нет" << std::endl
        << "    Длина_поля : integer = " << gameBoard.m_tilesCountX << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Ряд: integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "    Местоположение: integer" << std::endl
        << "$Body" << std::endl
        << "    return (Местоположение - 1)/Длина_поля + 1;" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Остаток_от_деления : integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "    Делимое   : integer" << std::endl
        << "    Делитель  : integer" << std::endl
        << "$Body" << std::endl
        << "    integer Целая_часть  = Делимое/Делитель;" << std::endl
        << "    integer Макс_делимое = Делитель * int(Целая_часть);" << std::endl
        << "    return Делимое -  Макс_делимое;" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Столбец: integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "    Местоположение: integer" << std::endl
        << "$Body" << std::endl
        << "    return Остаток_от_деления(Местоположение - 1,Длина_поля) + 1;" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Где_дырка : such_as Место_дырки" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "    _Место: such_as Фишка.Местоположение" << std::endl
        << "$Body" << std::endl
        << "    if (Столбец(_Место) == Столбец(Дырка.Место) and Ряд(_Место) == Ряд(Дырка.Место)+ 1) return сверху;" << std::endl
        << "    if (Столбец(_Место) == Столбец(Дырка.Место) and Ряд(_Место) == Ряд(Дырка.Место)- 1) return снизу;" << std::endl
        << "    if (Ряд(_Место) == Ряд(Дырка.Место) and Столбец(_Место) == Столбец(Дырка.Место)- 1) return справа;" << std::endl
        << "    if (Ряд(_Место) == Ряд(Дырка.Место) and Столбец(_Место) == Столбец(Дырка.Место)+ 1) return слева;" << std::endl
        << "    return дырки_рядом_нет;" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Фишка_на_месте : integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "    _Номер: such_as Фишка.Номер" << std::endl
        << "    _Место: such_as Фишка.Местоположение" << std::endl
        << "$Body" << std::endl
        << "    if (_Номер == _Место) return 1;" << std::endl
        << "    else                  return 0;" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Кол_во_фишек_не_на_месте : integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "$Body" << std::endl
        << "    return " << gameBoard.getQuantityOfTiles() << " - (Фишка_на_месте(Фишка1.Номер, Фишка1.Местоположение)+" << std::endl;
    for (int i = 2; i < gameBoard.getQuantityOfTiles(); i++)
    {
        FUNtabTextStream
            << "                Фишка_на_месте(Фишка" << i <<".Номер, Фишка" << i <<".Местоположение)+" << std::endl;
    }
    FUNtabTextStream
        << "                Фишка_на_месте(Фишка" << gameBoard.getQuantityOfTiles() <<".Номер, Фишка" << gameBoard.getQuantityOfTiles() <<".Местоположение));" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Расстояние_фишки_до_места : integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "    Откуда: integer" << std::endl
        << "    Куда  : integer" << std::endl
        << "$Body" << std::endl
        << "    return Abs(Ряд(Откуда)-Ряд(Куда)) + Abs(Столбец(Откуда)-Столбец(Куда));" << std::endl
        << "$End" << std::endl
        << std::endl
        << "$Function Расстояния_фишек_до_мест : integer" << std::endl
        << "$Type = algorithmic" << std::endl
        << "$Parameters" << std::endl
        << "$Body" << std::endl
        << "    return Расстояние_фишки_до_места(Фишка1.Номер, Фишка1.Местоположение)+" << std::endl;
    for (int i = 2; i < gameBoard.getQuantityOfTiles(); i++)
    {
        FUNtabTextStream
            << "           Расстояние_фишки_до_места(Фишка" << i << ".Номер, Фишка" << i << ".Местоположение)+" << std::endl;
    }
    FUNtabTextStream
        << "           Расстояние_фишки_до_места(Фишка" << gameBoard.getQuantityOfTiles() << ".Номер, Фишка" << gameBoard.getQuantityOfTiles() << ".Местоположение);" << std::endl
        << "$End";
    return QString::fromStdString(FUNtabTextStream.str());
}
