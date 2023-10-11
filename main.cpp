#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>                                      
#define PI 3.14159265  
#include <iostream>
#include <time.h>
#include <sstream>

using namespace sf;

std::ostringstream score_string; // текстовая переменная для количества очков
std::ostringstream level_string; // текстовая переменная для номера уровня

int deg = 0, // угол поворота шприца в градусах
	d = 0,   // величина смещения шприца
	score = 0, // количество очков
	score_from_begin_level = 0, // количество набранных очков с начала уровня
	level = 1, // уровень
	//i = 0, // счётчик попаданий
	num_for_sound_check = 1, // чтобы звук попадания был 1 раз
	num_for_sound_start = 1, // чтобы звук старта был 1 раз
	num_for_sound_off = 1; // чтобы звук пропуска был 1 раз

float k = 0.1, h = 0.1, // масштабы "проиграл" и "в яблочко"
	  speed_ass = 4; // скорость попы

double  rad = 0, // угол поворота шприца в радианах
		dx = 0,  // коэффициент смещения шприца по пси Х
		dy = 0,  // коэффициент смещения шприца по пси У
		x_ass = 400, // координата попы Х
		y_ass = 80,  // координата попы У
		x_arrow = 0, // координата шприца Х 
		y_arrow = 0, // координата шприца У
		scale = 1,   // масштаб шприца
		x2 = 0,      // координата конца иглы Х
		y2 = 0,		 // координата конца иглы У
		timer = 0,    // метка времени
		delay = 0.01;  // задержка для масштабирования яблочка

bool start = false, // старт шприца
	 check = false; // попадание в цель

const int def = 40; // точность для попадания ( +/- )
const int speed_arrow = 17; // скорость шприца
const float scale_factor = 0.1; // величина изменения масштаба в яблочко
const int score_add = 100; // столько очков добавляем при попадании
const float speed_ass_add = 1.1; // во столько раз увиличиваем скорость попы при попадании
//const int num_for_add_level = 3; // столько раз надо попасть, чтобы повысился уровень
const float speed_ass_ini = 4; // начальная скорость попы
const int score_next = 300; // столько очков надо набрать для перехода на следущий уровень

Clock cl;

