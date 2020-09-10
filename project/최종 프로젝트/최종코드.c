
#include<stdio.h>
#include <conio.h>
#include <string.h>
#include<Windows.h>
#include<time.h> // clock 함수
#include<stdlib.h> //feof, exit


#define STAGE_DATA_FILE "pushpush.txt"
#define RANK_DATA_FILE "rank.txt"
#define MAX_STAGE 200
#define EXT_KEY         0xffffffe0   //확장키 인식값 방향키 값 입력받기 위해 매개체로 사용 .
#define KEY_LEFT      75
#define KEY_RIGHT      77
#define KEY_UP         72
#define KEY_DOWN      80
#define JUMP     97 // 알파벳 a
struct member {
	char name[30];
	float time_result;
};


enum {
	BLACK,      /*  0 : 까망 */
	DARK_BLUE,    /*  1 : 어두운 파랑 */
	DARK_GREEN,    /*  2 : 어두운 초록 */
	DARK_SKY_BLUE,  /*  3 : 어두운 하늘 */
	DARK_RED,    /*  4 : 어두운 빨강 */
	DARK_VOILET,  /*  5 : 어두운 보라 */
	DARK_YELLOW,  /*  6 : 어두운 노랑 */
	GRAY,      /*  7 : 회색 */
	DARK_GRAY,    /*  8 : 어두운 회색 */
	BLUE,      /*  9 : 파랑 */
	GREEN,      /* 10 : 초록 */
	SKY_BLUE,    /* 11 : 하늘 */
	RED,      /* 12 : 빨강 */
	VOILET,      /* 13 : 보라 */
	YELLOW,      /* 14 : 노랑 */
	WHITE,      /* 15 : 하양 */
};

typedef struct _stage {
	char   map[20][20];
	int pos_x;   //캐릭터의 x좌표
	int pos_y;
}STAGE;

//*********************************
//전역변수선언
//*********************************
int c_pos_x, c_pos_y;   //현재 캐릭터의 좌표값
int stageNum = 0;   //현재 스테이지 번호 1부터 시작
int stageMax = 3;   //최대 스테이지 3개
int step = 1;   //움직인 횟수
int item1 = 0;
int item2 = 0;
int item3 = 0;
STAGE re_stage[2000];   //되돌리기 기능을 위한 임시저장 영역의 포인터정보
STAGE stage_data[MAX_STAGE];   //스테이지 최초정보 저장

//*********************************
//함수 선언
//*********************************
void init();               //각종변수 초기화
void logo();         //시작로고
int show_stage(STAGE *p_stage, int all_load);   //stage를 화면에 출력 all_load가 1이면 벽도 출력
int gotoxy(int x, int y);   //커서옮기기
int check_success(STAGE *p_stage); //스테이지를 끝냈는지 검사
int show_frame(int start_col, int all_load);   //화면 오른쪽의 상태창출력 all_load가 1이면 모두출력 0이면 변수만출력
int show_success();
void bublesort(struct member members[30], struct member temp);
void SetColor(int color);   //색표현
void jump();
void item();

