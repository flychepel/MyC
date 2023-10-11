#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>                                      
#define PI 3.14159265  
#include <iostream>
#include <time.h>
#include <sstream>

using namespace sf;

std::ostringstream score_string; // ��������� ���������� ��� ���������� �����
std::ostringstream level_string; // ��������� ���������� ��� ������ ������

int deg = 0, // ���� �������� ������ � ��������
	d = 0,   // �������� �������� ������
	score = 0, // ���������� �����
	score_from_begin_level = 0, // ���������� ��������� ����� � ������ ������
	level = 1, // �������
	//i = 0, // ������� ���������
	num_for_sound_check = 1, // ����� ���� ��������� ��� 1 ���
	num_for_sound_start = 1, // ����� ���� ������ ��� 1 ���
	num_for_sound_off = 1; // ����� ���� �������� ��� 1 ���

float k = 0.1, h = 0.1, // �������� "��������" � "� �������"
	  speed_ass = 4; // �������� ����

double  rad = 0, // ���� �������� ������ � ��������
		dx = 0,  // ����������� �������� ������ �� ��� �
		dy = 0,  // ����������� �������� ������ �� ��� �
		x_ass = 400, // ���������� ���� �
		y_ass = 80,  // ���������� ���� �
		x_arrow = 0, // ���������� ������ � 
		y_arrow = 0, // ���������� ������ �
		scale = 1,   // ������� ������
		x2 = 0,      // ���������� ����� ���� �
		y2 = 0,		 // ���������� ����� ���� �
		timer = 0,    // ����� �������
		delay = 0.01;  // �������� ��� ��������������� �������

bool start = false, // ����� ������
	 check = false; // ��������� � ����

const int def = 40; // �������� ��� ��������� ( +/- )
const int speed_arrow = 17; // �������� ������
const float scale_factor = 0.1; // �������� ��������� �������� � �������
const int score_add = 100; // ������� ����� ��������� ��� ���������
const float speed_ass_add = 1.1; // �� ������� ��� ����������� �������� ���� ��� ���������
//const int num_for_add_level = 3; // ������� ��� ���� �������, ����� ��������� �������
const float speed_ass_ini = 4; // ��������� �������� ����
const int score_next = 300; // ������� ����� ���� ������� ��� �������� �� �������� �������

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
	
	// ��������� �����
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
			// ������������ ����� �� �������� � ����� ������� ����?
			if (event.type == Event::Closed) window.close();
			// ������� ������ (���� �� �� �����)
			if (event.key.code == Keyboard::Right && (!start)) deg += 5;
			if (event.key.code == Keyboard::Left && (!start)) deg -= 5;
			// ��������� �����
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

		// �������� �����
		Texture t_arrow;
		t_arrow.loadFromFile("textures\\shpritz.png");
		Sprite s_arrow(t_arrow);
		t_arrow.setSmooth(true);

		// �������� ����
		Texture t_ass;
		t_ass.loadFromFile("textures\\ass.png");
		Sprite s_ass(t_ass);
		//t_ass.setSmooth(true);
		s_ass.setOrigin(65.f, 70.f);

		// �������� ���� � ������
		Texture t_ass_blood;
		t_ass_blood.loadFromFile("textures\\ass_blood.png");
		Sprite s_ass_blood(t_ass_blood);
		t_ass_blood.setSmooth(true);
		s_ass_blood.setOrigin(65.f, 70.f);

		// �������� � �������
		Texture t_apple;
		t_apple.loadFromFile("textures\\apple.jpg");
		Sprite s_apple(t_apple);
		s_apple.setOrigin(150.f, 110.f);

		// �������� ��������
		Texture t_over;
		t_over.loadFromFile("textures\\over.png");
		Sprite s_over(t_over);
		s_over.setOrigin(124.f, 90.f);

		rad = (PI * deg) / 180; // ��������� ������� � �������

		// ����������� �������� �� ���� (������� �� ���� �������)
		dx = sin(rad);
		dy = cos(rad);

		if (start)	// ��������� ����� ��� ������� �� ������
		{
			d += speed_arrow;	//�������� �������� ������
			if (!check) scale -= 0.01;	//��������� ������� ������
		}

		if (x_ass > 720 || x_ass < 60) speed_ass = -speed_ass;	// ���� ���� ������� �� �����, ������������� �

		// �������� ���������		
		if (x_arrow > (x_ass - def) && x_arrow < (x_ass + def) && y_arrow >(y_ass - def) && y_arrow < (y_ass + def) && start) check = true;

		if (!check)	// ���� ������ - ����� � ���� �������� �� �����
		{
			x_ass += speed_ass;	// ������� ����
			//���������� ������
			x_arrow = 400 + d * dx;
			y_arrow = 500 - d * dy;
		}

		// ��������� ����
		s_ass.setPosition(Vector2f(x_ass, y_ass));
		if (!check)	window.draw(s_ass);
		else  // ���� ������ - ������ � ������
		{
			s_ass_blood.setPosition(Vector2f(x_ass, y_ass));
			window.draw(s_ass_blood);
		}

		// ��������� ������
		s_arrow.setOrigin(32.f, 150.f);
		s_arrow.rotate(deg);
		s_arrow.setPosition(Vector2f(x_arrow, y_arrow));
		s_arrow.scale(scale, scale);
		window.draw(s_arrow);

		// ����� 
		if (check)
		{
			if (num_for_sound_check == 1)
			{
				buffer.loadFromFile("sound\\check.ogg");
				check_sound.setBuffer(buffer);
				check_sound.play();
			}
			num_for_sound_check++;
			//��������� ������
			float time = cl.getElapsedTime().asSeconds();
			cl.restart();
			timer += time;
			if (timer > delay)
			{
				timer = 0;
				if (k < 1) k += scale_factor; // ������������ � �������
			}
			s_apple.scale(k, k);
			s_apple.setPosition(Vector2f(400, 300));
			window.draw(s_apple);
		}

		// �� �����
		if (y_arrow < 0)
		{
			if (num_for_sound_off == 1)
			{
				buffer_off.loadFromFile("sound\\off.ogg");
				off_sound.setBuffer(buffer_off);
				off_sound.play();
			}
			num_for_sound_off++;
			//��������� ������
			float time = cl.getElapsedTime().asSeconds();
			cl.restart();
			timer += time;
			if (timer > delay)
			{
				timer = 0;
				if (h < 1) h += scale_factor; // ������������ ��������
			}
			s_over.scale(h, h);
			s_over.setPosition(Vector2f(400, 300));
			window.draw(s_over);
		}

		if (Keyboard::isKeyPressed(Keyboard::Enter))  // ������ �� ������ - ���������� ����
		{
			if (check) // ���� ������
			{
				score += (score_add + abs(deg)); // ��������� ���� ��� ���������
				score_from_begin_level += (score_add + abs(deg));
				if (score_from_begin_level >= score_next) // ������� �� ��������� ������� ��� ������ score_next �����
				{
					score_from_begin_level = 0;
					level++; 
					speed_ass *= speed_ass_add;
				}
			}

			if (y_arrow < 0) // ���� �� ������
			{
				speed_ass = speed_ass_ini;
				level = 1;
				score = 0;
				score_from_begin_level = 0;
			}

			// ���������� ����
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

		level_string.str(""); // ������� ��������� ���������� ������
		level_string << level; // ������� 
		score_string.str("");  // ������� ��������� ���������� �����
		score_string << score; // ����������� ��������� ���������� �������� �������� ���������� (��� ������ � ���� ������)

		// ������� ���������� �����
		score_text.setPosition(10, 480);
		score_text.setString("����: " + score_string.str());
		score_text.setColor(Color::Blue);
		score_text.setStyle(Text::Bold);
		window.draw(score_text);

		// ������� �������
		level_text.setPosition(10, 520);
		level_text.setString("�������: " + level_string.str());
		level_text.setColor(Color::Red);
		level_text.setStyle(Text::Bold);
		window.draw(level_text);
		window.display();
	}
	return 0;
}