int main()
{
	Font font;
	font.loadFromFile("font\\CyrilicOld.TTF");
	Text score_text("", font, 25);
	Text level_text("", font, 25);
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(800, 600), "Doctor Ass", Style::Default, settings);
	
	// загружаем звуки
	SoundBuffer buffer, buffer_start, buffer_off;

	Sound check_sound;
	Sound start_sound;
	Sound off_sound;

	score_string << "0";	level_string << "1";

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			// Пользователь нажал на «крестик» и хочет закрыть окно?
			if (event.type == Event::Closed) window.close();
			// Поворот шприца (если он не летит)
			if (event.key.code == Keyboard::Right && (!start)) deg += 5;
			if (event.key.code == Keyboard::Left && (!start)) deg -= 5;
			// запускаем шприц
			if (event.key.code == Keyboard::Space)
			{
				if (num_for_sound_start == 1)
				{
					buffer_start.loadFromFile("sound\\start.ogg");
					start_sound.setBuffer(buffer_start);
					start_sound.play();
				}
				num_for_sound_start++;
				start = true;
			}
		}

		window.clear(Color::White);

		// текстура шприц
		Texture t_arrow;
		t_arrow.loadFromFile("textures\\shpritz.png");
		Sprite s_arrow(t_arrow);
		t_arrow.setSmooth(true);

		// текстура попа
		Texture t_ass;
		t_ass.loadFromFile("textures\\ass.png");
		Sprite s_ass(t_ass);
		//t_ass.setSmooth(true);
		s_ass.setOrigin(65.f, 70.f);

		// текстура попа с кровью
		Texture t_ass_blood;
		t_ass_blood.loadFromFile("textures\\ass_blood.png");
		Sprite s_ass_blood(t_ass_blood);
		t_ass_blood.setSmooth(true);
		s_ass_blood.setOrigin(65.f, 70.f);

		// текстура в яблочко
		Texture t_apple;
		t_apple.loadFromFile("textures\\apple.jpg");
		Sprite s_apple(t_apple);
		s_apple.setOrigin(150.f, 110.f);

		// текстура ПРОИГРАЛ
		Texture t_over;
		t_over.loadFromFile("textures\\over.png");
		Sprite s_over(t_over);
		s_over.setOrigin(124.f, 90.f);

		rad = (PI * deg) / 180; // переводим градусы в радианы

		// коэффициент смещения по осям (зависит от угла наклона)
		dx = sin(rad);
		dy = cos(rad);

		if (start)	// запускаем шприц при нажатии на пробел
		{
			d += speed_arrow;	//величина смещения шприца
			if (!check) scale -= 0.01;	//изменение размера шприца
		}

		if (x_ass > 720 || x_ass < 60) speed_ass = -speed_ass;	// если попа доехала до конца, разворачиваем её

		// проверка попадания		
		if (x_arrow > (x_ass - def) && x_arrow < (x_ass + def) && y_arrow >(y_ass - def) && y_arrow < (y_ass + def) && start) check = true;

		if (!check)	// если попали - шприц и попа остаются на месте
		{
			x_ass += speed_ass;	// смещаем попу
			//координаты шприца
			x_arrow = 400 + d * dx;
			y_arrow = 500 - d * dy;
		}

		// отрисовка попы
		s_ass.setPosition(Vector2f(x_ass, y_ass));
		if (!check)	window.draw(s_ass);
		else  // если попали - рисуем с кровью
		{
			s_ass_blood.setPosition(Vector2f(x_ass, y_ass));
			window.draw(s_ass_blood);
		}

		// отрисовка шприца
		s_arrow.setOrigin(32.f, 150.f);
		s_arrow.rotate(deg);
		s_arrow.setPosition(Vector2f(x_arrow, y_arrow));
		s_arrow.scale(scale, scale);
		window.draw(s_arrow);

		// попал 
		if (check)
		{
			if (num_for_sound_check == 1)
			{
				buffer.loadFromFile("sound\\check.ogg");
				check_sound.setBuffer(buffer);
				check_sound.play();
			}
			num_for_sound_check++;
			//запускаем таймер
			float time = cl.getElapsedTime().asSeconds();
			cl.restart();
			timer += time;
			if (timer > delay)
			{
				timer = 0;
				if (k < 1) k += scale_factor; // масштабируем В яблочко
			}
			s_apple.scale(k, k);
			s_apple.setPosition(Vector2f(400, 300));
			window.draw(s_apple);
		}

		// не попал
		if (y_arrow < 0)
		{
			if (num_for_sound_off == 1)
			{
				buffer_off.loadFromFile("sound\\off.ogg");
				off_sound.setBuffer(buffer_off);
				off_sound.play();
			}
			num_for_sound_off++;
			//запускаем таймер
			float time = cl.getElapsedTime().asSeconds();
			cl.restart();
			timer += time;
			if (timer > delay)
			{
				timer = 0;
				if (h < 1) h += scale_factor; // масштабируем Проиграл
			}
			s_over.scale(h, h);
			s_over.setPosition(Vector2f(400, 300));
			window.draw(s_over);
		}

		if (Keyboard::isKeyPressed(Keyboard::Enter))  // Нажали на пробел - перезапуск игры
		{
			if (check) // если попали
			{
				score += (score_add + abs(deg)); // добавляем очки при попадании
				score_from_begin_level += (score_add + abs(deg));
				if (score_from_begin_level >= score_next) // переход на следующий уровень при наборе score_next очков
				{
					score_from_begin_level = 0;
					level++; 
					speed_ass *= speed_ass_add;
				}
			}

			if (y_arrow < 0) // если не попали
			{
				speed_ass = speed_ass_ini;
				level = 1;
				score = 0;
				score_from_begin_level = 0;
			}

			// перезапуск игры
			x_arrow = 0; y_arrow = 0; deg = 0; d = 0;
			scale = 1;
			x_ass = 400;
			start = false;
			check = false;
			k = 0.1;
			h = 0.1;
			num_for_sound_check = 1;
			num_for_sound_start = 1;
			num_for_sound_off = 1;
		}

		level_string.str(""); // очищаем строковую переменную уровня
		level_string << level; // уровень 
		score_string.str("");  // очистка строковой переменной очков
		score_string << score; // присваиваем текстовой переменной значение числовой переменной (для вывода в виде текста)

		// выводим количество очков
		score_text.setPosition(10, 480);
		score_text.setString("Очки: " + score_string.str());
		score_text.setColor(Color::Blue);
		score_text.setStyle(Text::Bold);
		window.draw(score_text);

		// выводим уровень
		level_text.setPosition(10, 520);
		level_text.setString("Уровень: " + level_string.str());
		level_text.setColor(Color::Red);
		level_text.setStyle(Text::Bold);
		window.draw(level_text);
		window.display();
	}
	return 0;
}