void main()
{

	int i = 0;
	int flag = 0;
	int tempKey;
	int stage_max_width = 0;
	clock_t start = 0, end = 0;
	struct member members[30], temp = { "student",0 };  // 이름, 시간 구조체  i가 0인것 부터 시작
	int s = 0, p = 0;
	char ch;
	FILE  *fp = fopen("rank.txt", "a");
	STAGE cur_stage;
	init();
	logo();
	gotoxy(12, 12);
	printf("이름을 입력하세요 :");
	while (1) {
		ch = getchar();
		if (ch == '\n') break;
		members[p].name[s++] = ch;
	}
	members[p].name[s] = '\0';
	start = clock();


	// 시작화면 



	SetColor(GRAY);
	stageNum = 0;
	cur_stage = stage_data[stageNum];
	re_stage[0] = cur_stage; // 되돌리기 위해 스테이지 정보 저장
	c_pos_x = cur_stage.pos_x;
	c_pos_y = cur_stage.pos_y;

	stage_max_width = show_stage(&cur_stage, 1); // max_width 리턴값 .. 1은 변화가 있는 지 없는 지
	show_frame(stage_max_width, 1);  // 옆에 상태창 띄울 때 , maxwidth로 시작 포인트 잡아줌
	//gotoxy(c_pos_x, c_pos_y);
	int h = 0;

	while (h == 0)
	{
		if (kbhit()) //키보드가 hit 되면
		{
			tempKey = getch();
			if (tempKey == 224 || tempKey == 0)
			{
				tempKey = getch();// 확장키(=방향기 스페이스바등등) 한번 더 받아주면 정수값으로 바꿀 수 있음
				switch (tempKey)
				{
				case KEY_UP:

					//위로이동 /*위로 올렸는데 옆에 커서가 가면서 별표가 생김  '*'자리에서 위로 $을 올릴때 였음
					if (cur_stage.map[c_pos_y - 2][c_pos_x] == '@')
					{
						if (cur_stage.map[c_pos_y - 1][c_pos_x] == ' ')
						{
							cur_stage.map[c_pos_y - 2][c_pos_x] = ' ';
						}
						else if (cur_stage.map[c_pos_y - 1][c_pos_x] == '$')
						{
							cur_stage.map[c_pos_y - 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y - 1][c_pos_x] = ' ';
						}
						else if (cur_stage.map[c_pos_y - 1][c_pos_x] == '*')
						{
							cur_stage.map[c_pos_y - 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y - 1][c_pos_x] = ' ';
						}
						else if (cur_stage.map[c_pos_y - 1][c_pos_x] == '#')break;
						c_pos_y--;
						flag = 1;
						break;
					}

					if (cur_stage.map[c_pos_y - 1][c_pos_x] == '#')
						break; // 벽이면 이동 x
					if (cur_stage.map[c_pos_y - 1][c_pos_x] == '@')
					{
						cur_stage.map[c_pos_y - 1][c_pos_x] = ' ';
						c_pos_y--;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y - 1][c_pos_x] == ' ' || cur_stage.map[c_pos_y - 1][c_pos_x] == '.')
					{
						c_pos_y--;
						flag = 1;
						break;
					} // 그냥 아무 방해물 없이 이동중 .이랑 빈공간 일때

					if (cur_stage.map[c_pos_y - 2][c_pos_x] == ' ' || cur_stage.map[c_pos_y - 2][c_pos_x] == '.') // 윗줄에서 한칸 먼저 움직이고 나서 이므로 연속 빈공간이랑 안겹침 즉, 위에 돌이 있다고 가정한 상황이 됨.
					{
						if (cur_stage.map[c_pos_y - 2][c_pos_x] == ' ' && cur_stage.map[c_pos_y - 1][c_pos_x] == '$')
						{
							cur_stage.map[c_pos_y - 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y - 1][c_pos_x] = ' ';
						}// 돌 밀면 돌은 나보다 두칸 위로 가고 나는 한칸 위로 가고
						else if (cur_stage.map[c_pos_y - 2][c_pos_x] == '.' && cur_stage.map[c_pos_y - 1][c_pos_x] == '$')
						{
							cur_stage.map[c_pos_y - 2][c_pos_x] = '*'; // 돌 밀었는데 .에 돌이 들어가버리면 *로 바뀌기
							cur_stage.map[c_pos_y - 1][c_pos_x] = ' ';
						}
						if (cur_stage.map[c_pos_y - 1][c_pos_x] == '*')
						{

							cur_stage.map[c_pos_y - 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y - 1][c_pos_x] = '.';

						}// 돌탱이 빈공간으로 만들기
						//돌탱이 박스에서 나오면 다시 박스로 만들기 

						c_pos_y--; // 현재위치 한 칸 올리기
						flag = 1;
						break;
					}
					break;
				case KEY_LEFT:      //왼쪽으로 이동
					if (cur_stage.map[c_pos_y][c_pos_x - 2] == '@')
					{
						if (cur_stage.map[c_pos_y][c_pos_x - 1] == ' ')
						{
							cur_stage.map[c_pos_y][c_pos_x - 2] = ' ';
						}
						else if (cur_stage.map[c_pos_y][c_pos_x - 1] == '$')
						{
							cur_stage.map[c_pos_y][c_pos_x - 2] = '$';
							cur_stage.map[c_pos_y][c_pos_x - 1] = ' ';
						}
						else if (cur_stage.map[c_pos_y][c_pos_x - 1] == '#')break;
						c_pos_x--;
						flag = 1;
						break;
					}

					if (cur_stage.map[c_pos_y][c_pos_x - 1] == '@')
					{

						cur_stage.map[c_pos_y][c_pos_x - 1] = ' ';
						c_pos_x--;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y][c_pos_x - 1] == '#')
						break;
					if (cur_stage.map[c_pos_y][c_pos_x - 1] == ' ' || cur_stage.map[c_pos_y][c_pos_x - 1] == '.' || cur_stage.map[c_pos_y][c_pos_x - 1] == '@')
					{
						c_pos_x--;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y][c_pos_x - 2] == ' ' || cur_stage.map[c_pos_y][c_pos_x - 2] == '.')
					{
						if (cur_stage.map[c_pos_y][c_pos_x - 2] == ' ' && cur_stage.map[c_pos_y][c_pos_x - 1] == '$')
						{
							cur_stage.map[c_pos_y][c_pos_x - 2] = '$';
							cur_stage.map[c_pos_y][c_pos_x - 1] = ' ';
						}// 돌 밀면 돌은 나보다 두칸 위로 가고 나는 한칸 위로 가고
						else if (cur_stage.map[c_pos_y][c_pos_x - 2] == '.' && cur_stage.map[c_pos_y][c_pos_x - 1] == '$')
						{
							cur_stage.map[c_pos_y][c_pos_x - 2] = '*'; // 돌 밀었는데 .에 돌이 들어가버리면 *로 바뀌기
							cur_stage.map[c_pos_y][c_pos_x - 1] = ' ';
						}
						if (cur_stage.map[c_pos_y][c_pos_x - 1] == '*')
						{

							cur_stage.map[c_pos_y][c_pos_x - 2] = '$';
							cur_stage.map[c_pos_y][c_pos_x - 1] = '.';
						}// 돌탱이 빈공간으로 만들기
						 //돌탱이 박스에서 나오면 다시 박스로 만들기

						c_pos_x--;
						flag = 1;
						break;
					}
					break;
				case KEY_RIGHT:
					//오른쪽으로 이동
					if (cur_stage.map[c_pos_y][c_pos_x + 2] == '@')
					{
						if (cur_stage.map[c_pos_y][c_pos_x + 1] == ' ')
						{
							cur_stage.map[c_pos_y][c_pos_x + 2] = ' ';
						}
						else if (cur_stage.map[c_pos_y][c_pos_x + 1] == '$')
						{
							cur_stage.map[c_pos_y][c_pos_x + 2] = '$';
							cur_stage.map[c_pos_y][c_pos_x + 1] = ' ';
						}
						else if (cur_stage.map[c_pos_y][c_pos_x + 1] == '#')break;
						c_pos_x++;
						flag = 1;
						break;
					}

					if (cur_stage.map[c_pos_y][c_pos_x + 1] == '#')
						break;
					if (cur_stage.map[c_pos_y][c_pos_x + 1] == '@')
					{
						cur_stage.map[c_pos_y][c_pos_x + 1] = ' ';
						c_pos_x++;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y][c_pos_x + 1] == ' ' || cur_stage.map[c_pos_y][c_pos_x + 1] == '.')
					{

						c_pos_x++;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y][c_pos_x + 2] == ' ' || cur_stage.map[c_pos_y][c_pos_x + 2] == '.')
					{
						if (cur_stage.map[c_pos_y][c_pos_x + 2] == ' ' && cur_stage.map[c_pos_y][c_pos_x + 1] == '$')
						{
							cur_stage.map[c_pos_y][c_pos_x + 2] = '$';
							cur_stage.map[c_pos_y][c_pos_x + 1] = ' ';
						}// 돌 밀면 돌은 나보다 두칸 위로 가고 나는 한칸 위로 가고
						else if (cur_stage.map[c_pos_y][c_pos_x + 2] == '.' && cur_stage.map[c_pos_y][c_pos_x + 1] == '$')
						{
							cur_stage.map[c_pos_y][c_pos_x + 2] = '*'; // 돌 밀었는데 .에 돌이 들어가버리면 *로 바뀌기
							cur_stage.map[c_pos_y][c_pos_x + 1] = ' ';
						}

						if (cur_stage.map[c_pos_y][c_pos_x + 1] == '*')
						{

							cur_stage.map[c_pos_y][c_pos_x + 1] = '.';
							cur_stage.map[c_pos_y][c_pos_x + 2] = '$';
						}// 돌탱이 빈공간으로 만들기
						 //돌탱이 박스에서 나오면 다시 박스로 만들기

						c_pos_x++;
						flag = 1;
						break;
					}
					break;
				case KEY_DOWN:      //아래로 이동
					if (cur_stage.map[c_pos_y + 2][c_pos_x] == '@')
					{
						if (cur_stage.map[c_pos_y + 1][c_pos_x] == ' ')
						{
							cur_stage.map[c_pos_y + 2][c_pos_x] = ' ';
						}
						else if (cur_stage.map[c_pos_y + 1][c_pos_x] == '$')
						{
							cur_stage.map[c_pos_y + 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y + 1][c_pos_x] = ' ';
						}
						else if (cur_stage.map[c_pos_y + 1][c_pos_x] == '#')break;
						c_pos_y++;
						flag = 1;
						break;
					}

					if (cur_stage.map[c_pos_y + 1][c_pos_x] == '@')
					{
						cur_stage.map[c_pos_y + 1][c_pos_x] = ' ';
						c_pos_y++;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y + 1][c_pos_x] == '#')
						break;
					if (cur_stage.map[c_pos_y + 1][c_pos_x] == ' ' || cur_stage.map[c_pos_y + 1][c_pos_x] == '.' || cur_stage.map[c_pos_y + 1][c_pos_x] == '@')
					{
						c_pos_y++;
						flag = 1;
						break;
					}
					if (cur_stage.map[c_pos_y + 2][c_pos_x] == ' ' || cur_stage.map[c_pos_y + 2][c_pos_x] == '.')
					{
						if (cur_stage.map[c_pos_y + 2][c_pos_x] == ' ' && cur_stage.map[c_pos_y + 1][c_pos_x] == '$')
						{
							cur_stage.map[c_pos_y + 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y + 1][c_pos_x] = ' ';
						}// 돌 밀면 돌은 나보다 두칸 위로 가고 나는 한칸 위로 가고
						else if (cur_stage.map[c_pos_y + 2][c_pos_x] == '.' && cur_stage.map[c_pos_y + 1][c_pos_x] == '$')
						{
							cur_stage.map[c_pos_y + 2][c_pos_x] = '*'; // 돌 밀었는데 .에 돌이 들어가버리면 *로 바뀌기
							cur_stage.map[c_pos_y + 1][c_pos_x] = ' ';
						}
						if (cur_stage.map[c_pos_y + 1][c_pos_x] == '*')
						{
							cur_stage.map[c_pos_y + 2][c_pos_x] = '$';
							cur_stage.map[c_pos_y + 1][c_pos_x] = '.'; // 돌탱이 빈공간으로 만들기
						} //돌탱이 박스에서 나오면 다시 박스로 만들기

						c_pos_y++;
						flag = 1;
						break;
					}
					break;
				}
			}

			else if (tempKey == 32)   //스페이스바를 눌렀을때 다시 시작
			{
				cur_stage = stage_data[stageNum];
				re_stage[0] = cur_stage;
				c_pos_x = cur_stage.pos_x;
				c_pos_y = cur_stage.pos_y;
				step = 1;
				flag = 1;
			}

			else if (tempKey == JUMP && item1 > 0) // 알파벳 a 로 ㄱ
			{
				tempKey = getch();
				if (tempKey == 224 || tempKey == 0)
				{
					tempKey = getch();
					switch (tempKey)
					{
					case KEY_UP:
						if (cur_stage.map[c_pos_y - 1][c_pos_x] == '#' || cur_stage.map[c_pos_y - 1][c_pos_x] == '*' || cur_stage.map[c_pos_y - 1][c_pos_x] == '$')    //벽이나 상자가 꽉찬 상태
						{
							if (cur_stage.map[c_pos_y - 2][c_pos_x] == '~' || cur_stage.map[c_pos_y - 2][c_pos_x] == '$')break;
							c_pos_y -= 2;
							flag = 1;
							break;
						}


					case KEY_LEFT:
						if (cur_stage.map[c_pos_y][c_pos_x - 1] == '#' || cur_stage.map[c_pos_y][c_pos_x - 1] == '*' || cur_stage.map[c_pos_y][c_pos_x - 1] == '$')
						{
							if (cur_stage.map[c_pos_y][c_pos_x - 2] == '~' || cur_stage.map[c_pos_y][c_pos_x - 2] == '$')break;
							c_pos_x -= 2;
							flag = 1;
							break;
						}


					case KEY_RIGHT:
						if (cur_stage.map[c_pos_y][c_pos_x + 1] == '#' || cur_stage.map[c_pos_y][c_pos_x + 1] == '*' || cur_stage.map[c_pos_y][c_pos_x + 1] == '$')
						{
							if (cur_stage.map[c_pos_y][c_pos_x + 2] == '~' || cur_stage.map[c_pos_y][c_pos_x + 2] == '$')break;
							c_pos_x += 2;
							flag = 1;
							break;
						}


					case KEY_DOWN:
						if (cur_stage.map[c_pos_y + 1][c_pos_x] == '#' || cur_stage.map[c_pos_y + 1][c_pos_x] == '*' || cur_stage.map[c_pos_y + 1][c_pos_x] == '$')
						{
							if (cur_stage.map[c_pos_y + 2][c_pos_x] == '~' || cur_stage.map[c_pos_y + 2][c_pos_x] == '$')break;
							c_pos_y += 2;
							flag = 1;
							break;
						}

					}

				}
				if (flag == 1)
				{

					re_stage[step] = cur_stage;
					re_stage[step].pos_x = c_pos_x;
					re_stage[step].pos_y = c_pos_y;
					show_stage(&cur_stage, 0);
					show_frame(stage_max_width, 0);
					flag = 0;
					step++;
				}
				fflush(stdin);
				SetColor(BLACK);
				Sleep(15);
				item1--;
			}
			// 공을 밀면 있던 자리에 상자가 생기고 상자에서 꺼내면 공이 사라짐

			if (check_success(&cur_stage))
			{
				show_stage(&cur_stage, 0);
				show_success(); // 성공했슴니다!
				stageNum++;   //다음 stage로 넘어감

				system("cls");
				if (stageNum != 3) {

					item();
				}

				else {
					system("cls");
				}

				if (stageNum < stageMax)
				{
					step = 0;
					cur_stage = stage_data[stageNum];
					c_pos_x = cur_stage.pos_x;
					c_pos_y = cur_stage.pos_y;
					re_stage[0] = cur_stage;
					stage_max_width = show_stage(&cur_stage, 1);
					show_frame(stage_max_width, 1);
				}
				else if (stageNum == stageMax)
				{
					h++;
				}
			}
		}
		if (flag == 1)
		{

			re_stage[step] = cur_stage;
			re_stage[step].pos_x = c_pos_x;
			re_stage[step].pos_y = c_pos_y;
			show_stage(&cur_stage, 0);
			show_frame(stage_max_width, 0);
			flag = 0;
			step++;
		}

		fflush(stdin);
		SetColor(BLACK);
		Sleep(15);
	}
	SetColor(WHITE);
	system("cls");
	gotoxy(12, 12);

	end = clock();
	members[i].time_result = (float)(end - start) / CLOCKS_PER_SEC;
	members[i].time_result = members[i].time_result - (item2 * 7);

	printf("소요시간 : %f\n\n\n\n", members[i].time_result);

	fprintf(fp, "%s %.2f\n", members[i].name, members[i].time_result);
	fclose(fp);
	system("cls");
	printf("************랭킹************\n");
	bublesort(members, temp);
	Sleep(10000);
}

