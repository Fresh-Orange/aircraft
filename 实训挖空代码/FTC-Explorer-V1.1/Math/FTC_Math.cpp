#include "FTC_Math.h"

//��֤����ֵ����Ч��
float safe_asin(float v)
{
    if (isnan(v)) {
        return 0.0;
    }
    if (v >= 1.0f) {
        return M_PI/2;
    }
    if (v <= -1.0f) {
        return -M_PI/2;
    }
    return asinf(v);
}

//�������޷�
float constrain_float(float amt, float low, float high) 
{
	if (isnan(amt)) {
		return (low+high)*0.5f;
	}
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

//16λ�������޷�
int16_t constrain_int16(int16_t amt, int16_t low, int16_t high) {
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

//16λ�޷����������޷�
uint16_t constrain_uint16(uint16_t amt, uint16_t low, uint16_t high)
{
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

//32λ�������޷�
int32_t constrain_int32(int32_t amt, int32_t low, int32_t high) {
	return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

int32_t applyDeadband(int32_t value, int32_t deadband)
{
    if (abs(value) < deadband) {
        value = 0;
    } else if (value > 0) {
        value -= deadband;
    } else if (value < 0) {
        value += deadband;
    }
    return value;
}

//�Ƕ�ת����
float radians(float deg) {
	return deg * DEG_TO_RAD;
}

//����ת�Ƕ�
float degrees(float rad) {
	return rad * RAD_TO_DEG;
}

//��ƽ��
float sq(float v) {
	return v*v;
}

//2ά��������
float pythagorous2(float a, float b) {
	return sqrtf(sq(a)+sq(b));
}

//3ά��������
float pythagorous3(float a, float b, float c) {
	return sqrtf(sq(a)+sq(b)+sq(c));
}

//4ά��������
float pythagorous4(float a, float b, float c, float d) {
	return sqrtf(sq(a)+sq(b)+sq(c)+sq(d));
}











