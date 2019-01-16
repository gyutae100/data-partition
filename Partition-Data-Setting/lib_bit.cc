#pragma once
#include "stdafx.h"

#include "lib_bit.h"

/**
@return bool :  ���� �۵��� true, �ƴ� ��� false

@param src_num : 2�� ���̳ʸ��� ��� �� ��

@brief 10���� ���� 2���� ��Ʈ ������ ����Ѵ�.

@todo ���� ó�� �̿�
*/
bool DisplayNumToBit(unsigned int src_num) {

	//�ش� �ڷ����� ��Ʈ ���� ���ϱ�
	unsigned int cnt_bit = sizeof(src_num) * 8;

	//��Ʈ MSB������ ����
	unsigned int bit_offset = 1 << ( cnt_bit - 1 );

	//��� ��Ʈ ����ũ ��Ʈ ����
	unsigned int total_cnt_bit = cnt_bit;

	//MSB ���� ���������� �ݺ�
	for (int i = 1; i <= total_cnt_bit; i++) {

		//��Ʈ ����ũ ����
		if ( src_num & bit_offset ) {

			putchar('1');
		}
		else {

			putchar('0');
		}

		//8��Ʈ ������ ���� ���� ���!
		if ( 0 == ( i % 8 ) ) {

			putchar(' ');
		}

		bit_offset = bit_offset >> 1;
	}

	putchar('\n');

	return true;
}







/**
@return bool :  ���� �۵��� true, �ƴ� ��� false

@param data : 2�� ��Ʈ ������ ���� ���
@param target_bit_offset : data�� 2�� ��Ʈ ������
@param set_bit_flag : true�� flag on, false�� flag off

@brief ������ ���� 2�� ��Ʈ �÷����� �������� ���� �����Ѵ�.
*/
bool SetBitFlag(unsigned int *data, unsigned int target_bit_offset, bool set_bit_flag) {

	//����ó�� : ���� ǥ�� ������ ��Ʈ ������ ���� üũ üũ
	int available_bit_cnt = sizeof(*data) * 8;

	//insigend int���� ��Ʈ�� 0�� �ڸ������� 31�� �ڸ������� ǥ�� ����
	if ( available_bit_cnt <= target_bit_offset ) {

		return false;
	}

	int offset_bit = 1 << ( target_bit_offset );

	//���� ��Ʈ �������� ��Ʈ �÷��װ� 1�� ���
	if ( *data & offset_bit ) {

		//�ش� ��Ʈ �������� ���� 0���� �����ϴ� ���
		if ( false == set_bit_flag ) {

			*data = *data & ( ~offset_bit );
		}
	}
	//���� ��Ʈ �������� ��Ʈ �÷��װ� 0�� ���
	else {

		//�ش� ��Ʈ ������ ���� 1���� �����ϴ� ���
		if ( true == set_bit_flag ) {

			*data = *data | ( offset_bit );
		}
	}

	return true;
}





/**
@return bool :  ���� �۵��� true, �ƴ� ��� false

@param partition_value : ���� ��ȯ ��
@param pos_partition : ��ȯ�� ���� ���� ��ȣ 
@param bit_size_of_partition : ���� ũ��
@param src_data : ���� ��� ��

@brief ���� ũ�� ������ ���� �� ��Ȱ �� ������ ��Ƽ���� ���� ��ȯ
*/
bool CopyPartitionValueTo(unsigned int* partition_value, int pos_partition, int bit_size_of_partition, unsigned int src_data) {

	//�� ��Ƽ�� ����
	int num_partition = sizeof(unsigned int);

	//��Ƽ�� �ִ� ��
	int available_value = ( 1 << ( bit_size_of_partition ) ) - 1;

	unsigned int value = *partition_value;

	//���� ó�� : ��Ƽ�� ũ�� ���� ����� ���
	if ( 0 > pos_partition || num_partition < pos_partition ) {

		//puts("���� ��� ��Ƽ�� ��ġ �Է�");
		return false;
	}

	//���� ó�� : ��� �� �� �̻� �Է� �� ���
	//��Ƽ�� ũ��� 1����Ʈ�� ���ѵ�
	if ( src_data > available_value ) {

		//puts("���� ��� ������ �� �Է�");
		return false;
	}

	//��Ƽ�� �������� 0���� ���Ͷ��
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
@return bool :  ���� �۵��� true, �ƴ� ��� false

@param partition_value : ���� ��ȯ ��
@param pos_partition : ��ȯ�� ���� ���� ��ȣ
@param bit_size_of_partition : ���� ũ��

@brief ���� ũ�� ������ ���� �� ���� �� ������ ��Ƽ�� �� �ش� pos�� ������ �� ��ȯ �Լ�

@todo ��Ʈ ����ũ�� 1��Ʈ������ �ƴ� ���� ��Ʈ�� ���ÿ� ��Ʈ ����ũ �Ѵٸ� ������ �� ������ �� ���̴�. �ٵ� ���� ��Ʈ ���� ��Ʈ ����ũ���� ����� ������ ������ �� �ɸ���.
@todo ���� 1byte���� Ȥ�� 4bit ���� ��Ʈ ����ũ ����� ��� 0xffff �Ǵ� 0xff�� ��Ʈ �����ڷ� �̵� �� ����ϸ� �ӵ��鿡�� ������ ���̴�
*/
bool GetPartitionValue(unsigned int *dest_data, unsigned int src_data, int pos_partition, int bit_size_of_partition) {

	//��Ƽ�� �� ����
	int cnt_partition = (sizeof(src_data) * 8) / bit_size_of_partition;
	int parition_data = 0;

	//���� ó�� : ���� ��Ƽ���� ��Ƽ�� ������ ��� ���
	if (0 > pos_partition || cnt_partition - 1 < pos_partition) {

		return false;
	}

	//��ȯ ��
	unsigned short dest_partiotion_value = 0;

	//��Ƽ�� �������� 0�������� ����
	//��Ƽ�� �� LSB ��Ʈ ���������� �̵�
	unsigned int src_offset = 1 << ( ( ( pos_partition ) * bit_size_of_partition) );
	unsigned int dest_offset = 1;

	//��Ƽ�� �� bit�� MSB���� �ݺ�
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