void bublesort(struct member members[30], struct member temp)
{
	FILE *fp = fopen("rank.txt", "rt");
	int i = 0;
	while (!feof(fp)) {
		fscanf(fp, "%s %f", &members[i].name, &members[i].time_result);
		i++;
	}
	fclose(fp);

	for (int k = 0; k < i; k++)
	{
		for (int p = 0; p < k; p++)
		{
			if (members[k].time_result < members[p].time_result)
			{
				temp = members[k];
				members[k] = members[p];
				members[p] = temp;
			}
		}
	}
	fp = fopen("final_rank.txt", "wt");
	for (int k = 0; k < i; k++)
	{
		fprintf(fp, "%d등 : %s  시간 : %.2f초\n", k + 1, members[k].name, members[k].time_result);
	}
	fclose(fp);
	for (int k = 0; k < i - 1; k++)
	{
		printf("%d등 : %s  시간 : %.2f초\n", k + 1, members[k + 1].name, members[k + 1].time_result);
	}
}
int gotoxy(int x, int y)
{
	COORD pos;
	pos.Y = y;
	pos.X = x;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	return 0;
}

void SetColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void init()
{
	FILE *fp;
	char buf[50];
	int stage_seq = -1;
	int row = 0;
	int i, j;
	fp = fopen(STAGE_DATA_FILE, "r");
	while (!feof(fp))
	{
		fgets(buf, 49, fp); //49 조정해줘야될 것 같은데.. ~추가하면
		if (buf[0] == ';')   //스테이지의 시작
		{
			for (i = 0; i < 20; i++)   //시작위치 찾기
			{
				for (j = 0; j < 20; j++)
				{
					if (stage_data[stage_seq].map[i][j] == '@')
					{
						stage_data[stage_seq].pos_y = i;
						stage_data[stage_seq].pos_x = j;
						i = 20;   //루프 탈출
						break; // 맨처음에는 -1이니까 그냥 스킵
						// 한줄씩 읽은 뒤에 체크 
					}

				}
			}
			row = 0;
			stage_seq++;
			continue;
		}
		if (stage_seq >= 0 && buf[0] != '\n')
		{
			strncpy(stage_data[stage_seq].map[row], buf, strlen(buf) - 1);
		}
		row++; //  stage는 0인 상태에서 buf로 ;전까지만 똑 떼서 한줄씩 stage_data[i]에 넣어주는 역할 buf는 계속 초기화되는 듯 넣을 때마다 
	}


}
void item()
{
	srand(time(NULL)); //seed변경하여 현재시간을 이용해 완벽한 난수 설정 
	int num = rand() % 4 + 1; //난수범위 1부터 3까지 생성 
	int choose; //입력할 수와 횟수를 세기 위한 변수 선언 
	int x = 5; //타이틀화면이 표시되는 x좌표 
	int y = 4; //타이틀화면이 표시되는 y좌표 
	gotoxy(x, y + 0); printf("■■■■■   ■■■■■   ■■■■■");
	gotoxy(x, y + 1); printf("■      ■   ■      ■   ■      ■");
	gotoxy(x, y + 2); printf("■   1  ■   ■   2  ■   ■   3  ■");
	gotoxy(x, y + 3); printf("■      ■   ■      ■   ■      ■");
	gotoxy(x, y + 4); printf("■■■■■   ■■■■■   ■■■■■");
	SetColor(WHITE);
	gotoxy(x + 40, y + 0); printf("점프 : 벽을 점프해서 넘어가는 개꿀템");
	gotoxy(x + 40, y + 2); printf("최종기록차감 : 최종 시간 기록에서 7초를 빼dream");
	gotoxy(x + 40, y + 4); printf("꽝: 뭔지 알잖아");

	gotoxy(x, y + 6); printf("아이템을 뽑아주세요! : ");

	scanf("%d", &choose);
	if (num == 1) {
		item1++;
		gotoxy(x, y + 9); printf("%d번 아이템 '점프' 당첨! ", choose);
		gotoxy(x, y + 11); printf("막힌 벽을 넘어보자! 두칸은 안돼! 이걸 걸린 당신은 A+ 받을 자격도 있군요!");
	}
	else if (num == 2) {
		item2++;
		gotoxy(x, y + 9); printf("%d번 아이템 '최종 기록 차감' 당첨! ", choose);
		gotoxy(x, y + 11); printf("기록을 빼서 순위를 높여보자! 순위보상 요구는 금지! 이걸 걸린 당신은 A+ 받을 자격도 있군요! ");
	}
	else if (num == 3) {
		item3++;
		gotoxy(x, y + 9); printf("%d번 아이템 꽝! ", choose);
		gotoxy(x, y + 11); printf("세상에! 1/3의 확률을 뚫은 당신! 이걸 걸린 당신은 A+ 받을 자격도 있군요!");
	}
	else
	{
		item3++;
		gotoxy(x, y + 9); printf("%d번 아이템 꽝! ", choose);
		gotoxy(x, y + 11); printf("세상에! 1/3의 확률을 뚫은 당신! 이걸 걸린 당신은 A+ 받을 자격도 있군요!");

	}
	SetColor(YELLOW);
	gotoxy(x, y + 13); printf("□□  □□  누적아이템");
	gotoxy(x, y + 14); printf("■      ■  점프 : %d 개", item1);
	gotoxy(x, y + 15); printf("■ wow! ■  최종기록차감 : %d 개", item2);
	gotoxy(x, y + 16); printf("■      ■  꽝 : %d 개", item3);
	gotoxy(x, y + 17); printf("■■■■■");
	Sleep(5000);
}
void logo()
{
	gotoxy(0, 0);
	int i;
	SetColor(WHITE * 16);
	gotoxy(3, 2);
	printf("                                                                                  ");
	gotoxy(3, 10);
	printf("                                                                                  ");
	for (i = 3; i < 10; i++) {
		gotoxy(3, i);
		printf("  ");

		gotoxy(83, i);
		printf("  ");

	}
	Sleep(50);

	SetColor(RED * 16);
	//첫째줄
	gotoxy(5, 3);
	printf("        ");
	gotoxy(15, 3);
	printf("  ");
	gotoxy(21, 3);
	printf("  ");
	gotoxy(25, 3);
	printf("        ");
	gotoxy(35, 3);
	printf("  ");
	gotoxy(41, 3);
	printf("  ");

	gotoxy(45, 3);
	printf("        ");
	gotoxy(55, 3);
	printf("  ");
	gotoxy(61, 3);
	printf("  ");
	gotoxy(65, 3);
	printf("        ");
	gotoxy(75, 3);
	printf("  ");
	gotoxy(81, 3);
	printf("  ");

	Sleep(50);
	//둘째줄
	SetColor(YELLOW * 16);
	gotoxy(5, 4);
	printf("  ");
	gotoxy(11, 4);
	printf("  ");
	gotoxy(15, 4);
	printf("  ");
	gotoxy(21, 4);
	printf("  ");
	gotoxy(25, 4);
	printf("  ");
	gotoxy(35, 4);
	printf("  ");
	gotoxy(41, 4);
	printf("  ");

	gotoxy(45, 4);
	printf("  ");
	gotoxy(55, 4);
	printf("  ");
	gotoxy(51, 4);
	printf("  ");
	gotoxy(61, 4);
	printf("  ");
	gotoxy(65, 4);
	printf("  ");
	gotoxy(75, 4);
	printf("  ");
	gotoxy(81, 4);
	printf("  ");
	Sleep(50);
	//셋째줄
	SetColor(GREEN * 16);
	gotoxy(5, 5);
	printf("  ");
	gotoxy(11, 5);
	printf("  ");
	gotoxy(15, 5);
	printf("  ");
	gotoxy(21, 5);
	printf("  ");
	gotoxy(25, 5);
	printf("  ");
	gotoxy(35, 5);
	printf("  ");
	gotoxy(41, 5);
	printf("  ");

	gotoxy(45, 5);
	printf("  ");
	gotoxy(55, 5);
	printf("  ");
	gotoxy(51, 5);
	printf("  ");
	gotoxy(61, 5);
	printf("  ");
	gotoxy(65, 5);
	printf("  ");
	gotoxy(75, 5);
	printf("  ");
	gotoxy(81, 5);
	printf("  ");

	Sleep(50);
	//넷째줄
	SetColor(DARK_SKY_BLUE * 16);
	gotoxy(5, 6);
	printf("        ");
	gotoxy(15, 6);
	printf("  ");
	gotoxy(21, 6);
	printf("  ");
	gotoxy(25, 6);
	printf("        ");
	gotoxy(35, 6);
	printf("        ");

	gotoxy(45, 6);
	printf("        ");
	gotoxy(55, 6);
	printf("  ");
	gotoxy(61, 6);
	printf("  ");
	gotoxy(65, 6);
	printf("        ");
	gotoxy(75, 6);
	printf("        ");
	Sleep(50);
	//다섯째줄
	SetColor(BLUE * 16);
	gotoxy(5, 7);
	printf("  ");
	gotoxy(15, 7);
	printf("  ");
	gotoxy(21, 7);
	printf("  ");
	gotoxy(31, 7);
	printf("  ");
	gotoxy(35, 7);
	printf("  ");
	gotoxy(41, 7);
	printf("  ");

	gotoxy(45, 7);
	printf("  ");
	gotoxy(55, 7);
	printf("  ");
	gotoxy(61, 7);
	printf("  ");
	gotoxy(71, 7);
	printf("  ");
	gotoxy(75, 7);
	printf("  ");
	gotoxy(81, 7);
	printf("  ");
	Sleep(50);
	//여섯째줄
	SetColor(VOILET * 16);
	gotoxy(5, 8);
	printf("  ");
	gotoxy(15, 8);
	printf("  ");
	gotoxy(21, 8);
	printf("  ");
	gotoxy(31, 8);
	printf("  ");
	gotoxy(35, 8);
	printf("  ");
	gotoxy(41, 8);
	printf("  ");

	gotoxy(45, 8);
	printf("  ");
	gotoxy(55, 8);
	printf("  ");
	gotoxy(61, 8);
	printf("  ");
	gotoxy(71, 8);
	printf("  ");
	gotoxy(75, 8);
	printf("  ");
	gotoxy(81, 8);
	printf("  ");
	Sleep(50);
	//일곱째줄
	SetColor(DARK_VOILET * 16);
	gotoxy(5, 9);
	printf("  ");
	gotoxy(15, 9);
	printf("        ");
	gotoxy(25, 9);
	printf("        ");
	gotoxy(35, 9);
	printf("  ");
	gotoxy(41, 9);
	printf("  ");

	gotoxy(45, 9);
	printf("  ");
	gotoxy(55, 9);
	printf("        ");
	gotoxy(65, 9);
	printf("        ");
	gotoxy(75, 9);
	printf("  ");
	gotoxy(81, 9);
	printf("  ");
	Sleep(50);

	SetColor(WHITE);
	gotoxy(18, 15);
	printf(" [소프트웨어설계 프로젝트]");
	gotoxy(22, 17);
	SetColor(DARK_YELLOW);
	printf(" <FOEVERYOUNG조>");
	SetColor(GRAY);
	gotoxy(22, 19);
	printf(" 2018707034 강나경");
	gotoxy(22, 21);
	printf(" 2017707029 이승건");
	gotoxy(22, 23);
	printf(" 2016707079 하상천");

	SetColor(DARK_RED);
	gotoxy(17, 15);
	printf("♥");
	gotoxy(17, 17);
	printf("♥");
	gotoxy(17, 19);
	printf("♥");
	gotoxy(17, 21);
	printf("♥");
	gotoxy(17, 23);
	printf("♥");
	gotoxy(44, 15);
	printf("♥");
	gotoxy(44, 17);
	printf("♥");
	gotoxy(44, 19);
	printf("♥");
	gotoxy(44, 21);
	printf("♥");
	gotoxy(44, 23);
	printf("♥");
	SetColor(YELLOW * 16 + RED);
	gotoxy(16, 25);
	printf("                                 ");
	gotoxy(16, 26);
	printf("  아무거나 누르면 시작한당(>_<)  ");
	gotoxy(16, 27);
	printf("                                 ");
	gotoxy(18, 30);

	fflush(stdin);
	Sleep(1000);
	getche();
	system("cls");
}


