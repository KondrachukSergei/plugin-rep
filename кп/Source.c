#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

const char Menu[] = "Выберите пункт меню введя соответствующую цифру:\n"
"1.Создание новой записи в базе данных\n"
"2.Чтение всех записей\n"
"3.Переход в меню поиска\n"
"4.Cортировка по названию\n"
"5.Сохранение в файл\n"
"6.Выход из программы\n";


struct Base {
	char name[30];
	char site[30];
	char type[30];
	int date;
	double bytes;
}; typedef struct Base plugin_t;

void SozdanieNovogoFaila(FILE* file, char name[]); // ОБьявление функции для создания рабочего текстового файла с заданным именем
void PechatVKonsol(plugin_t* Plugin_t, int RazmerMassiva); // Обьявление функции для печати всех полей массива в определнном виде
void SochranenieFaila(int* RazmerMassiva, plugin_t* Plugin_t, char name[]); // Обьявление функции для записи изменений совершенных в базе данных в файл

int* PosikPoSaitam(plugin_t* Plugin_t, int RazmerMassiva, char SiteForSearch[], int* RezultatPoiska);// Обьявление функции для поиска индексов элементов массива по имени
int* PosikPoBaitam(plugin_t* Plugin_t, int RazmerMassiva, int byteforsearch, int* RezultatPoiska);

plugin_t* ChtenieIzFaila(char name[], plugin_t* Plugin_t, int* RazmerMassiva); // Обьявление функции для первоначального считывания данных из текстового файла
plugin_t* DobavleniePolya(plugin_t* Plugin_t, int RazmerMassiva); //  Обьявление функции для добавления новой записи в массив
plugin_t* Sorting(plugin_t* plugins, int k);//сортировка по цвету




int main() {
	setlocale(LC_ALL, 0);// Установка локализации консоли и ее очистка
	system("chcp 1251");
	system("cls");

	FILE* file;
	plugin_t* Plugin_t;

	int* RazmerMassiva = (int*)malloc(1000 * sizeof(int));

	int MenuVibora = 0;
	int MenuPoiska = 0;


	char name[] = "plugin.txt";

	char* SiteForSearch[12];
	double BytesForSearch;

	//Проверка существания файла с базой и усспешности его открытия
	if ((file = fopen(name, "r")) == NULL) {
		SozdanieNovogoFaila(file, name);
		fclose(file);
	}

	Plugin_t = (plugin_t*)calloc(1, sizeof(plugin_t));//Выделение первоначальной ячейки памяти под массив
	Plugin_t = ChtenieIzFaila(name, Plugin_t, RazmerMassiva);// Инициализация массива из файла

	printf("\t Программа для работы с базой данных\n \t\"Репозиторий плагинов\" \n");
	printf(Menu);
	// Реализация пользователььского меню для взаимодействия с программой с помощью цикла while и оператора switch
	while (1) {
		int VremenniRazmer = *RazmerMassiva;
		int* RezultatPoiska = (int*)calloc(VremenniRazmer, sizeof(int));
		scanf("%d", &MenuVibora);
		switch (MenuVibora)
		{
		case 1:
			system("cls");
			*RazmerMassiva = *RazmerMassiva + 1;
			Plugin_t = DobavleniePolya(Plugin_t, VremenniRazmer);
			printf("\n%d\n", *RazmerMassiva);
			printf("Запись добавлена в базу\n");
			printf(Menu);
			break;
		case 2:
			system("cls");
			PechatVKonsol(Plugin_t, VremenniRazmer);
			getchar();
			printf(Menu);
			break;
		case 4:
			system("cls");
			Plugin_t = Sorting(Plugin_t, VremenniRazmer);
			PechatVKonsol(Plugin_t, VremenniRazmer);
			printf(Menu);
			break;
		case 3:
			system("cls");
			printf("Переход к выбору типа поиска\n");
			printf("1. Поиск по сайту разработчика\n2. Поиск по количеству байт\n ");
			scanf("%d", &MenuPoiska);
			switch (MenuPoiska) {
			case 1:
				system("cls");
				printf("Выбран поиск по сайту производителя\nВведите сайт для поиска:  ");
				scanf("%s", &SiteForSearch);
				RezultatPoiska = PosikPoSaitam(Plugin_t, VremenniRazmer, SiteForSearch, RezultatPoiska);
				printf("По запросу %s найденны следующие записи\n", SiteForSearch);
				break;
			case 2:
				system("cls");
				printf("Выбран поиск по количеству байт\nВведите количество байт для поиска:  ");
				scanf("%lf", &BytesForSearch);
				RezultatPoiska = PosikPoBaitam(Plugin_t, VremenniRazmer, BytesForSearch, RezultatPoiska);
				printf("По запросу %lf найдены следующие записи\n", BytesForSearch);
				break;
			}
			for (int i = 0; i < VremenniRazmer; i++) {
				int FoundIndex = *RezultatPoiska++;
				if (FoundIndex >= 0 && FoundIndex <= VremenniRazmer) {
					printf("\t Название: %s\nСайт: %s\nТип: %s\nДата выпуска %d.%d.%d\nКоличество байт: %lf\n", Plugin_t[i].name, Plugin_t[i].site, Plugin_t[i].type, Plugin_t[i].date / 1000000, Plugin_t[i].date % 1000000 / 10000, Plugin_t[i].date % 10000, Plugin_t[i].bytes);
				}
			}
			printf("\n");
			printf(Menu);
			break;
		case 5:
			SochranenieFaila(RazmerMassiva, Plugin_t, name);
			printf("Внесенные изменения были сохранены\n");
			printf(Menu);
			break;
		case 6:
			free(Plugin_t);
			free(RezultatPoiska);
			exit(0);
			break;
		default:
			break;
		}
	}
}

