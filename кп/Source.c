#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <malloc.h>

const char Menu[] = "�������� ����� ���� ����� ��������������� �����:\n"
"1.�������� ����� ������ � ���� ������\n"
"2.������ ���� �������\n"
"3.������� � ���� ������\n"
"4.C��������� �� ��������\n"
"5.���������� � ����\n"
"6.����� �� ���������\n";


struct Base {
	char name[30];
	char site[30];
	char type[30];
	int date;
	double bytes;
}; typedef struct Base plugin_t;

void SozdanieNovogoFaila(FILE* file, char name[]); // ���������� ������� ��� �������� �������� ���������� ����� � �������� ������
void PechatVKonsol(plugin_t* Plugin_t, int RazmerMassiva); // ���������� ������� ��� ������ ���� ����� ������� � ����������� ����
void SochranenieFaila(int* RazmerMassiva, plugin_t* Plugin_t, char name[]); // ���������� ������� ��� ������ ��������� ����������� � ���� ������ � ����

int* PosikPoSaitam(plugin_t* Plugin_t, int RazmerMassiva, char SiteForSearch[], int* RezultatPoiska);// ���������� ������� ��� ������ �������� ��������� ������� �� �����
int* PosikPoBaitam(plugin_t* Plugin_t, int RazmerMassiva, int byteforsearch, int* RezultatPoiska);

plugin_t* ChtenieIzFaila(char name[], plugin_t* Plugin_t, int* RazmerMassiva); // ���������� ������� ��� ��������������� ���������� ������ �� ���������� �����
plugin_t* DobavleniePolya(plugin_t* Plugin_t, int RazmerMassiva); //  ���������� ������� ��� ���������� ����� ������ � ������
plugin_t* Sorting(plugin_t* plugins, int k);//���������� �� �����




