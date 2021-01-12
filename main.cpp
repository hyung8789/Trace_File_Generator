#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <iostream>

#define RAND_MODE 0 //trace 순서를 섞는다
#define OVERWRITE_COUNT 0 //Overwrite 횟수 : 모든 섹터(페이지)에 대해 각각의 위치에 대한 Overwrite 횟수만큼 생성

#define MB_PER_BLOCK 64 //1MB당 64 블록
#define MB_PER_SECTOR 2048 //1MB당 2048섹터
#define BLOCK_PER_SECTOR 32 //한 블록에 해당하는 섹터의 개수
#define SPARE_BLOCK_RATIO 0.08 //전체 블록 개수에 대한 시스템에서 관리할 Spare Block 비율 (기본 값 8%)

unsigned int size = 0; //생성 할 개수
unsigned short mb = 0;
unsigned int block_size = 0;
unsigned int sector_size = 0;
float mb_gen_ratio = 0;
bool* written_chk_array = NULL; //중복 검사

void main()
{
	int select = 0;
	printf("0 : MB단위에 해당하는 전체 섹터(페이지)개수만큼 생성\n");
	printf("1 : 일정 개수 만큼 생성\n");
	printf("2 : MB단위에 해당하는 전체 섹터(페이지)개수 - Spare Block이 차지하는 섹터(페이지) 개수만큼 생성\n");
	printf("3 : MB단위에 해당하는 전체 섹터(페이지)개수 - Spare Block이 차지하는 섹터(페이지) 개수에 전체에 대한 일정 비율 만큼 채우기 위해 생성\n");
	
	printf(">>");
	scanf("%d", &select);

	switch (select)
	{
	case 0:
		goto MB_PAGE_SIZE_GEN;

	case 1:
		goto SPECIFIC_SIZE_GEN;

	case 2:
		goto MB_PAGE_SIZE_NOT_INC_SPARE_GEN;

	case 3:
		goto MB_PAGE_SIZE_RATIO_NOT_INC_SPARE_GEN;
	}

MB_PAGE_SIZE_GEN:
	printf("MB 단위 입력 (최대 65472MB) : ");
	scanf("%hd", &mb);

	block_size = mb * MB_PER_BLOCK; //할당된 메모리 크기에 해당하는 전체 블록의 개수
	sector_size = block_size * BLOCK_PER_SECTOR; //할당된 메모리 크기에 해당하는 전체 섹터의 개수
	size = sector_size;

	goto WRITE_TO_FILE;

SPECIFIC_SIZE_GEN:
	printf("생성할 개수 : ");
	scanf("%u", &size);

	goto WRITE_TO_FILE;

MB_PAGE_SIZE_NOT_INC_SPARE_GEN:
	printf("MB 단위 입력 (최대 65472MB) : ");
	scanf("%hd", &mb);

	block_size = (mb * MB_PER_BLOCK) - (unsigned int)round((mb * MB_PER_BLOCK) * SPARE_BLOCK_RATIO); //할당된 메모리 크기에 해당하는 Spare Block을 제외한 전체 블록의 개수
	sector_size = block_size * BLOCK_PER_SECTOR; //할당된 메모리 크기에 해당하는 전체 섹터의 개수
	size = sector_size;

	goto WRITE_TO_FILE;

MB_PAGE_SIZE_RATIO_NOT_INC_SPARE_GEN:
	printf("MB 단위 입력 (최대 65472MB) : ");
	scanf("%hd", &mb);

	while (1)
	{
		printf("MB 용량에 대한 생성하고자 하는 비율 설정 (0.0 초과 ~ 1.0 이하) : ");
		scanf("%f", &mb_gen_ratio);

		if (mb_gen_ratio > 0 && mb_gen_ratio <= 1.0)
			break;

		mb_gen_ratio = 0;
	}

	block_size = (mb * MB_PER_BLOCK) - (unsigned int)round((mb * MB_PER_BLOCK) * SPARE_BLOCK_RATIO); //할당된 메모리 크기에 해당하는 Spare Block을 제외한 전체 블록의 개수
	sector_size = block_size * BLOCK_PER_SECTOR; //할당된 메모리 크기에 해당하는 전체 섹터의 개수
	size = sector_size * mb_gen_ratio;

	goto WRITE_TO_FILE;

WRITE_TO_FILE:
	char file_name[FILENAME_MAX];
	FILE* fp = NULL;

	std::cout << "출력 trace 파일 이름 지정 (이름.확장자) >>";
	//gets_s(file_name, FILENAME_MAX);
	std::cin >> file_name;

	fp = fopen(file_name, "wt");

	switch (RAND_MODE)
	{
	case 0: //순서대로 기록
		if (OVERWRITE_COUNT > 0)
		{
			int overwrite_counter = 0; //현재 Overwrite횟수 카운터
			while (1) //Overwrite 횟수만큼 반복
			{
				for (unsigned int i = 0; i < size; i++)
					fprintf(fp, "W\t%u\n", i);

				overwrite_counter++;
				if (overwrite_counter > OVERWRITE_COUNT)
					break;
			}
		}
		else
		{
			for (unsigned int i = 0; i < size; i++)
				fprintf(fp, "W\t%u\n", i);
		}

		break;

	case 1: //랜덤한 순서로 기록
		if (OVERWRITE_COUNT > 0)
		{
			int overwrite_counter = 0; //현재 Overwrite횟수 카운터
			while (1) //Overwrite 횟수만큼 반복
			{
				srand((unsigned)time(NULL));
				if (written_chk_array != NULL)
				{
					delete[] written_chk_array;
					written_chk_array = NULL;
				}
				written_chk_array = new bool[size];
				unsigned int write_counter = 0; //출력 카운트를 통해 size만큼 출력 되었는지 확인

				for (unsigned int i = 0; i < size; i++) //초기화
				{
					written_chk_array[i] = false; //아직 기록되지 않음
				}

				while (1)
				{
					unsigned int rand_num = rand() % size; //0 ~ size 범위
					if (written_chk_array[rand_num] == false) //기록되지 않았으면
					{
						written_chk_array[rand_num] = true;
						fprintf(fp, "W\t%u\n", rand_num);
						write_counter++;
					}

					if (write_counter == size) //기록횟수 만큼 수행되었으면 종료
						break;
				}
				overwrite_counter++;
				if (overwrite_counter > OVERWRITE_COUNT)
					break;
			}
		}
		else
		{
			srand((unsigned)time(NULL));
			if (written_chk_array != NULL)
			{
				delete[] written_chk_array;
				written_chk_array = NULL;
			}
			written_chk_array = new bool[size];
			unsigned int write_counter = 0; //출력 카운트를 통해 size만큼 출력 되었는지 확인

			for (unsigned int i = 0; i < size; i++) //초기화
			{
				written_chk_array[i] = false; //아직 기록되지 않음
			}

			while (1)
			{
				unsigned int rand_num = rand() % size; //0 ~ size 범위
				if (written_chk_array[rand_num] == false) //기록되지 않았으면
				{
					written_chk_array[rand_num] = true;
					fprintf(fp, "W\t%u\n", rand_num);
					write_counter++;
				}

				if (write_counter == size) //기록할 개수만큼 수행되었으면 종료
					break;
			}
		}

		break;
	}
	
	if(OVERWRITE_COUNT ==  0)
		printf("trace.txt >> %u 개수만큼 생성됨\n", size);
	else
		printf("trace.txt >> %u 개수만큼 생성됨\n", size * (OVERWRITE_COUNT + 1));

	system("pause");
	exit(1);
}