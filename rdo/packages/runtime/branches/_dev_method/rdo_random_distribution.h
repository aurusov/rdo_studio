/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_random_distribution.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.04.2008
  \brief     Датчики псевдослучайных чисел
  \indent    4T
*/

#ifndef _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
#define _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <math.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdo_value.h"
// --------------------------------------------------------------------------------

OPEN_RDO_RUNTIME_NAMESPACE

//! Законы распределения псевдослучайных чисел в стиле РДО
#define RDO_NATIVE_U01

//! Базовое равномерное распределение [0, 1)
class RandGenerator
{
public:
	//! Удалять нельзя, нужен именно виртуальный деструктор для вызова из \ref RDOCalcSeqInit::~RDOCalcSeqInit()
	virtual ~RandGenerator();

	//! Установка базы генератора
	//! @param seed - База генератора
	void setSeed(long int seed = 123456789);

	//! @brief  Генерация псевдо-случайного числа на интевале [0, 1) по равномерному закону распределения
	/*! @note   База все время растет и переполняет память, лишнее отбрасывается.
		         69069 - то число, благодаря которому получается все гладко и красиво (шаманство).
		         +1    - Благодаря этому можно выбраться из ноля.
		         Функция возвращает (long double)UINT_MAX + 1*/
	//! @result Псевдо-случайное число, значение в дипазоне [0, 1)
	double u01();

protected:

#ifdef RDO_NATIVE_U01
	//! @param  m_seed  База генератора
	ruint m_seed;
#else
	long int m_seed;
#endif
	RandGenerator(long int seed = 123456789);
};

//! Равномерное распределение [0, 1)
class RandGeneratorUniform: public RandGenerator
{
public:
	//! @param   seed   База генератора псевдослучайных чисел
	RandGeneratorUniform(long int seed = 123456789);

	//! @brief          Генерация чисел из последовательности по Равномерному закону
	//! @param   from   Нижняя граница диапазона распределения
	//! @param   to     Верхняя граница диапазона распределения
	//! @note           Базовое равномерное распределение масштабируется домножением на ширину диапазона и смещается прибавлением левой границы
	//! @return         Число из диапазона распределения
	double next(double from, double to);
};

//! Экспоненциальное распределение
class RandGeneratorExponential: public RandGenerator
{
public:
	//! @param   seed   База генератора псевдослучайных чисел
	RandGeneratorExponential(long int seed = 123456789);

	//! @brief          Генерация чисел из последовательности по Эспоненциальному закону
	//! @param   math   Математическое ожидание закона распределения
	//! @result         Число от 0 до бесконечности
	double next(double math);
};

//! Нормальное распределение
class RandGeneratorNormal: public RandGenerator
{
public:
	//! @param   seed   База генератора псевдослучайных чисел
	RandGeneratorNormal(long int seed = 123456789);

	//! @brief          Генерация чисел из последовательности по Нормальному закону
	//! @param   av     Математическое ожидание последовательности
	//! @param   var    Среднеквадратическое отклонение
	//! @note           Необъяснимые телодвижения, продиктованные опытом поколений (взято из литературы по программированию)
	//! @result         Число от минус бесконечноти до бесконечности. Но по факту чаще всего число из диапазона av+-3*var
	double next(double av, double var);
};

//! Треугольное распределение
class RandGeneratorTriangular: public RandGenerator
{
public:
	//! @param   seed   База генератора псевдослучайных чисел
	RandGeneratorTriangular(long int seed = 123456789);

	//! @brief         Генерация чисел из последовательности по Треугольному закону
	//! @param   from  Левая граница интервала распределения
	//! @param   top   Точка под вершиной треугольника
	//! @param   to    Правая граница интервала распределения
	/*! @note          Все вычисления производятся относительно точки под вершиной треугольника.
	                   Весь диапазон разбит на две части: слева и справа от вершины треугольника.
	                   С помощью Базового равномерного распределения определяется, в которой из частей будет лежать сгенерированное число.
	                   Далее через обратную функцию от Базового равномерного распределения генерируется число.
	                   Потом оно сдвигается (-1), масштабируется (домножением). Возвращается число, сдвинутое на расстояние от начала координат до вершины треугольника*/
	//! result         Число из диапазона распределения
	double next(double from, double top, double to);
};

