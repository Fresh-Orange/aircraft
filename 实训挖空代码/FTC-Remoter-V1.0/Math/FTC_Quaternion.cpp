#include "FTC_Math.h"


// ���ظ���Ԫ���ĵ�Ч��ת�����е���������
void Quaternion::vector_gravity(Vector3f &v)
{
  v.x = 2*(q2*q4 - q1*q3);								
  v.y = 2*(q1*q2 + q3*q4);						  
  v.z = 1-2*(q2*q2 + q3*q3);
}

//��Ԫ����һ��
void Quaternion::normalize(void)
{
	float length; 
	length= pythagorous4(q1, q2, q3, q4);
	q1 /= length;
	q2 /= length;
	q3 /= length;
	q4 /= length;
}

//һ�����������������Ԫ��
void Quaternion::Runge_Kutta_1st(Vector3f &g, float deltaT)
{
  q1 += 0.5 * (-q2*g.x - q3*g.y - q4*g.z)* deltaT;
  q2 += 0.5 * (q1*g.x + q3*g.z - q4*g.y)* deltaT;
  q3 += 0.5 * (q1*g.y - q2*g.z + q4*g.x)* deltaT;
  q4 += 0.5 * (q1*g.z + q2*g.y - q3*g.x)* deltaT;	
}

//ŷ����ת��Ԫ��
void Quaternion::from_euler(float roll, float pitch, float yaw)
{
    float cr2 = cosf(roll*0.5f);
    float cp2 = cosf(pitch*0.5f);
    float cy2 = cosf(yaw*0.5f);
    float sr2 = sinf(roll*0.5f);
    float sp2 = sinf(pitch*0.5f);
    float sy2 = sinf(yaw*0.5f);

    q1 = cr2*cp2*cy2 + sr2*sp2*sy2;
    q2 = sr2*cp2*cy2 - cr2*sp2*sy2;
    q3 = cr2*sp2*cy2 + sr2*cp2*sy2;
    q4 = cr2*cp2*sy2 - sr2*sp2*cy2;
}

//��Ԫ��תŷ����
void Quaternion::to_euler(float *roll, float *pitch, float *yaw)
{
    if (roll) {
        *roll = degrees(atan2f(2.0f*(q1*q2 + q3*q4),1 - 2.0f*(q2*q2 + q3*q3)));
    }
    if (pitch) {
        // ʹ��safe_asin()������pitch�ӽ�90/-90ʱ�����
        *pitch = degrees(safe_asin(2.0f*(q1*q3 - q2*q4)));
    }
    if (yaw) {
        *yaw = degrees(atan2f(2.0f*(q2*q3 - q1*q4), 2.0f*(q1*q1 + q2*q2) - 1));
    }
}