int main() {
	setlocale(LC_ALL, 0);// ��������� ����������� ������� � �� �������
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

	//�������� ����������� ����� � ����� � ����������� ��� ��������
	if ((file = fopen(name, "r")) == NULL) {
		SozdanieNovogoFaila(file, name);
		fclose(file);
	}

	Plugin_t = (plugin_t*)calloc(1, sizeof(plugin_t));//��������� �������������� ������ ������ ��� ������
	Plugin_t = ChtenieIzFaila(name, Plugin_t, RazmerMassiva);// ������������� ������� �� �����

	printf("\t ��������� ��� ������ � ����� ������\n \t\"����������� ��������\" \n");
	printf(Menu);
	// ���������� ������������������ ���� ��� �������������� � ���������� � ������� ����� while � ��������� switch
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
			printf("������ ��������� � ����\n");
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
			printf("������� � ������ ���� ������\n");
			printf("1. ����� �� ����� ������������\n2. ����� �� ���������� ����\n ");
			scanf("%d", &MenuPoiska);
			switch (MenuPoiska) {
			case 1:
				system("cls");
				printf("������ ����� �� ����� �������������\n������� ���� ��� ������:  ");
				scanf("%s", &SiteForSearch);
				RezultatPoiska = PosikPoSaitam(Plugin_t, VremenniRazmer, SiteForSearch, RezultatPoiska);
				printf("�� ������� %s �������� ��������� ������\n", SiteForSearch);
				break;
			case 2:
				system("cls");
				printf("������ ����� �� ���������� ����\n������� ���������� ���� ��� ������:  ");
				scanf("%lf", &BytesForSearch);
				RezultatPoiska = PosikPoBaitam(Plugin_t, VremenniRazmer, BytesForSearch, RezultatPoiska);
				printf("�� ������� %lf ������� ��������� ������\n", BytesForSearch);
				break;
			}
			for (int i = 0; i < VremenniRazmer; i++) {
				int FoundIndex = *RezultatPoiska++;
				if (FoundIndex >= 0 && FoundIndex <= VremenniRazmer) {
					printf("\t ��������: %s\n����: %s\n���: %s\n���� ������� %d.%d.%d\n���������� ����: %lf\n", Plugin_t[i].name, Plugin_t[i].site, Plugin_t[i].type, Plugin_t[i].date / 1000000, Plugin_t[i].date % 1000000 / 10000, Plugin_t[i].date % 10000, Plugin_t[i].bytes);
				}
			}
			printf("\n");
			printf(Menu);
			break;
		case 5:
			SochranenieFaila(RazmerMassiva, Plugin_t, name);
			printf("��������� ��������� ���� ���������\n");
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

//����������� ������� ��� �������� ����� � ����� � ������ ��� ���������
void SozdanieNovogoFaila(FILE* file, char name[]) {
	file = fopen(name, "w+");
	fprintf(file, "%s %s %s %d %lf\n\n", "������_������", "site.com", "Utility", 10102023, 0.1);
}

//����������� ������� ��� �������������� ������������� ���� �� ����� 
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

//����������� ������� ��� ������ ���� � �������� ����
void PechatVKonsol(plugin_t* Plugin_t, int RazmerMassiva) {
	int buf = 0;
	printf("�\t |\t��������\t|\t����\t\t|\t��� �������\t|\t���� ������\t|\t����������� ����\t|\n");
	for (int i = 0; i < RazmerMassiva; i++) {
		printf("\n");
		printf("�%d  \t |\t%s\t\t|\t%s\t|\t%s\t\t|", i + 1, &Plugin_t[i].name, &Plugin_t[i].site, &Plugin_t[i].type);
		buf = Plugin_t[i].date / 1000000;
		printf("\t%d.", buf);
		buf = Plugin_t[i].date % 1000000 / 10000;
		printf("%d.", buf);
		buf = Plugin_t[i].date % 10000;
		printf("%d\t|\t%lf\t\t|\n", buf, Plugin_t[i].bytes);
	}
	printf("\n");
}

//����������� ������� ��� ���������� ��������� � ���� � ��������� ����
void SochranenieFaila(int* RazmerMassiva, plugin_t* Plugin_t, char name[]) {
	FILE* file;
	file = fopen(name, "w+");
	for (int i = 0; i < *RazmerMassiva; i++) {
		fprintf(file, "%s %s %s %d %lf\n\n", Plugin_t[i].name, Plugin_t[i].site, Plugin_t[i].type, Plugin_t[i].date, Plugin_t[i].bytes);
	}
	fclose(file);
}

//����������� ������� ��� ������ �� ����
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

//����������� ������� ��� ���������� � ������ ����� ������
plugin_t* DobavleniePolya(plugin_t* Plugin_t, int RazmerMassiva) {
	plugin_t* newBase_T = (plugin_t*)realloc(Plugin_t, (RazmerMassiva + 1) * sizeof(plugin_t));
	if (newBase_T == NULL) {
		printf("\a ������ ��� ���������� ������� �������\n");
		return Plugin_t;
	}
	printf("������� �������� �������\n");
	if (scanf("%s", &Plugin_t[RazmerMassiva].name) != 1) return 1;

	printf("������� ���� ������������ �������\n");
	if (scanf("%s", &Plugin_t[RazmerMassiva].site) != 1) return 1;

	printf("������� ���� ������� \n");;
	if (scanf("%s", &Plugin_t[RazmerMassiva].type) != 1) return 1;

	printf("������� ���� ������� ������� ��� ����� � ������� ��������\n");
	if (scanf("%d", &Plugin_t[RazmerMassiva].date) != 1) return 1;

	printf("������� ����������� ����\n");
	if (scanf("%lf", &Plugin_t[RazmerMassiva].bytes) != 1) return 1;
	return Plugin_t;
}

plugin_t* Sorting(plugin_t* plugins, int k)//����������
{
	struct Base tmp;
	printf("\n");
	printf("\t\t\t � ���������� �� �������� ������� \n");
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
