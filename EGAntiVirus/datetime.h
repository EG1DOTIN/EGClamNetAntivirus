#ifndef DATE_TIME_HEADER
#define DATE_TIME_HEADER

#include <wx\wx.h>

void GetSystemDate(wxString& date); //e.g. 04-Apr-2018
int GetNumberOfDaysBtTwoDates(const wxDateTime& dt1, const wxDateTime& dt2);
int GetNumberOfDaysBtTwoDates(unsigned short day1, unsigned short month1, unsigned short year1, unsigned short day2, unsigned short month2, unsigned short year2);
int GetNumberOfDaysFromToday(unsigned short day, unsigned short month, unsigned short year);

struct OnlyDateStruct
{
	unsigned short m_day;
	unsigned short m_month;
	unsigned short m_year;
};

void ParseDateInOnlyDateStruct(const wxString& DateInSlashFormat, OnlyDateStruct& OutOnlyDateformat);

#endif