#pragma once
#include <time.h>
#include <string>

//C������time_t�ļ򵥷�װ
//��ȷ����
class CKTime
{
public:
	CKTime(void);
	CKTime(time_t time);
	CKTime(struct tm* timeptr);
	~CKTime(void);

	void SetTime(time_t time);
	void SetTime(struct tm* timeptr);
	/*����ʱ���ʽ�� ���� �� �� ʱ���֣��� ��*/
	std::string Date();
	/*��������ʱ��*/
	std::string GMDate();
	/*����ʱ��*/
	std::string LocalDate();
	
	double operator-(const CKTime& time);
	CKTime& operator=(const CKTime& time);

private:
	time_t m_time;
};


/*
1.GetTickCount:ͷ�ļ�windows.h, �õ�����ϵͳ���е�ʱ�䣬��ȷ�����롣

2.C++�У���C������struct tm��Ӧ�Ľṹ����SYSTEMTIME, ��ȡ��ʽGetLocalTime()��

3.��ȡ�߾���ʱ�䣬����ʹ��
BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency)��ȡϵͳ�ļ�������Ƶ��
BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount)��ȡ��������ֵ
Ȼ�������μ������Ĳ����Frequency�͵õ�ʱ�䡣
e.g
    LARGE_INTEGER m_nFreq;  
    LARGE_INTEGER m_nBeginTime;  
    LARGE_INTEGER nEndTime;  
    QueryPerformanceFrequency(&m_nFreq); // ��ȡʱ������   
    QueryPerformanceCounter(&m_nBeginTime); // ��ȡʱ�Ӽ���   
    Sleep(100);  
    QueryPerformanceCounter(&nEndTime);  
    cout << (double)(nEndTime.QuadPart-m_nBeginTime.QuadPart)*1000/m_nFreq.QuadPart << endl;

4.timeGetTime();��ȷ�����룬��GetTickCount�൱��
*/
