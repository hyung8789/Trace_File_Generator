#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <time.h>

#define RAND_MODE 0 //trace ������ ���´�
#define OVERWRITE_COUNT 0 //Overwrite Ƚ�� : ��� ����(������)�� ���� ������ ��ġ�� ���� Overwrite Ƚ����ŭ ����

#define MB_PER_BLOCK 64 //1MB�� 64 ���
#define MB_PER_SECTOR 2048 //1MB�� 2048����
#define BLOCK_PER_SECTOR 32 //�� ��Ͽ� �ش��ϴ� ������ ����

unsigned int size = 0; //���� �� ����
unsigned short mb = 0;
unsigned int block_size = 0;
unsigned int sector_size = 0;
bool* written_chk_array = NULL; //�ߺ� �˻�

void main()
{
	int select = 0;
	printf("0 : MB������ �ش��ϴ� ��ü ����(������)������ŭ ����\n1 : ���� ���� ��ŭ ����\n>>");
	scanf("%d", &select);

	switch (select)
	{
	case 0:
		goto MB_PAGE_SIZE_GEN;
		break;

	case 1:
		goto SPECIFIC_SIZE_GEN;
		break;
	}

MB_PAGE_SIZE_GEN:
	printf("MB ���� �Է� (�ִ� 65472MB) : ");
	scanf("%hd", &mb);

	block_size = mb * MB_PER_BLOCK; //�Ҵ�� �޸� ũ�⿡ �ش��ϴ� ��ü ����� ���� (Spare Block ����)
	sector_size = block_size * BLOCK_PER_SECTOR; //�Ҵ�� �޸� ũ�⿡ �ش��ϴ� ��ü ������ ���� (Spare Block ����)
	size = sector_size;

	goto WRITE_TO_FILE;

SPECIFIC_SIZE_GEN:
	printf("������ ���� : ");
	scanf("%u", &size);

	goto WRITE_TO_FILE;

WRITE_TO_FILE:
	FILE* fp = fopen("trace.txt", "wt");

	switch (RAND_MODE)
	{
	case 0: //������� ���
		if (OVERWRITE_COUNT > 0)
		{
			int overwrite_counter = 0; //���� OverwriteȽ�� ī����
			while (1) //Overwrite Ƚ����ŭ �ݺ�
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

	case 1: //������ ������ ���
		if (OVERWRITE_COUNT > 0)
		{
			int overwrite_counter = 0; //���� OverwriteȽ�� ī����
			while (1) //Overwrite Ƚ����ŭ �ݺ�
			{
				srand((unsigned)time(NULL));
				if (written_chk_array != NULL)
				{
					delete[] written_chk_array;
					written_chk_array = NULL;
				}
				written_chk_array = new bool[size];
				unsigned int write_counter = 0; //��� ī��Ʈ�� ���� size��ŭ ��� �Ǿ����� Ȯ��

				for (unsigned int i = 0; i < size; i++) //�ʱ�ȭ
				{
					written_chk_array[i] = false; //���� ��ϵ��� ����
				}

				while (1)
				{
					unsigned int rand_num = rand() % size; //0 ~ size ����
					if (written_chk_array[rand_num] == false) //��ϵ��� �ʾ�����
					{
						written_chk_array[rand_num] = true;
						fprintf(fp, "W\t%u\n", rand_num);
						write_counter++;
					}

					if (write_counter == size) //����� ��ŭ ����Ǿ����� ����
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
			unsigned int write_counter = 0; //��� ī��Ʈ�� ���� size��ŭ ��� �Ǿ����� Ȯ��

			for (unsigned int i = 0; i < size; i++) //�ʱ�ȭ
			{
				written_chk_array[i] = false; //���� ��ϵ��� ����
			}

			while (1)
			{
				unsigned int rand_num = rand() % size; //0 ~ size ����
				if (written_chk_array[rand_num] == false) //��ϵ��� �ʾ�����
				{
					written_chk_array[rand_num] = true;
					fprintf(fp, "W\t%u\n", rand_num);
					write_counter++;
				}

				if (write_counter == size) //����� ��ŭ ����Ǿ����� ����
					break;
			}
		}

		break;
	}
	
	if(OVERWRITE_COUNT ==  0)
		printf("trace.txt >> %u ������ŭ ������\n", size);
	else
		printf("trace.txt >> %u ������ŭ ������\n", size * (OVERWRITE_COUNT + 1));

	system("pause");
	exit(1);
}