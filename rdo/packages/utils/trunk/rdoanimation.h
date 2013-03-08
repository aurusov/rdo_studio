/*!
  \copyright (c) RDO-Team, 2011
  \file      rdoanimation.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Копнин Андрей (kopninandrey@gmail.com)
  \date      13.06.2009
  \brief     Описание объектов анимации, которые передаются от симулятора к GUI для отрисовки
  \indent    4T
*/

#ifndef _UTILS_RDOANIMATION_H_
#define _UTILS_RDOANIMATION_H_

// ----------------------------------------------------------------------- INCLUDES
#include <vector>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdocommon.h"
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
			FIT_CIRCLE , //!< Окружности
			FIT_ELLIPSE, //!< Эллипс
			FIT_BMP    , //!< Картинка
			FIT_S_BMP  , //!< Смасштабированная картинка
			FIT_ACTIVE   //!< Область активности
		};

		//! \param type - тип элемента
		FrameItem(Type type);

		//! \details Возвращает тип элемента
		Type getType() const;

		rbool operator== (CREF(FrameItem) item) const
		{
			return m_type == item.m_type;
		}

	private:
		Type m_type;
	};

	//! \brief   Цвет
	//! \details Хранит RGB-цвет и признак прозрачности
	STRUCT(Color)
	{
		rbyte m_r;           //!< Красная составляющая цвета
		rbyte m_g;           //!< Зелёная составляющая цвета
		rbyte m_b;           //!< Синяя составляющая цвета
		rbool m_transparent; //!< Признак прозрачности

		//! \details Создаёт прозрачный цвет. Значения других атрибутов неопределены.
		Color();

		//! Конструктор копии
		//! \param color - цвет, на основе которого создаётся новый
		Color(CREF(Color) color);

		//! Создаёт цвет
		//! \param r           - красная составляющая цвета
		//! \param g           - зелёная составляющая цвета
		//! \param b           - синяя составляющая цвета
		//! \param transparent - признак прозрачности
		Color(rbyte r, rbyte g, rbyte b, rbool transparent = false);

		rbool operator== (CREF(Color) color) const
		{
			return m_r == color.m_r && m_g == color.m_g && m_b == color.m_b && m_transparent == color.m_transparent;
		}
	};

	//! \brief   Цвет элемента
	//! \details Используется для хранения цветов переднего плана и фона элемента анимации
	STRUCT(ColoredElement)
	{
		Color m_background; //!< Цвет фона
		Color m_foreground; //!< Цвет переднего плана

		//! Конструктор копии
		//! \param color - цвет, на основе которого создаётся новый
		ColoredElement(CREF(ColoredElement) color);

		//! Создаёт цвет элемента
		//! \param background - цвет фона
		//! \param foreground - цвет переднего плана
		ColoredElement(CREF(Color) background, CREF(Color) foreground);

		rbool operator== (CREF(ColoredElement) element) const
		{
			return m_background == element.m_background && m_foreground == element.m_foreground;
		}
	};

	//! \brief   Точка
	//! \details Начало системы координат - левый верхний угол
	STRUCT(Point)
	{
		double m_x; //!< Координата x
		double m_y; //!< Координата y

		//! Конструктор копии
		//! \param point - точка, на основе которой создаётся новый
		Point(CREF(Point) point);

		//! Создаёт точку
		//! \param x - координата x
		//! \param y - координата y
		Point(double x, double y);

		rbool operator== (CREF(Point) point) const
		{
			return m_x == point.m_x && m_y == point.m_y;
		}
	};

	//! Размер
	STRUCT(Size)
	{
		double m_width;  //!< Ширина
		double m_height; //!< Высота

		//! \details Создаёт объект нулевых размеров
		Size();

		//! Конструктор копии
		//! \param size - размер, на основе которой создаётся новый
		Size(CREF(Size) size);

		//! Создаёт размер
		//! \param width  - ширина
		//! \param height - высота
		Size(double width, double height);

		rbool operator== (CREF(Size) size) const
		{
			return m_width == size.m_width && m_height == size.m_height;
		}
	};

	//! Вписанный в прямоуголник элемент
	STRUCT(BoundedElement)
	{
		Point  m_point; //!< Координата элемента
		Size   m_size;  //!< Размер элемента

		//! Конструктор копии
		//! \param rect - прямоуголник, на основе которой создаётся новый
		BoundedElement(CREF(BoundedElement) rect);

		//! Создаёт вписанный в прямоуголник элемент
		//! \param point - координата элемента
		//! \param size  - размер элемента
		BoundedElement(CREF(Point) point, CREF(Size) size);

		rbool operator== (CREF(BoundedElement) element) const
		{
			return m_point == element.m_point && m_size == element.m_size;
		}
	};

	//! Радиус окружности
	STRUCT(Radius)
	{
		double m_radius; //!< Радиус

		//! Конструктор копии
		//! \param radius - радиус, на основе которой создаётся новый
		Radius(CREF(Radius) radius);

		//! Создаёт радиус
		//! \param radius - значение радиуса
		Radius(double radius);

		rbool operator== (CREF(Radius) radius) const
		{
			return m_radius == radius.m_radius;
		}
	};