//Определение функции для создания файла с базой в случае его отсуствия
void SozdanieNovogoFaila(FILE* file, char name[]) {
	file = fopen(name, "w+");
	fprintf(file, "%s %s %s %d %lf\n\n", "Первый_плагин", "site.com", "Utility", 10102023, 0.1);
}

//Определение функции для первоначальной инициализации базы из файла 
plugin_t* ChtenieIzFaila(char name[], plugin_t* Plugin_t, int* RazmerMassiva) {
	FILE* file;
	char buf;
	int newsize = 1, i = 0;
	file = fopen(name, "r");
	while (!feof(file)) {
		newsize++;
		if (i > 0)Plugin_t = (plugin_t*)realloc(Plugin_t, (newsize) * sizeof(plugin_t));
		fscanf(file, "%s %s %s %d %lf ", &Plugin_t[i].name, &Plugin_t[i].site, &Plugin_t[i].type, &Plugin_t[i].date, &Plugin_t[i].bytes);
		i++;
	}
	newsize = --i;
	*RazmerMassiva = newsize + 1;
	return Plugin_t;
}

//Определение функции для печати базы в заданном виде
void PechatVKonsol(plugin_t* Plugin_t, int RazmerMassiva) {
	int buf = 0;
	printf("№\t |\tНазвание\t|\tСайт\t\t|\tТип плагина\t|\tДата релиза\t|\tКолличество байт\t|\n");
	for (int i = 0; i < RazmerMassiva; i++) {
		printf("\n");
		printf("№%d  \t |\t%s\t\t|\t%s\t|\t%s\t\t|", i + 1, &Plugin_t[i].name, &Plugin_t[i].site, &Plugin_t[i].type);
		buf = Plugin_t[i].date / 1000000;
		printf("\t%d.", buf);
		buf = Plugin_t[i].date % 1000000 / 10000;
		printf("%d.", buf);
		buf = Plugin_t[i].date % 10000;
		printf("%d\t|\t%lf\t\t|\n", buf, Plugin_t[i].bytes);
	}
	printf("\n");
}

//Определение функции для сохранения изменений в базе в текстовый файл
void SochranenieFaila(int* RazmerMassiva, plugin_t* Plugin_t, char name[]) {
	FILE* file;
	file = fopen(name, "w+");
	for (int i = 0; i < *RazmerMassiva; i++) {
		fprintf(file, "%s %s %s %d %lf\n\n", Plugin_t[i].name, Plugin_t[i].site, Plugin_t[i].type, Plugin_t[i].date, Plugin_t[i].bytes);
	}
	fclose(file);
}

//Определение функции для поиска по дате
int* PosikPoSaitam(plugin_t* Plugin_t, int RazmerMassiva, char SiteForSearch[], int* RezultatPoiska) {
	int n = 0;
	for (int i = 0; i < RazmerMassiva; i++) {
		if (strcmp(Plugin_t[i].site, SiteForSearch) == 0) {
			RezultatPoiska[n++] = i;
		}
		else RezultatPoiska[n++] = RazmerMassiva + 10;
	}
	return RezultatPoiska;
}

//Определение функции для добавления в массив новой записи
plugin_t* DobavleniePolya(plugin_t* Plugin_t, int RazmerMassiva) {
	plugin_t* newBase_T = (plugin_t*)realloc(Plugin_t, (RazmerMassiva + 1) * sizeof(plugin_t));
	if (newBase_T == NULL) {
		printf("\a Ошибка при увеличении размера массива\n");
		return Plugin_t;
	}
	printf("Введите название плагина\n");
	if (scanf("%s", &Plugin_t[RazmerMassiva].name) != 1) return 1;

	printf("Введите сайт разработчика плагина\n");
	if (scanf("%s", &Plugin_t[RazmerMassiva].site) != 1) return 1;

	printf("Введите типа плагина \n");;
	if (scanf("%s", &Plugin_t[RazmerMassiva].type) != 1) return 1;

	printf("Введите дату выпуска плагина как число в формате ДДММГГГГ\n");
	if (scanf("%d", &Plugin_t[RazmerMassiva].date) != 1) return 1;

	printf("Введите колличество байт\n");
	if (scanf("%lf", &Plugin_t[RazmerMassiva].bytes) != 1) return 1;
	return Plugin_t;
}

plugin_t* Sorting(plugin_t* plugins, int k)//сортировка
{
	struct Base tmp;
	printf("\n");
	printf("\t\t\t » Сортировка по названию плагина \n");
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k - i - 1; j++)
		{
			if (strcmp(plugins[j].name, plugins[j + 1].name) > 0)
			{
				tmp = plugins[j];
				plugins[j] = plugins[j + 1];
				plugins[j + 1] = tmp;
			}
		}
	}
	return plugins;
}

int* PosikPoBaitam(plugin_t* Plugin_t, int RazmerMassiva, int byteforsearch, int* RezultatPoiska) {
	int n = 0;
	for (int i = 0; i < RazmerMassiva; i++) {
		if (Plugin_t[i].bytes == byteforsearch) {
			RezultatPoiska[n++] = i;
		}
		else RezultatPoiska[i] = RazmerMassiva + 10;
	}
	return RezultatPoiska;
}