int show_stage(STAGE *p_stage, int all_load)
{
	int i, j;
	int max_width = 0;
	if (all_load == 1)
		system("cls");

	for (i = 0; i < 20; i++)
	{
		printf("  ");
		for (j = 0; j < 20; j++)
		{

			if (i == c_pos_y && j == c_pos_x)
			{
				gotoxy(j * 2 + 2, i + 2);
				SetColor(SKY_BLUE);
				printf("★");
			}
			else {
				switch (p_stage->map[i][j])
				{
				case '#':   //벽일때
					if (all_load == 1)
					{
						gotoxy(j * 2 + 2, i + 2);
						SetColor(WHITE);
						printf("▩");

						if (j > max_width)
							max_width = j;
					}
					break;
				case '@':
					gotoxy(j * 2 + 2, i + 2);
					printf("  ");
					break;
				case '.':   //상자를 놓는 장소(현재 빈상태)
					SetColor(WHITE);
					gotoxy(j * 2 + 2, i + 2);
					printf("○");
					break;
				case '$':   //상자
					SetColor(RED);
					gotoxy(j * 2 + 2, i + 2);
					printf("●");
					break;
					//case '+':
				case '*':   //상자를 놓는 장소(현재 상자가 놓인 상태)
					SetColor(GREEN);
					gotoxy(j * 2 + 2, i + 2);
					printf("◎");
					break;
				case 0:
					if (all_load == 1)
					{
						gotoxy(j * 2 + 2, i + 2);
						printf("  ");
					}
					break;
				case ' ':   //공백
					gotoxy(j * 2 + 2, i + 2);
					printf("  ");
					break;
				}
			}
		}
		printf("\n");
	}
	return max_width * 2 + 5;
}

