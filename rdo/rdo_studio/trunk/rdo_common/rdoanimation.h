/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoanimation.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      13.06.2009
  \brief     Описание объектов анимации, которые передаются от симулятора к GUI для отрисовки
  \indent    4T
*/

#ifndef _RDOANIMATION_H_
#define _RDOANIMATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "rdo_common/namespace.h"
#include "rdo_common/rdotypes.h"
#include "rdo_common/rdomacros.h"
// --------------------------------------------------------------------------------

OPEN_RDO_ANIMATION_NAMESPACE

	//! Базовый класс элементов анимации
	STRUCT(FrameItem)
	{
		//! \details Тип элемента
		enum Type
		{
			FIT_NULL   , //!< Пустой
			FIT_TEXT   , //!< Текст
			FIT_LINE   , //!< Линия
			FIT_TRIANG , //!< Треугольник
			FIT_RECT   , //!< Прямоугольник
			FIT_R_RECT , //!< Прямоугольник со скруглёнными краями
			FIT_ELLIPSE, //!< Эллипс
			FIT_BMP    , //!< Картинка
			FIT_S_BMP  , //!< Смасштабированная картинка
			FIT_ACTIVE   //!< Область активности
		};

		//! \param type - тип элемента
		FrameItem(Type type);

		//! \details Возвращает тип элемента
		Type getType() const;

	private:
		Type m_type;
	};

	//! \brief   Цвет
	//! \details Хранит RGB-цвет и признак прозрачности
	STRUCT(RDOColor)
	{
		rbyte m_r;           //!< Красная составляющая цвета
		rbyte m_g;           //!< Зелёная составляющая цвета
		rbyte m_b;           //!< Синяя составляющая цвета
		rbool m_transparent; //!< Признак прозрачности

		//! \details Создаёт прозрачный цвет. Значения других атрибутов неопределены.
		RDOColor();

		//! Конструктор копии
		//! \param color - цвет, на основе которого создаётся новый
		RDOColor(CREF(RDOColor) color);

		//! Создаёт цвет
		//! \param r           - красная составляющая цвета
		//! \param g           - зелёная составляющая цвета
		//! \param b           - синяя составляющая цвета
		//! \param transparent - признак прозрачности
		RDOColor(rbyte r, rbyte g, rbyte b, rbool transparent = false);
	};

	//! \brief   Цвет элемента
	//! \details Используется для хранения цветов переднего плана и фона элемента анимации
	STRUCT(RDOColoredElement)
	{
		RDOColor m_background; //!< Цвет фона
		RDOColor m_foreground; //!< Цвет переднего плана

		//! Конструктор копии
		//! \param color - цвет, на основе которого создаётся новый
		RDOColoredElement(CREF(RDOColoredElement) color);

		//! Создаёт цвет элемента
		//! \param background - цвет фона
		//! \param foreground - цвет переднего плана
		RDOColoredElement(CREF(RDOColor) background, CREF(RDOColor) foreground);
	};

	//! \brief   Точка
	//! \details Начало системы координат - левый верхний угол
	STRUCT(RDOPoint)
	{
		double m_x; //!< Координата x
		double m_y; //!< Координата y

		//! Конструктор копии
		//! \param point - точка, на основе которой создаётся новый
		RDOPoint(CREF(RDOPoint) point);

		//! Создаёт точку
		//! \param x - координата x
		//! \param y - координата y
		RDOPoint(double x, double y);
	};

	//! Размер
	STRUCT(RDOSize)
	{
		double m_width;  //!< Ширина
		double m_height; //!< Высота

		//! \details Создаёт объект нулевых размеров
		RDOSize();

		//! Конструктор копии
		//! \param size - размер, на основе которой создаётся новый
		RDOSize(CREF(RDOSize) size);

		//! Создаёт размер
		//! \param width  - ширина
		//! \param height - высота
		RDOSize(double width, double height);
	};

	//! Вписанный в прямоуголник элемент
	STRUCT(RDOBoundedElement)
	{
		RDOPoint  m_point; //!< Координата элемента
		RDOSize   m_size;  //!< Размер элемента

		//! Конструктор копии
		//! \param rect - прямоуголник, на основе которой создаётся новый
		RDOBoundedElement(CREF(RDOBoundedElement) rect);

		//! Создаёт вписанный в прямоуголник элемент
		//! \param point - координата элемента
		//! \param size  - размер элемента
		RDOBoundedElement(CREF(RDOPoint) point, CREF(RDOSize) size);
	};

