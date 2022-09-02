#include "datetime.h"

void GetSystemDate(wxString& date) //e.g. 04-Apr-2018
{
	SYSTEMTIME loc_time_ll;
	GetLocalTime(&loc_time_ll);
	wxDateTime loc_dt(loc_time_ll);
	date = loc_dt.Format(wxT("%d-%b-%Y"));
}

int GetNumberOfDaysBtTwoDates(const wxDateTime& dt1, const wxDateTime& dt2)
{
	return (dt2 - dt1).GetDays();
}

int GetNumberOfDaysBtTwoDates(unsigned short day1, unsigned short month1, unsigned short year1, unsigned short day2, unsigned short month2, unsigned short year2)
{
	if ((day1 < 1 || day1 > 31) || ((day2 < 1 || day2 > 31)) || (month1 < 1 || month1 > 12) || (month2 < 1 || month2 > 12))
		return -1;

	wxDateTime date1(day1, wxDateTime::Month(month1 - 1), year1, 0, 0, 0, 0), date2(day2, wxDateTime::Month(month2 - 1), year2, 0, 0, 0, 0);

	return GetNumberOfDaysBtTwoDates(date1, date2);
}

int GetNumberOfDaysFromToday(unsigned short day, unsigned short month, unsigned short year)
{
	if ((day < 1 || day > 31) || (month < 1 || month > 12))
		return -1;

	wxDateTime d1(day, wxDateTime::Month(month - 1), year, 0, 0, 0, 0), d2;

	d2 = wxDateTime::GetTimeNow();

	return GetNumberOfDaysBtTwoDates(d1, d2);
}

void ParseDateInOnlyDateStruct(const wxString& DateInSlashFormat, OnlyDateStruct& OutOnlyDateformat)
{
	wxString day, month, year;
	ULONG ul_day, ul_month, ul_year;

	day = DateInSlashFormat.BeforeFirst('/');
	if (day == DateInSlashFormat)
	{
		day = DateInSlashFormat.BeforeFirst('\\');
		month = DateInSlashFormat.BeforeLast('\\').AfterFirst('\\');
		year = DateInSlashFormat.AfterLast('\\');

		day.ToULong(&ul_day);
		month.ToULong(&ul_month);
		year.ToULong(&ul_year);

		OutOnlyDateformat.m_day = (unsigned short)(ul_day);
		OutOnlyDateformat.m_month = (unsigned short)(ul_month);
		OutOnlyDateformat.m_year = (unsigned short)(ul_year);
		return;
	}

	month = DateInSlashFormat.BeforeLast('/').AfterFirst('/');
	year = DateInSlashFormat.AfterLast('/');

	day.ToULong(&ul_day);
	month.ToULong(&ul_month);
	year.ToULong(&ul_year);

	OutOnlyDateformat.m_day = ul_day;
	OutOnlyDateformat.m_month = ul_month;
	OutOnlyDateformat.m_year = ul_year;
	return;
}