#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int sudoku[9][9];
int sudoku_exam[9][9];
int level, answer_count;
int start_time, game_time;

int get_rand_num(int start, int end, int seed) {
	int rand_num;
	
	srand(time(NULL));	
	rand_num = (rand() * seed) % (end - start + 1) + start;
	
	return rand_num;
}

int check_sudoku_num(int y, int x) {
	int i, j;
	
	// 1. 작은 사각형 검사
	int sy, sx;	
	sy = y / 3 * 3, sx = x / 3 * 3;
	
	for(i = sy; i < sy + 3; i++) {
		for(j = sx; j < sx + 3; j++) {
			if(i != y && j != x) {
				if(sudoku[y][x] == sudoku[i][j]) {
					return 0;
				}
			}
		}
	} 
	
	// 가로 줄 검사 
	for(i = 0; i < 9; i++) {
		if(x != i && sudoku[y][x] == sudoku[y][i]) {
			return 0;
		}
	} 
	
	// 세로 줄 검사 
	for(i = 0; i < 9; i++) {
		if(y != i && sudoku[y][x] == sudoku[i][x]) {
			return 0;
		}
	}	 
	
	return 1;
}

int check_wrong_sudoku(int y, int x) {
	int i, temp, result = 0;
	
	temp = sudoku[y][x];
	for(i = 1; i <= 9; i++) {
		sudoku[y][x] = i;
		
		if(check_sudoku_num(y, x)) {
			result = 1;
			break;
		}
	}
	
	sudoku[y][x] = temp;
	
	return result;
}

int create_sudoku() {	
	int i, j, seed = 1234;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			sudoku[i][j] = 0;
		}
	}
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; ) {
			sudoku[i][j] = get_rand_num(1, 9, seed++);
			
			if(check_sudoku_num(i, j)) {
				j++;
			}
			else if(!check_wrong_sudoku(i, j)) {
				return 0;
			}
			
			if(seed > 9999) {
				seed = 1234;
			}
		}
	}
	
	return 1;
}

void create_sudoku_exam() {
	int small_rectangle_y[9] = {0, 0, 0, 3, 3, 3, 6, 6, 6};
	int small_rectangle_x[9] = {0, 3, 6, 0, 3, 6, 0, 3, 6};
	int i, j, y, x, current_rectangle, seed = 4321;
	
	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++) {
			sudoku_exam[i][j] = sudoku[i][j];
		}
	}
	
	current_rectangle = get_rand_num(0, 8, seed++);
	
	for(i = 0; i < level;) {
		current_rectangle %= 9;
		
		y = small_rectangle_y[current_rectangle] + get_rand_num(0, 2, seed++);
		x = small_rectangle_x[current_rectangle] + get_rand_num(0, 2, seed++);
		
		if(sudoku_exam[y][x] != 0) {
			sudoku_exam[y][x] = 0;
			i++;
			current_rectangle++;
		}
	}
}

void print_sudoku() {
	int i, j;
	game_time = time(NULL) - start_time;
	
	printf("\n\t [정답/문제수] %d / %d  || 게임시간: %d초 \n", answer_count, level, game_time);
	
	for(i = 0; i < 9; i++) {
		
		if(i == 0) {
			printf("\n\n\t %4d%4d%4d%4d%4d%4d%4d%4d%4d \n", 1, 2, 3, 4, 5, 6, 7, 8, 9);
			printf("\t ------------------------------------ ");
		}
		
		printf("\n       %d|", i + 1);
		
		for(j = 0; j < 9; j++) {
			if(sudoku_exam[i][j] != 0) {
				printf("%4d", sudoku_exam[i][j]);
			}
			else {
				printf("%4c", 'X');
			}
		}
		
		printf("\n\t|");
	}	
}

void play_sudoku() {
	int i, x, y, v;
	
	printf("\n\t\t [ SUDOKU Game ] \n");
	printf("\n\t\t\t\t\t\t 2019.03.20 Written by Longlee \n");
	printf("\n\t 게임의 레벨을 선택하세요! \n");
	printf("\t 1: 숫자 20 개 맞추기 \n");
	printf("\t 2. 숫자 35 개 맞추기 \n");
	printf("\t 3. 숫자 50 개 맞추기 \n");
	printf("\t >>> ");
	
	scanf("%d", &level);
	
	printf("\n\n\t 레벨%d, 20개 숫자 맞추기를 선택하셨습니다. \n", level);
	
	switch(level) {
		case 1:
			level = 20;
			break;
		case 2:
			level = 35;
			break;
		case 3:
			level = 50;
			break;
		default:
			level = 20;
	}
	
	create_sudoku_exam();
	start_time = time(NULL);
	
	for(i = 0; i < level; ) {
		print_sudoku();
		
		printf("\n\n\t 맞추고자 하는 비어있는 번호의 가로(x), 세로(y) 번호와 답(v)을 입력해 주세요! \n");
		printf("\t [입력예]  >>> x y v \n\n");
		printf("\t >>> ");
		scanf("%d %d %d", &x, &y, &v);
		
		if(x < 1 || x > 9 || y < 1 || y > 9 || v < 1 || v > 9) {
			printf("\n\n\t 올바른 값을 입력하세요!!! \n");
		}
		else {
			x--, y--;
			
			if(sudoku[y][x] == v) {
				sudoku_exam[y][x] = v;
				answer_count++;
				i++;
				printf("\n\t 정답 ... ^^* \n\n");
			}
			else {
				printf("\n\t 오답 ... -,.ㅜ \n\n");
			}
			
			Sleep(500);
			system("cls"); 
		}
	}
	
	game_time = time(NULL) - start_time;
	
	printf("\n\n\t [ 게임 종료! ] 총 게임시간: %d초 \n", game_time);
}

int main(void) {
	while(1) {
		if(create_sudoku()) {
			break;
		}
	}
	
	play_sudoku();
	
	return 0;
}