//! Объявление нового элемента
#define FRAME_ITEM(A) STRUCT_PARENT_OF(A, FrameItem)

	//! \brief   Текст
	//! \details Используется для реализации команды анимации <tt>text[x, y, ширина, высота, цвет_фона, цвет, [выравнивание] содержимое]</tt>
	FRAME_ITEM(TextElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                        AND IMPLEMENTATION_OF(ColoredElement)
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
		TextElement(CREF(BoundedElement) rect, CREF(ColoredElement) color, CREF(tstring) text, TextAlign align);
	};

	//! \brief   Отрезок
	//! \details Используется для реализации команды анимации <tt>line[x1, y1, x2, y2, цвет]</tt>
	FRAME_ITEM(LineElement)
	{
		Point  m_point1; //!< Начальная точка
		Point  m_point2; //!< Конечная точка
		Color  m_color;  //!< Цвет отрезка

		//! Создаёт отрезок
		//! \param point1 - начальная точка
		//! \param point2 - конечная точка
		//! \param color  - цвет отрезка
		LineElement(CREF(Point) point1, CREF(Point) point2, CREF(Color) color);
	
		rbool operator== (CREF(LineElement) element) const
		{
			return m_point1 == element.m_point1 && m_point2 == element.m_point2 && m_color == element.m_color;
		}
	};

	//! \brief   Треугольник
	//! \details Используется для реализации команды анимации <tt>triang[x1, y1, x2, y2, x3, y3, цвет_фона, цвет]</tt>
	FRAME_ITEM(TriangElement) IS IMPLEMENTATION_OF(ColoredElement)
	{
		Point  m_point1; //!< Первая точка треугольника
		Point  m_point2; //!< Вторая точка треугольника
		Point  m_point3; //!< Третья точка треугольника

		//! Создаёт треугольник
		//! \param point1 - первая точка треугольника
		//! \param point2 - вторая точка треугольника
		//! \param point3 - третья точка треугольника
		//! \param color  - цвет треугольника
		TriangElement(CREF(Point) point1, CREF(Point) point2, CREF(Point) point3, CREF(ColoredElement) color);
	};

	//! \brief   Прямоугольник
	//! \details Используется для реализации команды анимации <tt>rect[x, y, ширина, высота, цвет_фона, цвет]</tt>
	FRAME_ITEM(RectElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                        AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! Создаёт прямоугольник
		//! \param rect  - координаты и размер фигуры
		//! \param color - цвет фигуры
		RectElement(CREF(BoundedElement) rect, CREF(ColoredElement) color);
	};

	//! \brief   Прямоугольник со скруглёнными углами
	//! \details Используется для реализации команды анимации <tt>r_rect[x, y, ширина, высота, цвет_фона, цвет]</tt>
	FRAME_ITEM(RoundRectElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                             AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! Создаёт прямоугольник
		//! \param rect  - координаты и размер фигуры
		//! \param color - цвет фигуры
		RoundRectElement(CREF(BoundedElement) rect, CREF(ColoredElement) color);
	};

	//! \brief   Окружность
	//! \details Используется для реализации команды анимации <tt>circle[x, y, радиус, цвет_фона, цвет]</tt>
	FRAME_ITEM(CircleElement) IS IMPLEMENTATION_OF(ColoredElement)
	{
		Point  m_center; //!< Центр окружности
		Radius m_radius; //!< Радиус окружности

		//! Создаёт окружность
		//! \param center - центр окружности
		//! \param radius - радиус окружности
		//! \param color  - цвет окружности
		CircleElement(CREF(Point) center, CREF(Radius) radius, CREF(ColoredElement) color);
	};

	//! \brief   Эллипс
	//! \details Используется для реализации команды анимации <tt>ellipse[x, y, ширина, высота, цвет_фона, цвет]</tt>
	FRAME_ITEM(EllipseElement) IS  IMPLEMENTATION_OF(BoundedElement)
	                           AND IMPLEMENTATION_OF(ColoredElement)
	{
		//! Создаёт эллипс
		//! \param rect  - координаты и размер фигуры
		//! \param color - цвет фигуры
		EllipseElement(CREF(BoundedElement) rect, CREF(ColoredElement) color);
	};

	//! \brief   Картинка
	//! \details Используется для реализации команды анимации <tt>bitmap[x, y, имя_файла_битовой_карты [, имя_файла_маски]]</tt>
	FRAME_ITEM(BmpElement)
	{
		Point    m_point;     //!< Координата левого верхнего угла картинки
		tstring  m_bmp_name;  //!< Имя файла картинки
		tstring  m_mask_name; //!< Имя файла маски картинки

		//! Создаёт картинку
		//! \param point     - координата левого верхнего угла картинки
		//! \param bmp_name  - имя файла картинки
		//! \param mask_name - имя файла маски картинки, может отсутствовать
		BmpElement(CREF(Point) point, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details Возвращает \b true, если маска указана
		rbool hasMask() const;
	};

	//! \brief   Масштабируемая картинка
	//! \details Используется для реализации команды анимации <tt>s_bmp[x, y, ширина, высота, имя_файла_битовой_карты [, имя_файла_маски]]</tt>
	FRAME_ITEM(ScaledBmpElement) IS IMPLEMENTATION_OF(BoundedElement)
	{
		tstring  m_bmp_name;  //!< Имя файла картинки
		tstring  m_mask_name; //!< Имя файла маски картинки

		//! Создаёт картинку
		//! \param rect      - координаты и размер фигуры
		//! \param bmp_name  - имя файла картинки
		//! \param mask_name - имя файла маски картинки, может отсутствовать
		ScaledBmpElement(CREF(BoundedElement) rect, CREF(tstring) bmp_name, CREF(tstring) mask_name = _T(""));

		//! \details Возвращает \b true, если маска указана
		rbool hasMask() const;
	};

	//! \brief   Активная область
	//! \details Используется для реализации команды анимации <tt>active имя_клавиатурной_операции[x, y, ширина, высота]</tt>
	FRAME_ITEM(ActiveElement) IS IMPLEMENTATION_OF(BoundedElement)
	{
		tstring  m_opr_name; //!< Имя клавиатурной операции

		//! Создаёт активную область
		//! \param rect      - координаты и размер области
		//! \param opr_name  - имя клавиатурной операции
		ActiveElement(CREF(BoundedElement) rect, CREF(tstring) opr_name);
	};

	//! \brief   Пустой элемент
	//! \details Нигде не используется
	FRAME_ITEM(NullElement)
	{
		//! \details Создаёт пустой элемент
		NullElement();
	};

	//! \brief   Фрейм
	//! \details Является контейнером элементов анимации.
	//!          Аналог $Frame, но содержит только те элементы, которые необходимо вывести
	STRUCT(Frame)
	{
		typedef  std::vector<PTR(FrameItem)>  Elements; //!< Тип контейнера элементов анимации

		Color     m_bgColor;     //!< Цвет фона фрейма
		tstring   m_bgImageName; //!< Имя фоновой картинки
		Size      m_size;        //!< Размер фрейма
		Elements  m_elements;    //!< Список элементов анимации

		//! \details Удаляет все элементы
		~Frame();

		//! \details Возвращает \b true, если указана фоновая картинка
		rbool hasBgImage() const;
	};

CLOSE_RDO_ANIMATION_NAMESPACE

#include "utils/rdoanimation.inl"

#endif // _UTILS_RDOANIMATION_H_
