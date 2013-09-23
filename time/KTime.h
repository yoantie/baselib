#pragma once
#include <time.h>
#include <string>

//C语言中time_t的简单封装
//精确到秒
class CKTime
{
public:
	CKTime(void);
	CKTime(time_t time);
	CKTime(struct tm* timeptr);
	~CKTime(void);

	void SetTime(time_t time);
	void SetTime(struct tm* timeptr);
	/*常用时间格式： 星期 月 日 时：分：秒 年*/
	std::string Date();
	/*格林威治时间*/
	std::string GMDate();
	/*本地时间*/
	std::string LocalDate();
	
	double operator-(const CKTime& time);
	CKTime& operator=(const CKTime& time);

private:
	time_t m_time;
};


/*
1.GetTickCount:头文件windows.h, 得到的是系统运行的时间，精确到毫秒。

2.C++中，与C语言中struct tm对应的结构体是SYSTEMTIME, 获取方式GetLocalTime()。

3.获取高精度时间，可以使用
BOOL QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency)获取系统的计数器的频率
BOOL QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount)获取计数器的值
然后用两次计数器的差除以Frequency就得到时间。
e.g
    LARGE_INTEGER m_nFreq;  
    LARGE_INTEGER m_nBeginTime;  
    LARGE_INTEGER nEndTime;  
    QueryPerformanceFrequency(&m_nFreq); // 获取时钟周期   
    QueryPerformanceCounter(&m_nBeginTime); // 获取时钟计数   
    Sleep(100);  
    QueryPerformanceCounter(&nEndTime);  
    cout << (double)(nEndTime.QuadPart-m_nBeginTime.QuadPart)*1000/m_nFreq.QuadPart << endl;

4.timeGetTime();精确到毫秒，与GetTickCount相当。
*/
