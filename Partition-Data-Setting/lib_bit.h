#pragma once

bool DisplayNumToBit(unsigned int); ///<10진수 값을 2진수 비트 단위로 출력한다.

bool SetBitFlag(unsigned int*, unsigned int, bool); ///<데이터 값의 2진 비트 플래그의 오프셋의 값을 변경한다.

bool CopyPartitionValueTo(unsigned int*, int, int, unsigned int); ///<데이터를 비트 크기로 파티션 분할 후 선택 파티션에 1바이트 값을 변경한다.

bool GetPartitionValue(unsigned int *, unsigned int, int, int);

