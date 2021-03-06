/**
@file Partition-Data-Setting.cpp

@mainpage 데이터를 크기 분할하여 분할 된 파티션 공간에 값 변경 후 출력하기

@date 2018/04/23

@author 김규태(gyutae100@gmail.com)

@details 사용자가 원하는 파티션 크기로 나누어 선택 파티션에 값 대입 및 선택 파티션의 값 반환 가능

@brief 쉬프트 연산자를 사용한 비트 마스크 연산

@version 0.0.1
*/
#include "stdafx.h"

#include <stdio.h>
#include <string.h>

#include <iostream>

#include "lib_bit.h"

using namespace std;

/**
@brief 메인 코드

@todo unsigned int a=0에서 a--시 0xffff ffff가 된다. 입력 값은 자연수이면서 unsigned int값 이내의 값만 입력 받을 수 있도록 에외처리가 필요하다.

@todo 
*/
void main(void)
{

	unsigned int data = 0;
	unsigned int value = 0;

	//파티션 분할 크기 
	int sz_bit_of_partition = 8;

	//총 파티션 갯수
	int cnt_partition = sizeof( data );
	while ( true ) {

		//분할 파티션 중 선택 파티션 위치
		int pos_partition = 0;

		//분할 된 파티션 중 복사 할 파티션 위치 입력받음
		printf( "위치(0~%d) : ", cnt_partition - 1 );
		scanf_s( "%d", &pos_partition );

		//예외 처리 : 파티션 크기 범주 벗어나는 경우
		if ( cnt_partition < pos_partition - 1 ) {

			puts("범위 벗어난 파티션 위치 입력");
			continue;
		}

		//파티션 최대 값
		int available_value = ( 1 << (sz_bit_of_partition) )-1;

		//분할 된 파티션에 복사 할 값 입력받음
		printf("값 [0~ %d] : ", available_value);
		scanf_s("%d", &value);

		//예외 처리 : 파티션 크기는 1바이트로 제한됨
		if (value > available_value) {

			puts("범위 벗어난 데이터 값 입력");
			continue;
		}

		unsigned int partition_value = 0;

		//데이터를 정한 비트 크기 단위로 파티션 분할 후 선택 파티션의 값을 변경한다.
		if (CopyPartitionValueTo(&data, pos_partition, sz_bit_of_partition, value)) {

			unsigned int partition_value = 0;

			//파티션을 8bit 단위로 리틀엔디안방식으로 10진수 값 출력
			for (int i = ( sz_bit_of_partition / 2 ) -1 ; i >=0 ; i--) {

				if (GetPartitionValue(&partition_value, data, i, sz_bit_of_partition)) {

					printf("%d 번째 바이트 값 : %d \n", i, partition_value);
				}
			}

			//파티션을 4bit 단위로 리틀엔디안방식으로 16진수 값 출력
			printf("전체 4바이트 값 : Ox");
			for (int i = sz_bit_of_partition ; i >= 0 ; i--) {

				if (GetPartitionValue(&partition_value, data, i, sz_bit_of_partition/2 )) {

					printf("%x", partition_value);
				}
			}

			puts(" ");

			//16진수 출력
			printf("Ox%08x\n\n", data);
		}
		else {

			puts("잘못 된 입력, 다시 시도해주세요");
		}


	}


}