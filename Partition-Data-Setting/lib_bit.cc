#pragma once
#include "stdafx.h"

#include "lib_bit.h"

/**
@return bool :  정상 작동시 true, 아닌 경우 false

@param src_num : 2진 바이너리로 출력 할 값

@brief 10진수 값을 2진수 비트 단위로 출력한다.

@todo 예외 처리 미완
*/
bool DisplayNumToBit(unsigned int src_num) {

	//해당 자료형의 비트 갯수 구하기
	unsigned int cnt_bit = sizeof(src_num) * 8;

	//비트 MSB오프셋 설정
	unsigned int bit_offset = 1 << ( cnt_bit - 1 );

	//모든 비트 마스크 비트 갯수
	unsigned int total_cnt_bit = cnt_bit;

	//MSB 부터 최하위까지 반복
	for (int i = 1; i <= total_cnt_bit; i++) {

		//비트 마스크 연산
		if ( src_num & bit_offset ) {

			putchar('1');
		}
		else {

			putchar('0');
		}

		//8비트 단위로 공백 간격 출력!
		if ( 0 == ( i % 8 ) ) {

			putchar(' ');
		}

		bit_offset = bit_offset >> 1;
	}

	putchar('\n');

	return true;
}







/**
@return bool :  정상 작동시 true, 아닌 경우 false

@param data : 2진 비트 오프셋 연산 대상
@param target_bit_offset : data의 2진 비트 오프셋
@param set_bit_flag : true시 flag on, false시 flag off

@brief 데이터 값의 2진 비트 플래그의 오프셋의 값을 변경한다.
*/
bool SetBitFlag(unsigned int *data, unsigned int target_bit_offset, bool set_bit_flag) {

	//예외처리 : 값이 표현 가능한 비트 오프셋 범위 체크 체크
	int available_bit_cnt = sizeof(*data) * 8;

	//insigend int형은 비트가 0의 자릿수부터 31의 자릿수까지 표현 가능
	if ( available_bit_cnt <= target_bit_offset ) {

		return false;
	}

	int offset_bit = 1 << ( target_bit_offset );

	//기존 비트 오프셋의 비트 플래그가 1인 경우
	if ( *data & offset_bit ) {

		//해당 비트 오프셋을 값을 0으로 변경하는 경우
		if ( false == set_bit_flag ) {

			*data = *data & ( ~offset_bit );
		}
	}
	//기존 비트 오프셋의 비트 플래그가 0인 경우
	else {

		//해당 비트 오프셋 값을 1으로 변경하는 경우
		if ( true == set_bit_flag ) {

			*data = *data | ( offset_bit );
		}
	}

	return true;
}





/**
@return bool :  정상 작동시 true, 아닌 경우 false

@param partition_value : 분할 반환 값
@param pos_partition : 반환할 분할 공간 번호 
@param bit_size_of_partition : 분할 크기
@param src_data : 분할 대상 값

@brief 분할 크기 단위로 분할 후 분활 된 데이터 파티션의 값을 반환
*/
bool CopyPartitionValueTo(unsigned int* partition_value, int pos_partition, int bit_size_of_partition, unsigned int src_data) {

	//총 파티션 갯수
	int num_partition = sizeof(unsigned int);

	//파티션 최대 값
	int available_value = ( 1 << ( bit_size_of_partition ) ) - 1;

	unsigned int value = *partition_value;

	//예외 처리 : 파티션 크기 범주 벗어나는 경우
	if ( 0 > pos_partition || num_partition < pos_partition ) {

		//puts("범위 벗어난 파티션 위치 입력");
		return false;
	}

	//예외 처리 : 허용 된 값 이상 입력 된 경우
	//파티션 크기는 1바이트로 제한됨
	if ( src_data > available_value ) {

		//puts("범위 벗어난 데이터 값 입력");
		return false;
	}

	//파티션 번지수는 0번지 부터라네
	unsigned int dest_offset = 1 << ( ( ( pos_partition ) * bit_size_of_partition ) );
	unsigned int src_offset = 1;

	for (int i = 0; i < bit_size_of_partition; i++) {

		if (src_data & src_offset) {

			value = value | dest_offset;
		}
		else {

			value = value & ( ~dest_offset );
		}

		src_offset = src_offset << 1;
		dest_offset = dest_offset << 1;
	}

	*partition_value = value;

	return true;
}


/**
@return bool :  정상 작동시 true, 아닌 경우 false

@param partition_value : 분할 반환 값
@param pos_partition : 반환할 분할 공간 번호
@param bit_size_of_partition : 분할 크기

@brief 분할 크기 단위로 분할 후 분할 된 데이터 파티션 중 해당 pos의 데이터 값 반환 함수

@todo 비트 마스크를 1비트단위가 아닌 여러 비트를 동시에 비트 마스크 한다면 연산이 더 간단해 질 것이다. 근데 여러 비트 동시 비트 마스크용을 만드는 연산이 오히려 더 걸린다.
@todo 만일 1byte단위 혹은 4bit 단위 비트 마스크 출력인 경우 0xffff 또는 0xff를 비트 연산자로 이동 후 출력하면 속도면에서 빨라질 것이다
*/
bool GetPartitionValue(unsigned int *dest_data, unsigned int src_data, int pos_partition, int bit_size_of_partition) {

	//파티션 총 갯수
	int cnt_partition = (sizeof(src_data) * 8) / bit_size_of_partition;
	int parition_data = 0;

	//예외 처리 : 선택 파티션이 파티션 오프셋 벗어난 경우
	if (0 > pos_partition || cnt_partition - 1 < pos_partition) {

		return false;
	}

	//반환 값
	unsigned short dest_partiotion_value = 0;

	//파티션 오프셋은 0번지부터 시작
	//파티션 중 LSB 비트 오프셋으로 이동
	unsigned int src_offset = 1 << ( ( ( pos_partition ) * bit_size_of_partition) );
	unsigned int dest_offset = 1;

	//파티션 값 bit의 MSB부터 반복
	for (int i = 0; i < bit_size_of_partition; i++) {

		if ( ( src_data ) & src_offset ) {

			parition_data = parition_data | ( dest_offset );
		}

		dest_offset = dest_offset << 1;
		src_offset = src_offset << 1;
	}

	*dest_data = parition_data;
	return true;
}