int check_success(STAGE *p_stage)
{
	int i, j;
	for (i = 0; i < 20; i++)
	{
		for (j = 0; j < 20; j++)
		{
			if (p_stage->map[i][j] == '.' || p_stage->map[i][j] == '$')   //빈돌이나 빈구멍이 있을때
				return 0; //실패
		}
	}
	return 1; //성공
}

int show_frame(int start_col, int all_load)
{
	if (all_load == 1)
	{
		SetColor(WHITE);
		gotoxy(start_col, 1);
		printf("┏━━━━━━━━━━━━━━━━┓");
		gotoxy(start_col, 2);
		printf("┃   광운대학교   ┃");
		gotoxy(start_col, 3);
		printf("┃ 전자통신공학과 ┃");
		gotoxy(start_col, 4);
		printf("┃ Foreveryoung조 ┃");
		gotoxy(start_col, 5);
		printf("┣━━━━━━━━━━━━━━━━┫");
		gotoxy(start_col, 6);
		printf("┃ Stage          ┃");
		gotoxy(start_col, 7);
		printf("┃                ┃");
		gotoxy(start_col, 8);
		printf("┃ Step  :        ┃");
		gotoxy(start_col, 9);
		printf("┣━━━━━━━━━━━━━━━━┫");
		gotoxy(start_col, 10);
		printf("┃     조작법     ┃");
		gotoxy(start_col, 11);
		printf("┃                ┃");
		gotoxy(start_col, 12);
		printf("┃↑↓←→ :방향키┃");
		gotoxy(start_col, 13);
		printf("┃                ┃");
		gotoxy(start_col, 14);
		printf("┃ Space: 다시시작┃");
		gotoxy(start_col, 15);
		printf("┃                ┃");
		gotoxy(start_col, 16);
		printf("┗━━━━━━━━━━━━━━━━┛");
		SetColor(RED);
		gotoxy(start_col + 2, 2);
		printf("♥");
		gotoxy(start_col + 14, 2);
		printf("♥");
	}
	SetColor(GREEN);
	gotoxy(start_col + 9, 6);
	printf("%3d/%-3d", stageNum + 1, stageMax);
	SetColor(YELLOW);
	gotoxy(start_col + 12, 8);
	printf("%-4d", step);
	gotoxy(0, 0);
	return 0;
}


int show_success()
{
	SetColor(DARK_YELLOW);
	gotoxy(10, 6);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~");
	gotoxy(10, 7);
	printf(" 경   STAGE CLEAR !!  축");
	gotoxy(10, 8);
	printf("~~~~~~~~~~~~~~~~~~~~~~~~");


	Sleep(3000);
	return 0;
}