//! Объявление нового элемента
#define FRAME_ITEM(A) STRUCT_PARENT_OF(A, FrameItem)

	//! \brief   Текст
	//! \details Используется для реализации команды анимации <tt>text[x, y, ширина, высота, цвет_фона, цвет, [выравнивание] содержимое]</tt>
	FRAME_ITEM(RDOTextElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                           AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! \details Тип выравнивания текста
		enum TextAlign
		{
			TETA_LEFT,   //!< По левому краю
			TETA_RIGHT,  //!< По правому краю
			TETA_CENTER  //!< По центру
		};

		tstring    m_text;  //!< Строка текста
		TextAlign  m_align; //!< Тип выравнивания

		//! Создаёт объект текста
		//! \param rect  - область вывода
		//! \param color - цвет текста
		//! \param text  - строка
		//! \param align - выравнивание
		RDOTextElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color, CREF(tstring) text, TextAlign align);
	};

	//! \brief   Отрезок
	//! \details Используется для реализации команды анимации <tt>line[x1, y1, x2, y2, цвет]</tt>
	FRAME_ITEM(RDOLineElement)
	{
		RDOPoint  m_point1; //!< Начальная точка
		RDOPoint  m_point2; //!< Конечная точка
		RDOColor  m_color;  //!< Цвет отрезка

		//! Создаёт отрезок
		//! \param point1 - начальная точка
		//! \param point2 - конечная точка
		//! \param color  - цвет отрезка
		RDOLineElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOColor) color);
	};

	//! \brief   Треугольник
	//! \details Используется для реализации команды анимации <tt>triang[x1, y1, x2, y2, x3, y3, цвет_фона, цвет]</tt>
	FRAME_ITEM(RDOTriangElement) IS IMPLEMENTATION_OF(RDOColoredElement)
	{
		RDOPoint  m_point1; //!< Первая точка треугольника
		RDOPoint  m_point2; //!< Вторая точка треугольника
		RDOPoint  m_point3; //!< Третья точка треугольника

		//! Создаёт треугольник
		//! \param point1 - первая точка треугольника
		//! \param point2 - вторая точка треугольника
		//! \param point3 - третья точка треугольника
		//! \param color  - цвет треугольника
		RDOTriangElement(CREF(RDOPoint) point1, CREF(RDOPoint) point2, CREF(RDOPoint) point3, CREF(RDOColoredElement) color);
	};

	//! \brief   Прямоугольник
	//! \details Используется для реализации команды анимации <tt>rect[x, y, ширина, высота, цвет_фона, цвет]</tt>
	FRAME_ITEM(RDORectElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                           AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! Создаёт прямоугольник
		//! \param rect  - координаты и размер фигуры
		//! \param color - цвет фигуры
		RDORectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	//! \brief   Прямоугольник со скруглёнными углами
	//! \details Используется для реализации команды анимации <tt>r_rect[x, y, ширина, высота, цвет_фона, цвет]</tt>
	FRAME_ITEM(RDORRectElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                            AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! Создаёт прямоугольник
		//! \param rect  - координаты и размер фигуры
		//! \param color - цвет фигуры
		RDORRectElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	//! \brief   Эллипс
	//! \details Используется для реализации команды анимации <tt>ellipse[x, y, ширина, высота, цвет_фона, цвет]</tt>
	FRAME_ITEM(RDOEllipseElement) IS  IMPLEMENTATION_OF(RDOBoundedElement)
	                              AND IMPLEMENTATION_OF(RDOColoredElement)
	{
		//! Создаёт эллипс
		//! \param rect  - координаты и размер фигуры
		//! \param color - цвет фигуры
		RDOEllipseElement(CREF(RDOBoundedElement) rect, CREF(RDOColoredElement) color);
	};

	//! \brief   Картинка
	//! \details Используется для реализации команды анимации <tt>bitmap[x, y, имя_файла_битовой_карты [, имя_файла_маски]]</tt>
	FRAME_ITEM(RDOBmpElement)
	{
		RDOPoint  m_point;     //!< Координата левого верхнего угла картинки
		tstring   m_bmp_name;  //!< Имя файла картинки
		tstring   m_mask_name; //!< Имя файла маски картинки

		//! Создаёт картинку
		//! \param point     - координата левого верхнего угла картинки
		//! \param bmp_name  - имя файла картинки
		//! \param mask_name - имя файла маски картинки, может отсутствовать
		RDOBmpElement(CREF(RDOPoint) point, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details Возвращает \b true, если маска указана
		rbool hasMask() const;
	};

	//! \brief   Масштабируемая картинка
	//! \details Используется для реализации команды анимации <tt>s_bmp[x, y, ширина, высота, имя_файла_битовой_карты [, имя_файла_маски]]</tt>
	FRAME_ITEM(RDOSBmpElement) IS IMPLEMENTATION_OF(RDOBoundedElement)
	{
		tstring  m_bmp_name;  //!< Имя файла картинки
		tstring  m_mask_name; //!< Имя файла маски картинки

		//! Создаёт картинку
		//! \param rect      - координаты и размер фигуры
		//! \param bmp_name  - имя файла картинки
		//! \param mask_name - имя файла маски картинки, может отсутствовать
		RDOSBmpElement(CREF(RDOBoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details Возвращает \b true, если маска указана
		rbool hasMask() const;
	};

	//! \brief   Активная область
	//! \details Используется для реализации команды анимации <tt>active имя_клавиатурной_операции[x, y, ширина, высота]</tt>
	FRAME_ITEM(RDOActiveElement) IS IMPLEMENTATION_OF(RDOBoundedElement)
	{
		tstring  m_opr_name; //!< Имя клавиатурной операции

		//! Создаёт активную область
		//! \param rect      - координаты и размер области
		//! \param opr_name  - имя клавиатурной операции
		RDOActiveElement(CREF(RDOBoundedElement) rect, CREF(tstring) opr_name);
	};

	//! \brief   Пустой элемент
	//! \details Нигде не используется
	FRAME_ITEM(RDONullElement)
	{
		//! \details Создаёт пустой элемент
		RDONullElement();
	};

	//! \brief   Фрейм
	//! \details Является контейнером элементов анимации.
	//!          Аналог $Frame, но содержит только те элементы, которые необходимо вывести, т.е. Show_if уже расчитаны.
	STRUCT(RDOFrame)
	{
		typedef  std::vector<PTR(FrameItem)>  Elements; //!< Тип контейнера элементов анимации

		RDOColor  m_bgColor;     //!< Цвет фона фрейма
		tstring   m_bgImageName; //!< Имя фоновой картинки
		RDOSize   m_size;        //!< Размер фрейма
		Elements  m_elements;    //!< Список элементов анимации


		//! \details Удаляет все элементы
		~RDOFrame();

		//! \details Возвращает \b true, если указана фоновая картинка
		rbool hasBgImage() const;
	};

CLOSE_RDO_ANIMATION_NAMESPACE

#include "rdo_common/rdoanimation.inl"

#endif //! _RDOANIMATION_H_