//! Общий класс для by_hist и enumerative
class RandGeneratorCommonNext
{
public:
	virtual ~RandGeneratorCommonNext();
	virtual RDOValue next() = 0;

protected:
	RandGeneratorCommonNext();
};

//! Базовый класс для гистограмм
class RandGeneratorByHist: public RandGeneratorUniform, public RandGeneratorCommonNext
{
protected:
	double summ;
	//! @param  seed  База генератора чисел
	RandGeneratorByHist(long int seed = 123456789);
};

//! Гистограмма вещественных чисел
class RandGeneratorByHistReal: public RandGeneratorByHist
{
public:
	//! @param   seed   База генератора чисел
	RandGeneratorByHistReal(long int seed = 123456789);

	//! @brief        Добавление элемента в последовательность by_hist
	//! @param  from  Нижняя граница диапазона
	//! @param  to    Верхняя граница диапазона
	//! @param  freq  Относительная частота попадания последовательности в заданный интервал
	//! @note         Добавление чисел в контейнер типа vector. Суммирование относительных частот
	void addValues(double from, double to, double freq);

private:
	std::vector< double > m_from;
	std::vector< double > m_to;
	std::vector< double > m_freq;

	//! @brief   Генерация элемента последовательности by_hist (Real)
	/*! @note    Генерируется число по равномерному закону от 0 до суммы всех частот.
	             Определяется номер элемента, при достижении которого сумма всех предыдущих частот и текущего элемента превысит сгенерированное число
	             Возвращается число, полученное по равномерному закону из диапазона с выше полученный номером*/
	//! @return  Значение от самой левой из всех границ до самой правой.
	virtual RDOValue next();
};

//! Гистограмма перечислимого типа
class RandGeneratorByHistEnum: public RandGeneratorByHist
{
public:
	//! @param   seed   База генератора чисел
	RandGeneratorByHistEnum(long int seed = 123456789);

	//! @brief        Добавление элемента в последовательность by_hist
	//! @param  val   Имя перечислимого типа
	//! @param  freq  Относительная частота попадания последовательности в заданный интервал
	//! @note         Добавление чисел в контейнер типа vector. Суммирование относительных частот
	void addValues(RDOValue val, double freq);

private:
	std::vector< RDOValue > m_vals;
	std::vector< double >   m_freq;

	//! @brief   Генерация элемента последовательности by_hist (Enum)
	/*! @note    Генерируется число по равномерному закону от 0 до суммы всех частот.
	             Определяется номер элемента, при достижении которого сумма всех предыдущих частот и текущего элемента превысит сгенерированное число
	             Возвращается число, соответсвующее выше полученному номеру.*/
	//! @result  Значение от самой левой из всех границ до самой правой.
	virtual RDOValue next();
};

//! Перечень значений
class RandGeneratorEnumerative: public RandGeneratorCommonNext
{
public:
	RandGeneratorEnumerative();
	virtual ~RandGeneratorEnumerative();

	//! @brief       Добавление элемента в последовательность enumerative
	//! @param  val  Имя вещественной константы или Имя значения в соответсвии с типом значений последовательности
	//! @note        Добавление значения в контейнер типа vector
	void addValue(RDOValue val);

private:
	std::vector<RDOValue>            m_vals;
	std::vector<RDOValue>::size_type m_curr;

	//! @brief   Получение элемента из множества элементов Enum
	//! @note    Возвращает очередное значение из множества определенных элементов. При окончании списка - возврат в начало
	//! @return  Одно из перечисленных значений
	virtual RDOValue next();
};

CLOSE_RDO_RUNTIME_NAMESPACE

#include "simulator/runtime/rdo_random_distribution.inl"

#endif // _LIB_RUNTIME_RANDOM_DISTRIBUTION_H_
