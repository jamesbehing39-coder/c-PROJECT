#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>

#include <algorithm>
#include <array>
#include <cwctype>
#include <cwchar>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Comctl32.lib")

namespace
{
constexpr wchar_t WindowClassName[] = L"StudentRecordSystemWindow";
constexpr wchar_t WindowTitle[] = L"Student Record Management System";

// Futuristic neon blue / cyberpunk palette
constexpr COLORREF Background = RGB(5, 11, 24);         // Deep Midnight Navy
constexpr COLORREF SidebarBackground = RGB(10, 20, 40); // Dark Navy
constexpr COLORREF Surface = RGB(16, 31, 56);           // Panel
constexpr COLORREF SurfaceLight = RGB(22, 42, 72);
constexpr COLORREF SurfaceDeep = RGB(12, 24, 46);
constexpr COLORREF Accent = RGB(8, 123, 255);           // Electric Blue
constexpr COLORREF AccentBright = RGB(0, 229, 255);     // Neon Cyan
constexpr COLORREF AccentIce = RGB(128, 245, 255);      // Ice Blue
constexpr COLORREF TextPrimary = RGB(244, 248, 255);
constexpr COLORREF TextSecondary = RGB(169, 189, 213);
constexpr COLORREF Border = RGB(22, 77, 120);
constexpr COLORREF BorderSoft = RGB(30, 66, 102);
constexpr COLORREF InputBackground = RGB(10, 20, 40);
constexpr COLORREF InputBorder = RGB(74, 104, 138);      // thin blue-gray
constexpr COLORREF InputBorderHover = RGB(98, 148, 196);
constexpr COLORREF InputBorderFocus = AccentBright;      // neon cyan
constexpr COLORREF InputGlow = RGB(0, 120, 160);
constexpr COLORREF Success = RGB(57, 255, 166);
constexpr COLORREF Warning = RGB(255, 191, 94);
constexpr COLORREF Danger = RGB(255, 92, 122);

constexpr int SidebarWidth = 248;
constexpr int NavigationTop = 144;
constexpr int NavigationHeight = 48;
constexpr int NavigationGap = 8;
constexpr int InputHeight = 42;
constexpr int InputRadius = 7;

enum ControlId
{
	StudentIdControl = 1001,
	FirstNameControl,
	LastNameControl,
	MiddleNameControl,
	CourseControl,
	YearLevelControl,
	SectionControl,
	EmailControl,
	SaveStudentButton,
	ClearFormButton,
	NotificationControl,
	StudentSearchControl,
	RefreshStudentsButton,
	UpdateSearchControl,
	FindStudentButton,
	UpdateStudentButton,
	CancelUpdateButton,
	UpdateNotificationControl,
	DeleteSearchControl,
	FindDeleteStudentButton,
	DeleteStudentButton,
	CancelDeleteButton,
	DeleteNotificationControl,
	GradeSearchControl,
	FindGradeStudentButton,
	SubjectControl,
	UnitsControl,
	GradeControl,
	SaveGradeButton,
	ClearGradeButton,
	ThresholdControl,
	ApplyThresholdButton,
	GradeNotificationControl,
	CalculateGwaButton,
	ReportSearchControl,
	FindReportStudentButton,
	SchoolNameControl,
	ExportReportButton,
	ReportNotificationControl
};

struct ReportState
{
	HWND search = nullptr;
	HWND findButton = nullptr;
	HWND schoolName = nullptr;
	HWND exportButton = nullptr;
	HWND notification = nullptr;
	bool notificationError = false;
	int selectedStudentIndex = -1;
	bool hasGwa = false;
	double totalUnits = 0.0;
	double gwa = 0.0;
};

struct Grade
{
	std::wstring subject;
	double units = 0.0;
	double grade = 0.0;
};

struct GradeState
{
	HWND search = nullptr;
	HWND findButton = nullptr;
	HWND subject = nullptr;
	HWND units = nullptr;
	HWND grade = nullptr;
	HWND saveButton = nullptr;
	HWND clearButton = nullptr;
	HWND threshold = nullptr;
	HWND applyThresholdButton = nullptr;
	HWND calculateGwaButton = nullptr;
	HWND notification = nullptr;
	bool notificationError = false;
	int selectedStudentIndex = -1;
	int editingGradeIndex = -1;
	double passingThreshold = 3.0;
	bool hasGwa = false;
	double totalUnits = 0.0;
	double gwa = 0.0;
};

struct DeleteState
{
	HWND search = nullptr;
	HWND findButton = nullptr;
	HWND studentId = nullptr;
	HWND firstName = nullptr;
	HWND lastName = nullptr;
	HWND middleName = nullptr;
	HWND course = nullptr;
	HWND yearLevel = nullptr;
	HWND section = nullptr;
	HWND email = nullptr;
	HWND deleteButton = nullptr;
	HWND cancelButton = nullptr;
	HWND notification = nullptr;
	bool notificationError = false;
	int selectedIndex = -1;
};

struct UpdateState
{
	HWND search = nullptr;
	HWND findButton = nullptr;
	HWND studentId = nullptr;
	HWND firstName = nullptr;
	HWND lastName = nullptr;
	HWND middleName = nullptr;
	HWND course = nullptr;
	HWND yearLevel = nullptr;
	HWND section = nullptr;
	HWND email = nullptr;
	HWND updateButton = nullptr;
	HWND cancelButton = nullptr;
	HWND notification = nullptr;
	bool notificationError = false;
	int selectedIndex = -1;
};

struct ViewState
{
	HWND search = nullptr;
	HWND refreshButton = nullptr;
};

class Student
{
public:
	std::wstring studentId;
	std::wstring firstName;
	std::wstring lastName;
	std::wstring middleName;
	std::wstring course;
	std::wstring yearLevel;
	std::wstring section;
	std::wstring email;
	std::vector<Grade> grades;
};

struct FormState
{
	HWND studentId = nullptr;
	HWND firstName = nullptr;
	HWND lastName = nullptr;
	HWND middleName = nullptr;
	HWND course = nullptr;
	HWND yearLevel = nullptr;
	HWND section = nullptr;
	HWND email = nullptr;
	HWND saveButton = nullptr;
	HWND clearButton = nullptr;
	HWND notification = nullptr;
	bool notificationError = false;
};

struct AppState
{
	HFONT titleFont = nullptr;
	HFONT headingFont = nullptr;
	HFONT bodyFont = nullptr;
	HFONT smallFont = nullptr;
	int hoveredNavigation = -1;
	int activeNavigation = 0;
	bool trackingMouse = false;
	std::array<RECT, 8> navigationRects{};
	HWND hoveredInput = nullptr;
	HWND focusedInput = nullptr;
	HWND inputTracking = nullptr;
	std::unordered_map<HWND, WNDPROC> inputOriginalProcs;
	FormState form;
	ViewState view;
	UpdateState update;
	DeleteState remove;
	GradeState grade;
	ReportState report;
};

AppState app;
std::vector<Student> students;

constexpr wchar_t StudentsFileName[] = L"students.txt";
constexpr wchar_t GradesFileName[] = L"grades.txt";
constexpr wchar_t StudentsTempFileName[] = L"students.txt.tmp";
constexpr wchar_t GradesTempFileName[] = L"grades.txt.tmp";

constexpr std::array<const wchar_t*, 10> CourseOptions{
	L"Bachelor of Science in Information Systems (BSIS)",
	L"Bachelor of Science in Information Technology (BSIT)",
	L"Bachelor of Science in Computer Science (BSCS)",
	L"Bachelor of Arts (BA)",
	L"Bachelor of Science in Hospitality Management (BSHM / HRM)",
	L"Engineering (ENGINEERING)",
	L"Education (EDUC)",
	L"Bachelor of Science in Criminology (BSCRIM)",
	L"Bachelor of Science in Nursing (BSN)",
	L"Bachelor of Fine Arts (BFA / FA)"
};

constexpr std::array<const wchar_t*, 4> YearLevelOptions{
	L"1st Year", L"2nd Year", L"3rd Year", L"4th Year"
};

void PopulateAcademicComboBoxes(HWND courseControl, HWND yearLevelControl)
{
	const DWORD comboStyle = WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS;
	SetWindowLongPtrW(courseControl, GWL_STYLE, GetWindowLongPtrW(courseControl, GWL_STYLE) | comboStyle);
	SetWindowLongPtrW(yearLevelControl, GWL_STYLE, GetWindowLongPtrW(yearLevelControl, GWL_STYLE) | comboStyle);
	for (const wchar_t* course : CourseOptions)
	{
		SendMessageW(courseControl, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(course));
	}
	for (const wchar_t* yearLevel : YearLevelOptions)
	{
		SendMessageW(yearLevelControl, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(yearLevel));
	}
}

void SelectComboValue(HWND control, const std::wstring& value)
{
	const LRESULT index = SendMessageW(control, CB_FINDSTRINGEXACT, static_cast<WPARAM>(-1),
		reinterpret_cast<LPARAM>(value.c_str()));
	if (index != CB_ERR)
	{
		SendMessageW(control, CB_SETCURSEL, static_cast<WPARAM>(index), 0);
		return;
	}
	if (!value.empty())
	{
		const LRESULT added = SendMessageW(control, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(value.c_str()));
		SendMessageW(control, CB_SETCURSEL, static_cast<WPARAM>(added), 0);
	}
}

std::vector<std::wstring> SplitStorageLine(const std::wstring& line)
{
	std::vector<std::wstring> fields;
	size_t start = 0;
	while (true)
	{
		const size_t separator = line.find(L'\t', start);
		fields.push_back(line.substr(start, separator == std::wstring::npos ? separator : separator - start));
		if (separator == std::wstring::npos)
		{
			break;
		}
		start = separator + 1;
	}
	return fields;
}

bool ParseStorageDouble(const std::wstring& text, double& value)
{
	if (text.empty())
	{
		return false;
	}
	wchar_t* end = nullptr;
	value = wcstod(text.c_str(), &end);
	return end != text.c_str() && *end == L'\0' && std::isfinite(value);
}

std::wstring SafeStorageValue(const std::wstring& value)
{
	std::wstring safe = value;
	for (wchar_t& character : safe)
	{
		if (character == L'\t' || character == L'\r' || character == L'\n')
		{
			character = L' ';
		}
	}
	return safe;
}

std::wstring StorageLowercase(std::wstring value)
{
	std::transform(value.begin(), value.end(), value.begin(), [](wchar_t character) {
		return static_cast<wchar_t>(towlower(character));
	});
	return value;
}

bool SaveData()
{
	std::wofstream studentFile(StudentsTempFileName, std::ios::trunc);
	std::wofstream gradeFile(GradesTempFileName, std::ios::trunc);
	if (!studentFile || !gradeFile)
	{
		return false;
	}
	for (const Student& student : students)
	{
		studentFile << SafeStorageValue(student.studentId) << L'\t'
			<< SafeStorageValue(student.firstName) << L'\t'
			<< SafeStorageValue(student.lastName) << L'\t'
			<< SafeStorageValue(student.middleName) << L'\t'
			<< SafeStorageValue(student.course) << L'\t'
			<< SafeStorageValue(student.yearLevel) << L'\t'
			<< SafeStorageValue(student.section) << L'\t'
			<< SafeStorageValue(student.email) << L'\n';
		for (const Grade& grade : student.grades)
		{
			gradeFile << SafeStorageValue(student.studentId) << L'\t'
				<< SafeStorageValue(grade.subject) << L'\t'
				<< grade.units << L'\t' << grade.grade << L'\n';
		}
	}
	studentFile.close();
	gradeFile.close();
	if (!studentFile || !gradeFile)
	{
		DeleteFileW(StudentsTempFileName);
		DeleteFileW(GradesTempFileName);
		return false;
	}
	if (!MoveFileExW(StudentsTempFileName, StudentsFileName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)
		|| !MoveFileExW(GradesTempFileName, GradesFileName, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
	{
		DeleteFileW(StudentsTempFileName);
		DeleteFileW(GradesTempFileName);
		return false;
	}
	return true;
}

void LoadData()
{
	students.clear();
	std::wifstream studentFile(StudentsFileName);
	std::wstring line;
	while (std::getline(studentFile, line))
	{
		const std::vector<std::wstring> fields = SplitStorageLine(line);
		if (fields.size() != 8 || fields[0].empty())
		{
			continue;
		}
		const bool duplicate = std::any_of(students.begin(), students.end(), [&](const Student& student) {
			return student.studentId == fields[0];
		});
		if (duplicate)
		{
			continue;
		}
		Student student;
		student.studentId = fields[0];
		student.firstName = fields[1];
		student.lastName = fields[2];
		student.middleName = fields[3];
		student.course = fields[4];
		student.yearLevel = fields[5];
		student.section = fields[6];
		student.email = fields[7];
		students.push_back(std::move(student));
	}

	std::wifstream gradeFile(GradesFileName);
	while (std::getline(gradeFile, line))
	{
		const std::vector<std::wstring> fields = SplitStorageLine(line);
		if (fields.size() != 4 || fields[0].empty() || fields[1].empty())
		{
			continue;
		}
		double units = 0.0;
		double gradeValue = 0.0;
		if (!ParseStorageDouble(fields[2], units) || !ParseStorageDouble(fields[3], gradeValue)
			|| units <= 0.0 || gradeValue < 1.0 || gradeValue > 5.0)
		{
			continue;
		}
		const auto student = std::find_if(students.begin(), students.end(), [&](const Student& item) {
			return item.studentId == fields[0];
		});
		if (student == students.end())
		{
			continue;
		}
		const std::wstring subject = StorageLowercase(fields[1]);
		const bool duplicate = std::any_of(student->grades.begin(), student->grades.end(), [&](const Grade& item) {
			return StorageLowercase(item.subject) == subject;
		});
		if (!duplicate)
		{
			student->grades.push_back(Grade{ fields[1], units, gradeValue });
		}
	}
}

HFONT CreateUiFont(int height, int weight, const wchar_t* face = L"Segoe UI")
{
	return CreateFontW(height, 0, 0, 0, weight, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,
		face);
}

void DrawTextBlock(HDC dc, const wchar_t* text, RECT rect, HFONT font, COLORREF color,
	UINT format = DT_LEFT | DT_VCENTER | DT_SINGLELINE);
void FillRoundedRect(HDC dc, RECT rect, COLORREF color, int radius = 10);
void DrawRoundedBorder(HDC dc, RECT rect, COLORREF color, int radius = 10);
void FillGradientRect(HDC dc, RECT rect, COLORREF top, COLORREF bottom);
void DrawGlowRect(HDC dc, RECT rect, COLORREF glow, int radius = 10);
std::wstring FormatDecimal(double value);

void DrawAddStudent(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(24, std::min(44, contentWidth / 18));
	const int left = contentLeft + padding;
	const int right = client.right - padding;

	RECT background{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &background, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 28, right, 46 };
	DrawTextBlock(dc, L"STUDENT RECORDS", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 48, right, 86 };
	DrawTextBlock(dc, L"Add Student", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 88, right, 112 };
	DrawTextBlock(dc, L"Register a new student in the academic records system.", subtitle, app.bodyFont, TextSecondary);

	RECT formPanel{ left, 126, right, std::min<LONG>(client.bottom - 18, 126 + 348) };
	FillGradientRect(dc, formPanel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, formPanel, Border, 12);
	RECT formTitle{ left + 22, 140, right - 22, 166 };
	DrawTextBlock(dc, L"Student Information", formTitle, app.headingFont, AccentBright);

	const int gap = 18;
	const int columnWidth = std::max(180, (right - left - gap) / 2);
	const int top = 176;
	const int rowHeight = 70;
	const std::array<const wchar_t*, 8> labels{
		L"Student ID", L"First Name", L"Last Name", L"Middle Name",
		L"Course", L"Year Level", L"Section", L"Email Address"
	};
	for (int i = 0; i < static_cast<int>(labels.size()); ++i)
	{
		const int column = i % 2;
		const int row = i / 2;
		const int x = left + column * (columnWidth + gap);
		RECT labelRect{ x + 2, top + row * rowHeight, x + columnWidth, top + row * rowHeight + 18 };
		DrawTextBlock(dc, labels[i], labelRect, app.smallFont, TextSecondary);
	}
}

void DrawTextBlock(HDC dc, const wchar_t* text, RECT rect, HFONT font, COLORREF color,
	UINT format)
{
	SetBkMode(dc, TRANSPARENT);
	SetTextColor(dc, color);
	SelectObject(dc, font);
	DrawTextW(dc, text, -1, &rect, format);
}

void FillRoundedRect(HDC dc, RECT rect, COLORREF color, int radius)
{
	if (rect.right <= rect.left || rect.bottom <= rect.top) return;
	HBRUSH brush = CreateSolidBrush(color);
	HGDIOBJ oldBrush = SelectObject(dc, brush);
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HGDIOBJ oldPen = SelectObject(dc, pen);
	RoundRect(dc, rect.left, rect.top, rect.right, rect.bottom, radius, radius);
	SelectObject(dc, oldPen);
	SelectObject(dc, oldBrush);
	DeleteObject(pen);
	DeleteObject(brush);
}

void DrawRoundedBorder(HDC dc, RECT rect, COLORREF color, int radius)
{
	if (rect.right <= rect.left || rect.bottom <= rect.top) return;
	HGDIOBJ oldBrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
	HPEN pen = CreatePen(PS_SOLID, 1, color);
	HGDIOBJ oldPen = SelectObject(dc, pen);
	RoundRect(dc, rect.left, rect.top, rect.right, rect.bottom, radius, radius);
	SelectObject(dc, oldPen);
	SelectObject(dc, oldBrush);
	DeleteObject(pen);
}

void FillGradientRect(HDC dc, RECT rect, COLORREF top, COLORREF bottom)
{
	if (rect.right <= rect.left || rect.bottom <= rect.top) return;
	TRIVERTEX vertices[2]{
		{ rect.left, rect.top, static_cast<COLOR16>(GetRValue(top) << 8), static_cast<COLOR16>(GetGValue(top) << 8), static_cast<COLOR16>(GetBValue(top) << 8), 0xff00 },
		{ rect.right, rect.bottom, static_cast<COLOR16>(GetRValue(bottom) << 8), static_cast<COLOR16>(GetGValue(bottom) << 8), static_cast<COLOR16>(GetBValue(bottom) << 8), 0xff00 }
	};
	GRADIENT_RECT g{ 0, 1 };
	GradientFill(dc, vertices, 2, &g, 1, GRADIENT_FILL_RECT_V);
}

void DrawGlowRect(HDC dc, RECT rect, COLORREF glow, int radius)
{
	for (int i = 2; i >= 1; --i)
	{
		RECT g{ rect.left - i, rect.top - i, rect.right + i, rect.bottom + i };
		DrawRoundedBorder(dc, g, RGB(
			(GetRValue(glow) * (3 - i) + GetRValue(Background) * i) / 3,
			(GetGValue(glow) * (3 - i) + GetGValue(Background) * i) / 3,
			(GetBValue(glow) * (3 - i) + GetBValue(Background) * i) / 3), radius + i);
	}
}

void DrawNavigationButton(HDC dc, const wchar_t* text, const RECT& rect, bool active, bool hovered)
{
	if (active)
	{
		FillGradientRect(dc, rect, RGB(12, 58, 110), RGB(8, 38, 78));
		DrawRoundedBorder(dc, rect, Border, 9);
		RECT indicator{ rect.left, rect.top + 9, rect.left + 4, rect.bottom - 9 };
		FillRoundedRect(dc, indicator, AccentBright, 3);
		RECT dot{ rect.left + 16, (rect.top + rect.bottom) / 2 - 3, rect.left + 22, (rect.top + rect.bottom) / 2 + 3 };
		FillRoundedRect(dc, dot, AccentIce, 3);
	}
	else if (hovered)
	{
		FillRoundedRect(dc, rect, RGB(18, 38, 68), 9);
		DrawRoundedBorder(dc, rect, BorderSoft, 9);
	}
	RECT textRect = rect;
	textRect.left += 34;
	DrawTextBlock(dc, text, textRect, app.bodyFont, active ? TextPrimary : (hovered ? RGB(214, 232, 255) : TextSecondary));
}

void DrawStatCard(HDC dc, RECT rect, const wchar_t* label, const wchar_t* value, COLORREF marker)
{
	FillGradientRect(dc, rect, SurfaceLight, Surface);
	DrawRoundedBorder(dc, rect, Border, 12);
	RECT markerRect{ rect.left + 18, rect.top + 20, rect.left + 23, rect.top + 54 };
	FillRoundedRect(dc, markerRect, marker, 2);
	RECT accent{ rect.left + 1, rect.top + 1, rect.right - 1, rect.top + 3 };
	HBRUSH ab = CreateSolidBrush(marker);
	FillRect(dc, &accent, ab);
	DeleteObject(ab);

	RECT labelRect{ rect.left + 38, rect.top + 16, rect.right - 16, rect.top + 38 };
	DrawTextBlock(dc, label, labelRect, app.smallFont, TextSecondary);
	RECT valueRect{ rect.left + 38, rect.top + 38, rect.right - 16, rect.bottom - 12 };
	DrawTextBlock(dc, value, valueRect, app.headingFont, TextPrimary);
}

void ModernizeInputControl(HWND control, bool isCombo);


HWND CreateFormEdit(HWND parent, int id)
{
	HWND control = CreateWindowExW(0, L"EDIT", L"", WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,
		0, 0, 0, 0, parent, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), nullptr, nullptr);
	SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	ModernizeInputControl(control, false);
	return control;
}

void ApplyControlFont(HWND control, HFONT font)
{
	if (control != nullptr)
	{
		SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(font), TRUE);
	}
}

HWND CreateFormLabel(HWND parent, const wchar_t* text)
{
	HWND label = CreateWindowExW(0, L"STATIC", text, WS_CHILD | SS_LEFT,
		0, 0, 0, 0, parent, nullptr, nullptr, nullptr);
	SendMessageW(label, WM_SETFONT, reinterpret_cast<WPARAM>(app.smallFont), TRUE);
	return label;
}

bool IsManagedInput(HWND control)
{
	return control != nullptr && app.inputOriginalProcs.find(control) != app.inputOriginalProcs.end();
}

void InvalidateInputChrome(HWND control)
{
	if (control == nullptr) return;
	HWND parent = GetParent(control);
	if (parent == nullptr) return;
	RECT rc{};
	GetWindowRect(control, &rc);
	MapWindowPoints(nullptr, parent, reinterpret_cast<POINT*>(&rc), 2);
	// Invalidate only the chrome band around the control (never erase into the control's text/caret area).
	const int pad = 4;
	RECT top{ rc.left - pad, rc.top - pad, rc.right + pad, rc.top + 2 };
	RECT bottom{ rc.left - pad, rc.bottom - 2, rc.right + pad, rc.bottom + pad };
	RECT left{ rc.left - pad, rc.top - pad, rc.left + 2, rc.bottom + pad };
	RECT right{ rc.right - 2, rc.top - pad, rc.right + pad, rc.bottom + pad };
	InvalidateRect(parent, &top, FALSE);
	InvalidateRect(parent, &bottom, FALSE);
	InvalidateRect(parent, &left, FALSE);
	InvalidateRect(parent, &right, FALSE);
}

void DrawInputChrome(HDC parentDc, HWND control)
{
	if (parentDc == nullptr || control == nullptr || !IsWindow(control) || !IsWindowVisible(control)) return;
	HWND parent = GetParent(control);
	if (parent == nullptr) return;
	RECT rc{};
	if (!GetWindowRect(control, &rc)) return;
	MapWindowPoints(nullptr, parent, reinterpret_cast<POINT*>(&rc), 2);
	// Frame sits outside the native control; clip excludes the control rect so native paint/text/caret/selection are never overdrawn.
	RECT frame = rc;
	InflateRect(&frame, 2, 2);

	const bool focused = (app.focusedInput == control) || (GetFocus() == control);
	const bool hovered = (app.hoveredInput == control);
	COLORREF border = InputBorder;
	if (focused) border = InputBorderFocus;
	else if (hovered) border = InputBorderHover;

	const int saved = SaveDC(parentDc);
	ExcludeClipRect(parentDc, rc.left, rc.top, rc.right, rc.bottom);
	HBRUSH fill = CreateSolidBrush(InputBackground);
	HGDIOBJ oldb = SelectObject(parentDc, fill);
	HPEN pen = CreatePen(PS_SOLID, 1, border);
	HGDIOBJ oldp = SelectObject(parentDc, pen);
	RoundRect(parentDc, frame.left, frame.top, frame.right, frame.bottom, InputRadius * 2, InputRadius * 2);
	SelectObject(parentDc, oldp);
	SelectObject(parentDc, oldb);
	DeleteObject(pen);
	DeleteObject(fill);

	if (focused)
	{
		HPEN gp = CreatePen(PS_SOLID, 1, InputGlow);
		HGDIOBJ oldpg = SelectObject(parentDc, gp);
		HGDIOBJ oldbg = SelectObject(parentDc, GetStockObject(NULL_BRUSH));
		RECT g = frame;
		InflateRect(&g, 1, 1);
		RoundRect(parentDc, g.left, g.top, g.right, g.bottom, InputRadius * 2 + 2, InputRadius * 2 + 2);
		SelectObject(parentDc, oldpg);
		SelectObject(parentDc, oldbg);
		DeleteObject(gp);
	}
	RestoreDC(parentDc, saved);
	// Native EDIT/COMBOBOX paints itself (no WM_PRINTCLIENT onto parent DC).
}

void DrawManagedInputChrome(HDC parentDc)
{
	if (parentDc == nullptr) return;
	for (const auto& entry : app.inputOriginalProcs)
	{
		DrawInputChrome(parentDc, entry.first);
	}
}

LRESULT CALLBACK ModernInputSubclassProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,
	UINT_PTR, DWORD_PTR)
{
	switch (msg)
	{
	case WM_NCDESTROY:
		RemoveWindowSubclass(hwnd, ModernInputSubclassProc, 1);
		app.inputOriginalProcs.erase(hwnd);
		if (app.hoveredInput == hwnd) app.hoveredInput = nullptr;
		if (app.focusedInput == hwnd) app.focusedInput = nullptr;
		if (app.inputTracking == hwnd) app.inputTracking = nullptr;
		break;
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		// Track focus state, let native EDIT/COMBO handle rendering, then invalidate only the chrome band.
		app.focusedInput = (msg == WM_SETFOCUS) ? hwnd : nullptr;
		{
			LRESULT fr = DefSubclassProc(hwnd, msg, wParam, lParam);
			InvalidateInputChrome(hwnd);
			return fr;
		}
	case WM_MOUSEMOVE:
		// Update hover/invalidate only when hover target actually changes (no repaint on every move inside the same control).
		if (app.hoveredInput != hwnd)
		{
			HWND old = app.hoveredInput;
			app.hoveredInput = hwnd;
			if (old != nullptr) InvalidateInputChrome(old);
			InvalidateInputChrome(hwnd);
		}
		if (app.inputTracking != hwnd)
		{
			TRACKMOUSEEVENT tme{ sizeof(tme), TME_LEAVE, hwnd, 0 };
			if (TrackMouseEvent(&tme))
			{
				app.inputTracking = hwnd;
			}
		}
		break;
	case WM_MOUSELEAVE:
		if (app.inputTracking == hwnd) app.inputTracking = nullptr;
		if (app.hoveredInput == hwnd)
		{
			app.hoveredInput = nullptr;
			InvalidateInputChrome(hwnd); // border-band regions only (FALSE erase inside helper)
		}
		break;
	default:
		break;
	}
	// All paint/nonclient messages (including WM_PAINT/WM_NCPAINT) go to the native control via DefSubclassProc.
	return DefSubclassProc(hwnd, msg, wParam, lParam);
}

void ModernizeInputControl(HWND control, bool isCombo = false)
{
	if (control == nullptr) return;
	LONG_PTR ex = GetWindowLongPtrW(control, GWL_EXSTYLE);
	ex &= ~static_cast<LONG_PTR>(WS_EX_CLIENTEDGE);
	SetWindowLongPtrW(control, GWL_EXSTYLE, ex);
	SetWindowPos(control, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOACTIVATE);
	if (app.inputOriginalProcs.find(control) == app.inputOriginalProcs.end())
	{
		SetWindowSubclass(control, ModernInputSubclassProc, 1, 0);
		app.inputOriginalProcs[control] = reinterpret_cast<WNDPROC>(1); // presence marker
	}
	if (!isCombo)
	{
		SendMessageW(control, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, MAKELPARAM(12, 12));
	}
	else
	{
		// Improve dropdown list row height/readability (CBS_DROPDOWNLIST)
		SendMessageW(control, CB_SETITEMHEIGHT, static_cast<WPARAM>(-1), 30);
		SendMessageW(control, CB_SETITEMHEIGHT, 0, 28);
	}
	InvalidateInputChrome(control);
}

void SetFormVisible(bool visible)
{
	const int command = visible ? SW_SHOW : SW_HIDE;
	ShowWindow(app.form.studentId, command);
	ShowWindow(app.form.firstName, command);
	ShowWindow(app.form.lastName, command);
	ShowWindow(app.form.middleName, command);
	ShowWindow(app.form.course, command);
	ShowWindow(app.form.yearLevel, command);
	ShowWindow(app.form.section, command);
	ShowWindow(app.form.email, command);
	ShowWindow(app.form.saveButton, command);
	ShowWindow(app.form.clearButton, command);
	ShowWindow(app.form.notification, command);
}

void SetViewVisible(bool visible)
{
	const int command = visible ? SW_SHOW : SW_HIDE;
	ShowWindow(app.view.search, command);
	ShowWindow(app.view.refreshButton, command);
}

void SetUpdateVisible(bool visible)
{
	const int command = visible ? SW_SHOW : SW_HIDE;
	ShowWindow(app.update.search, command);
	ShowWindow(app.update.findButton, command);
	ShowWindow(app.update.studentId, command);
	ShowWindow(app.update.firstName, command);
	ShowWindow(app.update.lastName, command);
	ShowWindow(app.update.middleName, command);
	ShowWindow(app.update.course, command);
	ShowWindow(app.update.yearLevel, command);
	ShowWindow(app.update.section, command);
	ShowWindow(app.update.email, command);
	ShowWindow(app.update.updateButton, command);
	ShowWindow(app.update.cancelButton, command);
	ShowWindow(app.update.notification, command);
}

void SetDeleteVisible(bool visible)
{
	const int command = visible ? SW_SHOW : SW_HIDE;
	ShowWindow(app.remove.search, command);
	ShowWindow(app.remove.findButton, command);
	ShowWindow(app.remove.studentId, command);
	ShowWindow(app.remove.firstName, command);
	ShowWindow(app.remove.lastName, command);
	ShowWindow(app.remove.middleName, command);
	ShowWindow(app.remove.course, command);
	ShowWindow(app.remove.yearLevel, command);
	ShowWindow(app.remove.section, command);
	ShowWindow(app.remove.email, command);
	ShowWindow(app.remove.deleteButton, command);
	ShowWindow(app.remove.cancelButton, command);
	ShowWindow(app.remove.notification, command);
}

void LayoutForm(HWND window, int width, int height)
{
	(void)height;
	const int contentLeft = SidebarWidth;
	const int contentWidth = width - contentLeft;
	const int padding = std::max(24, std::min(44, contentWidth / 18));
	const int left = contentLeft + padding;
	const int right = width - padding;
	const int gap = 18;
	const int columnWidth = std::max(180, (right - left - gap) / 2);
	const int top = 176;
	const int rowHeight = 70;
	const int inputHeight = 36;

	const std::array<HWND, 8> fields{
		app.form.studentId, app.form.firstName, app.form.lastName, app.form.middleName,
		app.form.course, app.form.yearLevel, app.form.section, app.form.email
	};
	for (int i = 0; i < static_cast<int>(fields.size()); ++i)
	{
		const int column = i % 2;
		const int row = i / 2;
		const int x = left + column * (columnWidth + gap);
		const int y = top + row * rowHeight;
		MoveWindow(fields[i], x, y + 22, columnWidth, InputHeight, TRUE);
	}

	const int actionY = top + rowHeight * 4 + 16;
	MoveWindow(app.form.notification, left, actionY, std::max(120, right - left - 292), 30, TRUE);
	MoveWindow(app.form.saveButton, right - 284, actionY - 4, 138, 42, TRUE);
	MoveWindow(app.form.clearButton, right - 136, actionY - 4, 136, 42, TRUE);
}

void LayoutView(HWND window, int width)
{
	(void)window;
	const int contentWidth = width - SidebarWidth;
	const int padding = std::max(24, std::min(44, contentWidth / 18));
	const int left = SidebarWidth + padding;
	const int right = width - padding;
	MoveWindow(app.view.search, left, 148, std::max(220, right - left - 148), InputHeight, TRUE);
	MoveWindow(app.view.refreshButton, right - 132, 148, 132, InputHeight, TRUE);
}

void LayoutUpdate(HWND window, int width)
{
	const int contentWidth = width - SidebarWidth;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = SidebarWidth + padding;
	const int right = width - padding;
	const int gap = 20;
	const int columnWidth = (right - left - gap) / 2;
	MoveWindow(app.update.search, left, 156, std::max(240, right - left - 148), InputHeight, TRUE);
	MoveWindow(app.update.findButton, right - 132, 156, 132, InputHeight, TRUE);

	const std::array<HWND, 8> fields{
		app.update.studentId, app.update.firstName, app.update.lastName, app.update.middleName,
		app.update.course, app.update.yearLevel, app.update.section, app.update.email
	};
	const int top = 260;
	const int rowHeight = 70;
	for (int i = 0; i < static_cast<int>(fields.size()); ++i)
	{
		const int column = i % 2;
		const int row = i / 2;
		const int x = left + column * (columnWidth + gap);
		MoveWindow(fields[i], x, top + row * rowHeight + 22, columnWidth, InputHeight, TRUE);
	}
	MoveWindow(app.update.notification, left, top + rowHeight * 4 + 10, right - left, 26, TRUE);
	MoveWindow(app.update.updateButton, right - 280, top + rowHeight * 4 + 44, 132, 42, TRUE);
	MoveWindow(app.update.cancelButton, right - 136, top + rowHeight * 4 + 44, 136, 42, TRUE);
}

void LayoutDelete(HWND window, int width)
{
	const int contentWidth = width - SidebarWidth;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = SidebarWidth + padding;
	const int right = width - padding;
	const int gap = 20;
	const int columnWidth = (right - left - gap) / 2;
	MoveWindow(app.remove.search, left, 156, std::max(240, right - left - 148), InputHeight, TRUE);
	MoveWindow(app.remove.findButton, right - 132, 156, 132, InputHeight, TRUE);

	const std::array<HWND, 8> fields{
		app.remove.studentId, app.remove.firstName, app.remove.lastName, app.remove.middleName,
		app.remove.course, app.remove.yearLevel, app.remove.section, app.remove.email
	};
	const int top = 260;
	const int rowHeight = 70;
	for (int i = 0; i < static_cast<int>(fields.size()); ++i)
	{
		const int column = i % 2;
		const int row = i / 2;
		const int x = left + column * (columnWidth + gap);
		MoveWindow(fields[i], x, top + row * rowHeight + 22, columnWidth, InputHeight, TRUE);
	}
	MoveWindow(app.remove.notification, left, top + rowHeight * 4 + 10, right - left, 26, TRUE);
	MoveWindow(app.remove.deleteButton, right - 280, top + rowHeight * 4 + 44, 132, 42, TRUE);
	MoveWindow(app.remove.cancelButton, right - 136, top + rowHeight * 4 + 44, 136, 42, TRUE);
}

void CreateFormControls(HWND window)
{
	app.form.studentId = CreateFormEdit(window, StudentIdControl);
	app.form.firstName = CreateFormEdit(window, FirstNameControl);
	app.form.lastName = CreateFormEdit(window, LastNameControl);
	app.form.middleName = CreateFormEdit(window, MiddleNameControl);
	app.form.course = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(CourseControl)), nullptr, nullptr);
	app.form.yearLevel = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(YearLevelControl)), nullptr, nullptr);
	app.form.section = CreateFormEdit(window, SectionControl);
	app.form.email = CreateFormEdit(window, EmailControl);

	PopulateAcademicComboBoxes(app.form.course, app.form.yearLevel);
	SendMessageW(app.form.course, CB_SETCURSEL, 0, 0);
	SendMessageW(app.form.yearLevel, CB_SETCURSEL, 0, 0);
	SendMessageW(app.form.course, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	SendMessageW(app.form.yearLevel, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	ModernizeInputControl(app.form.course, true);
	ModernizeInputControl(app.form.yearLevel, true);

	app.form.saveButton = CreateWindowExW(0, L"BUTTON", L"Save Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(SaveStudentButton)), nullptr, nullptr);
	app.form.clearButton = CreateWindowExW(0, L"BUTTON", L"Clear Form", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ClearFormButton)), nullptr, nullptr);
	SendMessageW(app.form.saveButton, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	SendMessageW(app.form.clearButton, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	app.form.notification = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(NotificationControl)), nullptr, nullptr);
	SendMessageW(app.form.notification, WM_SETFONT, reinterpret_cast<WPARAM>(app.smallFont), TRUE);

	SetFormVisible(false);
}

void CreateViewControls(HWND window)
{
	app.view.search = CreateFormEdit(window, StudentSearchControl);
	app.view.refreshButton = CreateWindowExW(0, L"BUTTON", L"Refresh", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(RefreshStudentsButton)), nullptr, nullptr);
	SendMessageW(app.view.search, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	SendMessageW(app.view.refreshButton, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	SetViewVisible(false);
}

void CreateUpdateControls(HWND window)
{
	app.update.search = CreateFormEdit(window, UpdateSearchControl);
	app.update.findButton = CreateWindowExW(0, L"BUTTON", L"Find Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(FindStudentButton)), nullptr, nullptr);
	app.update.studentId = CreateFormEdit(window, StudentIdControl + 100);
	app.update.firstName = CreateFormEdit(window, FirstNameControl + 100);
	app.update.lastName = CreateFormEdit(window, LastNameControl + 100);
	app.update.middleName = CreateFormEdit(window, MiddleNameControl + 100);
	app.update.course = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(CourseControl + 100)), nullptr, nullptr);
	app.update.yearLevel = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(YearLevelControl + 100)), nullptr, nullptr);
	app.update.section = CreateFormEdit(window, SectionControl + 100);
	app.update.email = CreateFormEdit(window, EmailControl + 100);
	app.update.updateButton = CreateWindowExW(0, L"BUTTON", L"Update Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(UpdateStudentButton)), nullptr, nullptr);
	app.update.cancelButton = CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(CancelUpdateButton)), nullptr, nullptr);
	app.update.notification = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(UpdateNotificationControl)), nullptr, nullptr);
	SendMessageW(app.update.notification, WM_SETFONT, reinterpret_cast<WPARAM>(app.smallFont), TRUE);

	PopulateAcademicComboBoxes(app.update.course, app.update.yearLevel);
	ModernizeInputControl(app.update.course, true);
	ModernizeInputControl(app.update.yearLevel, true);
	for (HWND control : { app.update.search, app.update.findButton, app.update.studentId, app.update.firstName,
		app.update.lastName, app.update.middleName, app.update.course, app.update.yearLevel, app.update.section,
		app.update.email, app.update.updateButton, app.update.cancelButton, app.update.notification })
	{
		SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	}
	EnableWindow(app.update.studentId, FALSE);
	SetUpdateVisible(false);
}

void CreateDeleteControls(HWND window)
{
	app.remove.search = CreateFormEdit(window, DeleteSearchControl);
	app.remove.findButton = CreateWindowExW(0, L"BUTTON", L"Find Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(FindDeleteStudentButton)), nullptr, nullptr);
	app.remove.studentId = CreateFormEdit(window, StudentIdControl + 200);
	app.remove.firstName = CreateFormEdit(window, FirstNameControl + 200);
	app.remove.lastName = CreateFormEdit(window, LastNameControl + 200);
	app.remove.middleName = CreateFormEdit(window, MiddleNameControl + 200);
	app.remove.course = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(CourseControl + 200)), nullptr, nullptr);
	app.remove.yearLevel = CreateWindowExW(0, L"COMBOBOX", L"", WS_CHILD | WS_TABSTOP | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(YearLevelControl + 200)), nullptr, nullptr);
	app.remove.section = CreateFormEdit(window, SectionControl + 200);
	app.remove.email = CreateFormEdit(window, EmailControl + 200);
	app.remove.deleteButton = CreateWindowExW(0, L"BUTTON", L"Delete Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(DeleteStudentButton)), nullptr, nullptr);
	app.remove.cancelButton = CreateWindowExW(0, L"BUTTON", L"Cancel", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(CancelDeleteButton)), nullptr, nullptr);
	app.remove.notification = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(DeleteNotificationControl)), nullptr, nullptr);
	SendMessageW(app.remove.notification, WM_SETFONT, reinterpret_cast<WPARAM>(app.smallFont), TRUE);

	PopulateAcademicComboBoxes(app.remove.course, app.remove.yearLevel);
	ModernizeInputControl(app.remove.course, true);
	ModernizeInputControl(app.remove.yearLevel, true);
	for (HWND control : { app.remove.search, app.remove.findButton, app.remove.studentId, app.remove.firstName,
		app.remove.lastName, app.remove.middleName, app.remove.course, app.remove.yearLevel, app.remove.section,
		app.remove.email, app.remove.deleteButton, app.remove.cancelButton, app.remove.notification })
	{
		SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	}
	for (HWND control : { app.remove.studentId, app.remove.firstName, app.remove.lastName, app.remove.middleName,
		app.remove.course, app.remove.yearLevel, app.remove.section, app.remove.email })
	{
		EnableWindow(control, FALSE);
	}
	SetDeleteVisible(false);
}

void SetGradeVisible(bool visible)
{
	const int command = visible ? SW_SHOW : SW_HIDE;
	ShowWindow(app.grade.search, command);
	ShowWindow(app.grade.findButton, command);
	ShowWindow(app.grade.subject, command);
	ShowWindow(app.grade.units, command);
	ShowWindow(app.grade.grade, command);
	ShowWindow(app.grade.saveButton, command);
	ShowWindow(app.grade.clearButton, command);
	ShowWindow(app.grade.threshold, command);
	ShowWindow(app.grade.applyThresholdButton, command);
	ShowWindow(app.grade.calculateGwaButton, command);
	ShowWindow(app.grade.notification, command);
}

void LayoutGrade(HWND window, int width)
{
	const int contentWidth = width - SidebarWidth;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = SidebarWidth + padding;
	const int right = width - padding;
	MoveWindow(app.grade.search, left, 148, std::max(240, right - left - 148), InputHeight, TRUE);
	MoveWindow(app.grade.findButton, right - 132, 148, 132, InputHeight, TRUE);
	const int top = 288;
	const int inputW = std::max(120, (right - left - 300) / 3);
	MoveWindow(app.grade.subject, left, top + 20, inputW + 80, InputHeight, TRUE);
	MoveWindow(app.grade.units, left + inputW + 92, top + 20, 96, InputHeight, TRUE);
	MoveWindow(app.grade.grade, left + inputW + 200, top + 20, 96, InputHeight, TRUE);
	MoveWindow(app.grade.saveButton, right - 280, top + 20, 132, InputHeight, TRUE);
	MoveWindow(app.grade.clearButton, right - 136, top + 20, 136, InputHeight, TRUE);
	MoveWindow(app.grade.threshold, left + 128, top + 76, 96, InputHeight, TRUE);
	MoveWindow(app.grade.applyThresholdButton, left + 236, top + 76, 148, InputHeight, TRUE);
	MoveWindow(app.grade.calculateGwaButton, right - 148, top + 76, 148, InputHeight, TRUE);
	MoveWindow(app.grade.notification, left, top + 124, right - left, 24, TRUE);
}

void CreateGradeControls(HWND window)
{
	app.grade.search = CreateFormEdit(window, GradeSearchControl);
	app.grade.findButton = CreateWindowExW(0, L"BUTTON", L"Find Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(FindGradeStudentButton)), nullptr, nullptr);
	app.grade.subject = CreateFormEdit(window, SubjectControl);
	app.grade.units = CreateFormEdit(window, UnitsControl);
	app.grade.grade = CreateFormEdit(window, GradeControl);
	app.grade.saveButton = CreateWindowExW(0, L"BUTTON", L"Save Grade", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(SaveGradeButton)), nullptr, nullptr);
	app.grade.clearButton = CreateWindowExW(0, L"BUTTON", L"Clear Entry", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ClearGradeButton)), nullptr, nullptr);
	app.grade.threshold = CreateFormEdit(window, ThresholdControl);
	app.grade.applyThresholdButton = CreateWindowExW(0, L"BUTTON", L"Apply Threshold", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ApplyThresholdButton)), nullptr, nullptr);
	app.grade.calculateGwaButton = CreateWindowExW(0, L"BUTTON", L"Calculate GWA", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(CalculateGwaButton)), nullptr, nullptr);
	app.grade.notification = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(GradeNotificationControl)), nullptr, nullptr);
	SendMessageW(app.grade.notification, WM_SETFONT, reinterpret_cast<WPARAM>(app.smallFont), TRUE);
	SetWindowTextW(app.grade.threshold, L"3.00");
	for (HWND control : { app.grade.search, app.grade.findButton, app.grade.subject, app.grade.units, app.grade.grade,
		app.grade.saveButton, app.grade.clearButton, app.grade.threshold, app.grade.applyThresholdButton,
		app.grade.calculateGwaButton, app.grade.notification })
	{
		SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	}
	SetGradeVisible(false);
}

void SetReportVisible(bool visible)
{
	const int command = visible ? SW_SHOW : SW_HIDE;
	ShowWindow(app.report.search, command);
	ShowWindow(app.report.findButton, command);
	ShowWindow(app.report.schoolName, command);
	ShowWindow(app.report.exportButton, command);
	ShowWindow(app.report.notification, command);
}

void LayoutReport(HWND window, int width)
{
	const int contentWidth = width - SidebarWidth;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = SidebarWidth + padding;
	const int right = width - padding;
	MoveWindow(app.report.search, left, 148, std::max(240, right - left - 148), InputHeight, TRUE);
	MoveWindow(app.report.findButton, right - 132, 148, 132, InputHeight, TRUE);
	MoveWindow(app.report.schoolName, left, 268, right - left - 168, InputHeight, TRUE);
	MoveWindow(app.report.exportButton, right - 156, 268, 156, InputHeight, TRUE);
	MoveWindow(app.report.notification, left, 316, right - left, 24, TRUE);
}

void CreateReportControls(HWND window)
{
	app.report.search = CreateFormEdit(window, ReportSearchControl);
	app.report.findButton = CreateWindowExW(0, L"BUTTON", L"Find Student", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(FindReportStudentButton)), nullptr, nullptr);
	app.report.schoolName = CreateFormEdit(window, SchoolNameControl);
	app.report.exportButton = CreateWindowExW(0, L"BUTTON", L"Export Report Card", WS_CHILD | WS_TABSTOP | BS_OWNERDRAW,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ExportReportButton)), nullptr, nullptr);
	app.report.notification = CreateWindowExW(0, L"STATIC", L"", WS_CHILD | SS_LEFT | SS_CENTERIMAGE,
		0, 0, 0, 0, window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(ReportNotificationControl)), nullptr, nullptr);
	SendMessageW(app.report.notification, WM_SETFONT, reinterpret_cast<WPARAM>(app.smallFont), TRUE);
	SetWindowTextW(app.report.schoolName, L"School Name (editable)");
	for (HWND control : { app.report.search, app.report.findButton, app.report.schoolName,
		app.report.exportButton, app.report.notification })
	{
		SendMessageW(control, WM_SETFONT, reinterpret_cast<WPARAM>(app.bodyFont), TRUE);
	}
	SetReportVisible(false);
}

void ClearForm(bool clearNotification = true)
{
	SetWindowTextW(app.form.studentId, L"");
	SetWindowTextW(app.form.firstName, L"");
	SetWindowTextW(app.form.lastName, L"");
	SetWindowTextW(app.form.middleName, L"");
	SetWindowTextW(app.form.section, L"");
	SetWindowTextW(app.form.email, L"");
	SendMessageW(app.form.course, CB_SETCURSEL, 0, 0);
	SendMessageW(app.form.yearLevel, CB_SETCURSEL, 0, 0);
	if (clearNotification)
	{
		SetWindowTextW(app.form.notification, L"");
		app.form.notificationError = false;
	}
}

std::wstring GetControlText(HWND control)
{
	const int length = GetWindowTextLengthW(control);
	std::wstring text(static_cast<size_t>(length) + 1, L'\0');
	if (length > 0)
	{
		GetWindowTextW(control, text.data(), length + 1);
	}
	text.resize(static_cast<size_t>(length));
	return text;
}

std::wstring GetComboBoxSelection(HWND control)
{
	const LRESULT selectedIndex = SendMessageW(control, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR)
	{
		return {};
	}
	const LRESULT textLength = SendMessageW(control, CB_GETLBTEXTLEN, static_cast<WPARAM>(selectedIndex), 0);
	if (textLength == CB_ERR)
	{
		return {};
	}
	std::wstring text(static_cast<size_t>(textLength) + 1, L'\0');
	if (SendMessageW(control, CB_GETLBTEXT, static_cast<WPARAM>(selectedIndex), reinterpret_cast<LPARAM>(text.data())) == CB_ERR)
	{
		return {};
	}
	text.resize(static_cast<size_t>(textLength));
	return text;
}

bool IsBlank(const std::wstring& text)
{
	return text.empty() || std::all_of(text.begin(), text.end(), [](wchar_t character) {
		return iswspace(character) != 0;
	});
}

void SetNotification(const std::wstring& message, bool error)
{
	app.form.notificationError = error;
	SetWindowTextW(app.form.notification, message.c_str());
	InvalidateRect(app.form.notification, nullptr, TRUE);
}

void SaveStudent()
{
	Student student{
		GetControlText(app.form.studentId), GetControlText(app.form.firstName),
		GetControlText(app.form.lastName), GetControlText(app.form.middleName),
		GetComboBoxSelection(app.form.course), GetComboBoxSelection(app.form.yearLevel),
		GetControlText(app.form.section), GetControlText(app.form.email)
	};
	const std::array<std::pair<const wchar_t*, const std::wstring*>, 8> required{
		std::pair{ L"Student ID", &student.studentId }, std::pair{ L"First Name", &student.firstName },
		std::pair{ L"Last Name", &student.lastName }, std::pair{ L"Middle Name", &student.middleName },
		std::pair{ L"Course", &student.course }, std::pair{ L"Year Level", &student.yearLevel },
		std::pair{ L"Section", &student.section }, std::pair{ L"Email Address", &student.email }
	};
	for (const auto& field : required)
	{
		if (IsBlank(*field.second))
		{
			SetNotification(std::wstring(L"Please complete the required field: ") + field.first, true);
			return;
		}
	}
	if (student.email.find(L'@') == std::wstring::npos || student.email.find(L'.') == std::wstring::npos)
	{
		SetNotification(L"Please enter a valid email address.", true);
		return;
	}
	const bool duplicate = std::any_of(students.begin(), students.end(), [&](const Student& existing) {
		return existing.studentId == student.studentId;
	});
	if (duplicate)
	{
		SetNotification(L"A student with this Student ID already exists.", true);
		return;
	}
	students.push_back(std::move(student));
	if (!SaveData())
	{
		SetNotification(L"Student registered, but the records could not be saved.", true);
		return;
	}
	SetNotification(L"Student registered successfully.", false);
	ClearForm();
}

void SetUpdateNotification(const std::wstring& message, bool error)
{
	app.update.notificationError = error;
	SetWindowTextW(app.update.notification, message.c_str());
	InvalidateRect(app.update.notification, nullptr, TRUE);
}

void ClearUpdateFields()
{
	SetWindowTextW(app.update.studentId, L"");
	SetWindowTextW(app.update.firstName, L"");
	SetWindowTextW(app.update.lastName, L"");
	SetWindowTextW(app.update.middleName, L"");
	SetWindowTextW(app.update.section, L"");
	SetWindowTextW(app.update.email, L"");
	SendMessageW(app.update.course, CB_SETCURSEL, static_cast<WPARAM>(-1), 0);
	SendMessageW(app.update.yearLevel, CB_SETCURSEL, static_cast<WPARAM>(-1), 0);
	app.update.selectedIndex = -1;
}

void FindStudentForUpdate()
{
	const std::wstring searchId = GetControlText(app.update.search);
	if (IsBlank(searchId))
	{
		SetUpdateNotification(L"Enter a Student ID to search.", true);
		return;
	}
	const auto found = std::find_if(students.begin(), students.end(), [&](const Student& student) {
		return student.studentId == searchId;
	});
	if (found == students.end())
	{
		ClearUpdateFields();
		SetUpdateNotification(L"No student was found with that Student ID.", true);
		return;
	}
	app.update.selectedIndex = static_cast<int>(std::distance(students.begin(), found));
	const Student& student = *found;
	SetWindowTextW(app.update.studentId, student.studentId.c_str());
	SetWindowTextW(app.update.firstName, student.firstName.c_str());
	SetWindowTextW(app.update.lastName, student.lastName.c_str());
	SetWindowTextW(app.update.middleName, student.middleName.c_str());
	SetWindowTextW(app.update.section, student.section.c_str());
	SetWindowTextW(app.update.email, student.email.c_str());
	SelectComboValue(app.update.course, student.course);
	SelectComboValue(app.update.yearLevel, student.yearLevel);
	SetUpdateNotification(L"Student information loaded. You may now edit the record.", false);
}

void UpdateStudentRecord()
{
	if (app.update.selectedIndex < 0 || app.update.selectedIndex >= static_cast<int>(students.size()))
	{
		SetUpdateNotification(L"Search for a student before updating.", true);
		return;
	}
	Student updated{
		GetControlText(app.update.studentId), GetControlText(app.update.firstName),
		GetControlText(app.update.lastName), GetControlText(app.update.middleName),
		GetComboBoxSelection(app.update.course), GetComboBoxSelection(app.update.yearLevel),
		GetControlText(app.update.section), GetControlText(app.update.email),
		students[app.update.selectedIndex].grades
	};
	const std::array<std::pair<const wchar_t*, const std::wstring*>, 8> required{
		std::pair{ L"Student ID", &updated.studentId }, std::pair{ L"First Name", &updated.firstName },
		std::pair{ L"Last Name", &updated.lastName }, std::pair{ L"Middle Name", &updated.middleName },
		std::pair{ L"Course", &updated.course }, std::pair{ L"Year Level", &updated.yearLevel },
		std::pair{ L"Section", &updated.section }, std::pair{ L"Email Address", &updated.email }
	};
	for (const auto& field : required)
	{
		if (IsBlank(*field.second))
		{
			SetUpdateNotification(std::wstring(L"Please complete the required field: ") + field.first, true);
			return;
		}
	}
	if (updated.email.find(L'@') == std::wstring::npos || updated.email.find(L'.') == std::wstring::npos)
	{
		SetUpdateNotification(L"Please enter a valid email address.", true);
		return;
	}
	updated.studentId = students[app.update.selectedIndex].studentId;
	students[app.update.selectedIndex] = std::move(updated);
	if (!SaveData())
	{
		SetUpdateNotification(L"Student updated, but the records could not be saved.", true);
		return;
	}
	SetUpdateNotification(L"Student information updated successfully.", false);
}

struct DashboardStatistics
{
	size_t enrolledStudents = 0;
	size_t subjects = 0;
	size_t gradeRecords = 0;
	double averageGwa = 0.0;
	bool hasAverageGwa = false;
};

DashboardStatistics CalculateDashboardStatistics()
{
	DashboardStatistics statistics;
	statistics.enrolledStudents = students.size();
	std::set<std::wstring> subjects;
	double gwaTotal = 0.0;
	size_t studentsWithGwa = 0;
	for (const Student& student : students)
	{
		double weightedGrades = 0.0;
		double totalUnits = 0.0;
		for (const Grade& grade : student.grades)
		{
			++statistics.gradeRecords;
			std::wstring subject = grade.subject;
			std::transform(subject.begin(), subject.end(), subject.begin(), [](wchar_t character) {
				return static_cast<wchar_t>(towlower(character));
			});
			if (!subject.empty())
			{
				subjects.insert(std::move(subject));
			}
			if (grade.units > 0.0 && grade.grade >= 1.0 && grade.grade <= 5.0)
			{
				weightedGrades += grade.grade * grade.units;
				totalUnits += grade.units;
			}
		}
		if (totalUnits > 0.0)
		{
			gwaTotal += weightedGrades / totalUnits;
			++studentsWithGwa;
		}
	}
	statistics.subjects = subjects.size();
	statistics.hasAverageGwa = studentsWithGwa > 0;
	if (statistics.hasAverageGwa)
	{
		statistics.averageGwa = gwaTotal / static_cast<double>(studentsWithGwa);
	}
	return statistics;
}

void DrawDashboard(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = contentLeft + padding;
	const int right = client.right - padding;

	RECT contentBackground{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &contentBackground, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 30, right, 50 };
	DrawTextBlock(dc, L"OVERVIEW", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 52, right, 92 };
	DrawTextBlock(dc, L"Dashboard", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 94, right, 120 };
	DrawTextBlock(dc, L"Welcome back. Here is the latest student activity.", subtitle, app.bodyFont, TextSecondary);

	const int cardGap = 16;
	const int cardWidth = std::max(150, (right - left - cardGap * 3) / 4);
	const int cardTop = 142;
	const int cardBottom = 232;
	const DashboardStatistics statistics = CalculateDashboardStatistics();
	const std::wstring enrolledStudents = std::to_wstring(statistics.enrolledStudents);
	const std::wstring subjectCount = std::to_wstring(statistics.subjects);
	const std::wstring gradeRecordCount = std::to_wstring(statistics.gradeRecords);
	std::wstring averageGwa = L"N/A";
	if (statistics.hasAverageGwa)
	{
		std::wostringstream averageGwaStream;
		averageGwaStream << std::fixed << std::setprecision(2) << statistics.averageGwa;
		averageGwa = averageGwaStream.str();
	}
	DrawStatCard(dc, RECT{ left, cardTop, left + cardWidth, cardBottom }, L"Enrolled Students", enrolledStudents.c_str(), Accent);
	DrawStatCard(dc, RECT{ left + cardWidth + cardGap, cardTop, left + cardWidth * 2 + cardGap, cardBottom },
		L"Total Subjects", subjectCount.c_str(), Success);
	DrawStatCard(dc, RECT{ left + (cardWidth + cardGap) * 2, cardTop, left + cardWidth * 3 + cardGap * 2, cardBottom },
		L"Grade Records", gradeRecordCount.c_str(), AccentBright);
	DrawStatCard(dc, RECT{ left + (cardWidth + cardGap) * 3, cardTop, right, cardBottom },
		L"Average GWA", averageGwa.c_str(), Warning);

	const int panelTop = 256;
	const int panelBottom = std::max(panelTop + 180, static_cast<int>(client.bottom) - 24);
	const int panelGap = 18;
	const int activityWidth = std::max(280, (right - left - panelGap) * 2 / 3);
	RECT activityPanel{ left, panelTop, left + activityWidth, panelBottom };
	FillGradientRect(dc, activityPanel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, activityPanel, Border, 12);
	RECT activityTitle{ left + 22, panelTop + 18, left + activityWidth - 22, panelTop + 46 };
	DrawTextBlock(dc, L"Recent Student Registrations", activityTitle, app.headingFont, AccentBright);

	const int registrationCount = std::min(5, static_cast<int>(students.size()));
	for (int row = 0; row < registrationCount; ++row)
	{
		const Student& student = students[students.size() - 1 - row];
		const int rowTop = panelTop + 58 + row * 46;
		RECT divider{ left + 22, rowTop - 1, left + activityWidth - 22, rowTop };
		HBRUSH dividerBrush = CreateSolidBrush(BorderSoft);
		FillRect(dc, &divider, dividerBrush);
		DeleteObject(dividerBrush);
		RECT nameRect{ left + 22, rowTop + 6, left + activityWidth / 2, rowTop + 30 };
		const std::wstring name = student.studentId + L"  " + student.firstName + L" " + student.middleName + L" " + student.lastName;
		DrawTextBlock(dc, name.c_str(), nameRect, app.bodyFont, TextPrimary);
		RECT courseRect{ left + activityWidth / 2, rowTop + 6, left + activityWidth - 150, rowTop + 30 };
		const std::wstring academicInfo = student.course + L" | " + student.yearLevel;
		DrawTextBlock(dc, academicInfo.c_str(), courseRect, app.smallFont, TextSecondary, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		RECT dateRect{ left + activityWidth - 150, rowTop + 6, left + activityWidth - 22, rowTop + 30 };
		const std::wstring registrationInfo = L"Record " + std::to_wstring(students.size() - row);
		DrawTextBlock(dc, registrationInfo.c_str(), dateRect, app.smallFont, AccentIce, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
	if (registrationCount == 0)
	{
		RECT emptyState{ left + 22, panelTop + 70, left + activityWidth - 22, panelTop + 110 };
		DrawTextBlock(dc, L"No student registrations yet.", emptyState, app.bodyFont, TextSecondary);
	}

	RECT summaryPanel{ left + activityWidth + panelGap, panelTop, right, panelBottom };
	FillGradientRect(dc, summaryPanel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, summaryPanel, Border, 12);
	RECT summaryTitle{ summaryPanel.left + 22, panelTop + 18, summaryPanel.right - 22, panelTop + 46 };
	DrawTextBlock(dc, L"System Status", summaryTitle, app.headingFont, AccentBright);
	RECT statusDot{ summaryPanel.left + 22, panelTop + 72, summaryPanel.left + 34, panelTop + 84 };
	FillRoundedRect(dc, statusDot, Success, 6);
	RECT statusText{ summaryPanel.left + 46, panelTop + 64, summaryPanel.right - 18, panelTop + 92 };
	DrawTextBlock(dc, L"All systems operational", statusText, app.bodyFont, TextSecondary);
	RECT tip{ summaryPanel.left + 22, panelTop + 114, summaryPanel.right - 22, panelTop + 188 };
	DrawTextBlock(dc, L"Use the navigation menu to manage student records, grades, GWA, and report cards.", tip,
		app.smallFont, TextSecondary, DT_LEFT | DT_TOP | DT_WORDBREAK);
}

void SetDeleteNotification(const std::wstring& message, bool error)
{
	app.remove.notificationError = error;
	SetWindowTextW(app.remove.notification, message.c_str());
	InvalidateRect(app.remove.notification, nullptr, TRUE);
}

void ClearDeleteFields()
{
	SetWindowTextW(app.remove.studentId, L"");
	SetWindowTextW(app.remove.firstName, L"");
	SetWindowTextW(app.remove.lastName, L"");
	SetWindowTextW(app.remove.middleName, L"");
	SetWindowTextW(app.remove.section, L"");
	SetWindowTextW(app.remove.email, L"");
	SendMessageW(app.remove.course, CB_SETCURSEL, static_cast<WPARAM>(-1), 0);
	SendMessageW(app.remove.yearLevel, CB_SETCURSEL, static_cast<WPARAM>(-1), 0);
	app.remove.selectedIndex = -1;
}

void FindStudentForDelete()
{
	const std::wstring searchId = GetControlText(app.remove.search);
	if (IsBlank(searchId))
	{
		SetDeleteNotification(L"Enter a Student ID to search.", true);
		return;
	}
	const auto found = std::find_if(students.begin(), students.end(), [&](const Student& student) {
		return student.studentId == searchId;
	});
	if (found == students.end())
	{
		ClearDeleteFields();
		SetDeleteNotification(L"No student was found with that Student ID.", true);
		return;
	}
	app.remove.selectedIndex = static_cast<int>(std::distance(students.begin(), found));
	const Student& student = *found;
	SetWindowTextW(app.remove.studentId, student.studentId.c_str());
	SetWindowTextW(app.remove.firstName, student.firstName.c_str());
	SetWindowTextW(app.remove.lastName, student.lastName.c_str());
	SetWindowTextW(app.remove.middleName, student.middleName.c_str());
	SetWindowTextW(app.remove.section, student.section.c_str());
	SetWindowTextW(app.remove.email, student.email.c_str());
	const LRESULT course = SendMessageW(app.remove.course, CB_FINDSTRINGEXACT, static_cast<WPARAM>(-1),
		reinterpret_cast<LPARAM>(student.course.c_str()));
	const LRESULT year = SendMessageW(app.remove.yearLevel, CB_FINDSTRINGEXACT, static_cast<WPARAM>(-1),
		reinterpret_cast<LPARAM>(student.yearLevel.c_str()));
	SendMessageW(app.remove.course, CB_SETCURSEL, course == CB_ERR ? static_cast<WPARAM>(-1) : static_cast<WPARAM>(course), 0);
	SendMessageW(app.remove.yearLevel, CB_SETCURSEL, year == CB_ERR ? static_cast<WPARAM>(-1) : static_cast<WPARAM>(year), 0);
	SetDeleteNotification(L"Student found. Review the information before deleting.", false);
}

void DeleteStudentRecord()
{
	if (app.remove.selectedIndex < 0 || app.remove.selectedIndex >= static_cast<int>(students.size()))
	{
		SetDeleteNotification(L"Search for a student before deleting.", true);
		return;
	}
	const Student& student = students[app.remove.selectedIndex];
	const std::wstring fullName = student.firstName + L" " + student.lastName;
	const std::wstring prompt = L"Delete student " + fullName + L" (" + student.studentId + L")?";
	const int confirmation = MessageBoxW(nullptr, prompt.c_str(), L"Confirm Student Deletion",
		MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2);
	if (confirmation != IDYES)
	{
		SetDeleteNotification(L"Deletion cancelled.", false);
		return;
	}
	students.erase(students.begin() + app.remove.selectedIndex);
	ClearDeleteFields();
	SetWindowTextW(app.remove.search, L"");
	if (!SaveData())
	{
		SetDeleteNotification(L"Student deleted, but the records could not be saved.", true);
		return;
	}
	SetDeleteNotification(L"Student deleted successfully.", false);
}

std::wstring Lowercase(std::wstring text)
{
	std::transform(text.begin(), text.end(), text.begin(), [](wchar_t character) {
		return static_cast<wchar_t>(towlower(character));
	});
	return text;
}

bool StudentMatchesSearch(const Student& student, const std::wstring& search)
{
	if (search.empty())
	{
		return true;
	}
	const std::wstring name = student.firstName + L" " + student.middleName + L" " + student.lastName;
	return Lowercase(student.studentId).find(search) != std::wstring::npos
		|| Lowercase(name).find(search) != std::wstring::npos;
}

bool ParseDecimal(HWND control, double& value)
{
	const std::wstring text = GetControlText(control);
	if (IsBlank(text))
	{
		return false;
	}
	wchar_t* end = nullptr;
	value = wcstod(text.c_str(), &end);
	return end != text.c_str() && *end == L'\0';
}

void SetGradeNotification(const std::wstring& message, bool error)
{
	app.grade.notificationError = error;
	SetWindowTextW(app.grade.notification, message.c_str());
	InvalidateRect(app.grade.notification, nullptr, TRUE);
}

void ClearGradeEntry()
{
	SetWindowTextW(app.grade.subject, L"");
	SetWindowTextW(app.grade.units, L"");
	SetWindowTextW(app.grade.grade, L"");
	app.grade.editingGradeIndex = -1;
}

void FindStudentForGrades()
{
	const std::wstring searchId = GetControlText(app.grade.search);
	if (IsBlank(searchId))
	{
		SetGradeNotification(L"Enter a Student ID to search.", true);
		return;
	}
	const auto found = std::find_if(students.begin(), students.end(), [&](const Student& student) {
		return student.studentId == searchId;
	});
	if (found == students.end())
	{
		app.grade.selectedStudentIndex = -1;
		ClearGradeEntry();
		SetGradeNotification(L"No student was found with that Student ID.", true);
		return;
	}
	app.grade.selectedStudentIndex = static_cast<int>(std::distance(students.begin(), found));
	app.grade.hasGwa = false;
	app.grade.totalUnits = 0.0;
	app.grade.gwa = 0.0;
	ClearGradeEntry();
	SetGradeNotification(L"Student found. Add a new subject or edit an existing subject.", false);
}

void ApplyGradeThreshold()
{
	double threshold = 0.0;
	if (!ParseDecimal(app.grade.threshold, threshold) || threshold < 1.0 || threshold > 5.0)
	{
		SetGradeNotification(L"Passing threshold must be between 1.00 and 5.00.", true);
		return;
	}
	app.grade.passingThreshold = threshold;
	SetGradeNotification(L"Passing threshold updated.", false);
}

void CalculateGwa()
{
	if (app.grade.selectedStudentIndex < 0 || app.grade.selectedStudentIndex >= static_cast<int>(students.size()))
	{
		app.grade.hasGwa = false;
		SetGradeNotification(L"Search for a student before calculating GWA.", true);
		return;
	}
	double weightedGrades = 0.0;
	double totalUnits = 0.0;
	for (const Grade& item : students[app.grade.selectedStudentIndex].grades)
	{
		if (item.units > 0.0 && item.grade >= 1.0 && item.grade <= 5.0)
		{
			weightedGrades += item.grade * item.units;
			totalUnits += item.units;
		}
	}
	if (totalUnits <= 0.0)
	{
		app.grade.hasGwa = false;
		app.grade.totalUnits = 0.0;
		app.grade.gwa = 0.0;
		SetGradeNotification(L"No valid grades with positive units exist for this student.", true);
		return;
	}
	app.grade.totalUnits = totalUnits;
	app.grade.gwa = weightedGrades / totalUnits;
	app.grade.hasGwa = true;
	SetGradeNotification(L"GWA calculated successfully.", false);
}

bool CalculateStudentReportGwa(const Student& student, double& totalUnits, double& gwa)
{
	double weightedGrades = 0.0;
	totalUnits = 0.0;
	for (const Grade& item : student.grades)
	{
		if (item.units > 0.0 && item.grade >= 1.0 && item.grade <= 5.0)
		{
			weightedGrades += item.grade * item.units;
			totalUnits += item.units;
		}
	}
	if (totalUnits <= 0.0)
	{
		gwa = 0.0;
		return false;
	}
	gwa = weightedGrades / totalUnits;
	return true;
}

void SetReportNotification(const std::wstring& message, bool error)
{
	app.report.notificationError = error;
	SetWindowTextW(app.report.notification, message.c_str());
	InvalidateRect(app.report.notification, nullptr, TRUE);
}

void FindStudentForReport()
{
	const std::wstring searchId = GetControlText(app.report.search);
	if (IsBlank(searchId))
	{
		SetReportNotification(L"Enter a Student ID to search.", true);
		return;
	}
	const auto found = std::find_if(students.begin(), students.end(), [&](const Student& student) {
		return student.studentId == searchId;
	});
	if (found == students.end())
	{
		app.report.selectedStudentIndex = -1;
		app.report.hasGwa = false;
		SetReportNotification(L"No student was found with that Student ID.", true);
		return;
	}
	app.report.selectedStudentIndex = static_cast<int>(std::distance(students.begin(), found));
	app.report.hasGwa = CalculateStudentReportGwa(*found, app.report.totalUnits, app.report.gwa);
	if (app.report.hasGwa)
	{
		SetReportNotification(L"Report card loaded successfully.", false);
	}
	else
	{
		SetReportNotification(L"Student found, but no valid grades exist for the report.", true);
	}
}

void ExportReportCard()
{
	if (app.report.selectedStudentIndex < 0 || app.report.selectedStudentIndex >= static_cast<int>(students.size()))
	{
		SetReportNotification(L"Search for a student before exporting a report card.", true);
		return;
	}
	const Student& student = students[app.report.selectedStudentIndex];
	app.report.hasGwa = CalculateStudentReportGwa(student, app.report.totalUnits, app.report.gwa);
	if (!app.report.hasGwa)
	{
		SetReportNotification(L"Cannot export: no valid grades or positive subject units exist.", true);
		return;
	}
	wchar_t fileName[MAX_PATH] = L"ReportCard.txt";
	OPENFILENAMEW dialog{ sizeof(OPENFILENAMEW) };
	dialog.hwndOwner = nullptr;
	dialog.lpstrFile = fileName;
	dialog.nMaxFile = MAX_PATH;
	dialog.lpstrFilter = L"Text Report (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	dialog.nFilterIndex = 1;
	dialog.lpstrDefExt = L"txt";
	dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
	if (!GetSaveFileNameW(&dialog))
	{
		SetReportNotification(L"Report export cancelled.", false);
		return;
	}
	std::wofstream file(fileName);
	if (!file)
	{
		SetReportNotification(L"Unable to create the report file.", true);
		return;
	}
	const std::wstring school = GetControlText(app.report.schoolName);
	file << school << L"\n\nREPORT CARD\n==============================\n";
	file << L"Student ID: " << student.studentId << L"\n";
	file << L"Student Name: " << student.firstName << L" " << student.middleName << L" " << student.lastName << L"\n";
	file << L"Course: " << student.course << L"\n";
	file << L"Year Level: " << student.yearLevel << L"\n";
	file << L"Section: " << student.section << L"\n\n";
	file << L"Subject\tUnits\tGrade\n------------------------------\n";
	for (const Grade& item : student.grades)
	{
		if (item.units > 0.0 && item.grade >= 1.0 && item.grade <= 5.0)
		{
			file << item.subject << L"\t" << FormatDecimal(item.units) << L"\t" << FormatDecimal(item.grade) << L"\n";
		}
	}
	file << L"\nTotal Subject Units: " << FormatDecimal(app.report.totalUnits) << L"\n";
	file << L"GWA: " << FormatDecimal(app.report.gwa) << L"\n";
	file.close();
	SetReportNotification(L"Report card exported successfully.", false);
}

void SaveGrade()
{
	if (app.grade.selectedStudentIndex < 0 || app.grade.selectedStudentIndex >= static_cast<int>(students.size()))
	{
		SetGradeNotification(L"Search for a student before entering grades.", true);
		return;
	}
	const std::wstring subject = GetControlText(app.grade.subject);
	if (IsBlank(subject))
	{
		SetGradeNotification(L"Enter a subject name.", true);
		return;
	}
	double units = 0.0;
	if (!ParseDecimal(app.grade.units, units) || units <= 0.0)
	{
		SetGradeNotification(L"Units must be a positive number.", true);
		return;
	}
	double gradeValue = 0.0;
	if (!ParseDecimal(app.grade.grade, gradeValue) || gradeValue < 1.0 || gradeValue > 5.0)
	{
		SetGradeNotification(L"Grade must be between 1.00 and 5.00.", true);
		return;
	}
	Student& student = students[app.grade.selectedStudentIndex];
	const std::wstring normalizedSubject = Lowercase(subject);
	const auto existing = std::find_if(student.grades.begin(), student.grades.end(), [&](const Grade& item) {
		return Lowercase(item.subject) == normalizedSubject;
	});
	if (existing != student.grades.end())
	{
		existing->units = units;
		existing->grade = gradeValue;
		app.grade.editingGradeIndex = static_cast<int>(std::distance(student.grades.begin(), existing));
		SetGradeNotification(L"Subject grade updated successfully.", false);
	}
	else
	{
		student.grades.push_back(Grade{ subject, units, gradeValue });
		app.grade.editingGradeIndex = static_cast<int>(student.grades.size()) - 1;
		SetGradeNotification(L"Subject grade added successfully.", false);
	}
	app.grade.hasGwa = false;
	app.grade.totalUnits = 0.0;
	app.grade.gwa = 0.0;
	if (!SaveData())
	{
		SetGradeNotification(L"Grade changed, but the records could not be saved.", true);
		return;
	}
	ClearGradeEntry();
}

void DrawViewStudents(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = contentLeft + padding;
	const int right = client.right - padding;
	RECT background{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &background, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 30, right, 50 };
	DrawTextBlock(dc, L"STUDENT DIRECTORY", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 52, right, 92 };
	DrawTextBlock(dc, L"View Students", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 94, right, 120 };
	DrawTextBlock(dc, L"Search and review all students currently registered.", subtitle, app.bodyFont, TextSecondary);
	RECT searchHint{ left, 134, left + 220, 154 };
	DrawTextBlock(dc, L"Search by ID or name", searchHint, app.smallFont, TextSecondary);

	RECT tablePanel{ left, 206, right, client.bottom - 24 };
	FillGradientRect(dc, tablePanel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, tablePanel, Border, 12);
	RECT header{ tablePanel.left + 16, tablePanel.top + 16, tablePanel.right - 16, tablePanel.top + 56 };
	FillGradientRect(dc, header, RGB(12, 52, 96), RGB(10, 38, 74));
	DrawRoundedBorder(dc, header, Border, 8);
	const int tableWidth = header.right - header.left;
	const int idWidth = tableWidth * 17 / 100;
	const int nameWidth = tableWidth * 29 / 100;
	const int courseWidth = tableWidth * 27 / 100;
	const int yearWidth = tableWidth * 14 / 100;
	const std::array<const wchar_t*, 5> headings{ L"STUDENT ID", L"FULL NAME", L"COURSE", L"YEAR LEVEL", L"SECTION" };
	const std::array<int, 6> columns{
		header.left, header.left + idWidth, header.left + idWidth + nameWidth,
		header.left + idWidth + nameWidth + courseWidth,
		header.left + idWidth + nameWidth + courseWidth + yearWidth, header.right
	};
	for (int i = 0; i < 5; ++i)
	{
		RECT headingRect{ columns[i] + 14, header.top, columns[i + 1] - 10, header.bottom };
		DrawTextBlock(dc, headings[i], headingRect, app.smallFont, AccentBright);
	}

	const std::wstring search = Lowercase(GetControlText(app.view.search));
	int visibleCount = 0;
	const int rowHeight = 48;
	for (const Student& student : students)
	{
		if (!StudentMatchesSearch(student, search))
		{
			continue;
		}
		const int rowTop = header.bottom + 6 + visibleCount * rowHeight;
		if (rowTop + rowHeight > tablePanel.bottom - 10)
		{
			break;
		}
		if (visibleCount % 2 == 0)
		{
			RECT rowBackground{ header.left, rowTop, header.right, rowTop + rowHeight - 1 };
			HBRUSH rowBrush = CreateSolidBrush(SurfaceDeep);
			FillRect(dc, &rowBackground, rowBrush);
			DeleteObject(rowBrush);
		}
		const std::wstring name = student.firstName + L" " + student.middleName + L" " + student.lastName;
		const std::array<std::wstring, 5> values{ student.studentId, name, student.course, student.yearLevel, student.section };
		for (int column = 0; column < 5; ++column)
		{
			RECT valueRect{ columns[column] + 14, rowTop, columns[column + 1] - 10, rowTop + rowHeight };
			DrawTextBlock(dc, values[column].c_str(), valueRect, app.bodyFont, TextPrimary,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
		++visibleCount;
	}

	if (visibleCount == 0)
	{
		RECT emptyState{ tablePanel.left + 24, header.bottom + 44, tablePanel.right - 24, header.bottom + 96 };
		const wchar_t* message = students.empty()
			? L"No student records exist yet. Use Add Student to register the first student."
			: L"No students match the current search.";
		DrawTextBlock(dc, message, emptyState, app.bodyFont, TextSecondary,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}
}

void DrawUpdateStudent(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = contentLeft + padding;
	const int right = client.right - padding;
	RECT background{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &background, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 30, right, 50 };
	DrawTextBlock(dc, L"STUDENT RECORDS", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 52, right, 92 };
	DrawTextBlock(dc, L"Update Student", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 94, right, 120 };
	DrawTextBlock(dc, L"Find a student by ID, then update the existing record.", subtitle, app.bodyFont, TextSecondary);
	RECT searchHint{ left, 134, left + 220, 154 };
	DrawTextBlock(dc, L"Search by Student ID", searchHint, app.smallFont, TextSecondary);

	RECT formPanel{ left, 206, right, client.bottom - 24 };
	FillGradientRect(dc, formPanel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, formPanel, Border, 12);
	RECT formTitle{ left + 24, 222, right - 24, 248 };
	DrawTextBlock(dc, L"Student Information", formTitle, app.headingFont, AccentBright);

	const int gap = 20;
	const int columnWidth = (right - left - gap) / 2;
	const int top = 252;
	const int rowHeight = 68;
	const std::array<const wchar_t*, 8> labels{
		L"Student ID", L"First Name", L"Last Name", L"Middle Name",
		L"Course", L"Year Level", L"Section", L"Email Address"
	};
	for (int i = 0; i < static_cast<int>(labels.size()); ++i)
	{
		const int column = i % 2;
		const int row = i / 2;
		const int x = left + column * (columnWidth + gap);
		RECT labelRect{ x + 2, top + row * rowHeight, x + columnWidth, top + row * rowHeight + 18 };
		DrawTextBlock(dc, labels[i], labelRect, app.smallFont, TextSecondary);
	}
}

void DrawDeleteStudent(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = contentLeft + padding;
	const int right = client.right - padding;
	RECT background{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &background, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 30, right, 50 };
	DrawTextBlock(dc, L"STUDENT RECORDS", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 52, right, 92 };
	DrawTextBlock(dc, L"Delete Student", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 94, right, 120 };
	DrawTextBlock(dc, L"Find a student by ID and remove the record after confirmation.", subtitle, app.bodyFont, TextSecondary);
	RECT searchHint{ left, 134, left + 220, 154 };
	DrawTextBlock(dc, L"Search by Student ID", searchHint, app.smallFont, TextSecondary);

	RECT formPanel{ left, 206, right, client.bottom - 24 };
	FillGradientRect(dc, formPanel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, formPanel, Border, 12);
	RECT formTitle{ left + 24, 222, right - 24, 248 };
	DrawTextBlock(dc, L"Student Information", formTitle, app.headingFont, Danger);

	const int gap = 20;
	const int columnWidth = (right - left - gap) / 2;
	const int top = 252;
	const int rowHeight = 68;
	const std::array<const wchar_t*, 8> labels{
		L"Student ID", L"First Name", L"Last Name", L"Middle Name",
		L"Course", L"Year Level", L"Section", L"Email Address"
	};
	for (int i = 0; i < static_cast<int>(labels.size()); ++i)
	{
		const int column = i % 2;
		const int row = i / 2;
		const int x = left + column * (columnWidth + gap);
		RECT labelRect{ x + 2, top + row * rowHeight, x + columnWidth, top + row * rowHeight + 18 };
		DrawTextBlock(dc, labels[i], labelRect, app.smallFont, TextSecondary);
	}
}

std::wstring FormatDecimal(double value)
{
	wchar_t buffer[32]{};
	swprintf_s(buffer, L"%.2f", value);
	return buffer;
}

void DrawGradeManagement(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = contentLeft + padding;
	const int right = client.right - padding;
	RECT background{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &background, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 30, right, 50 };
	DrawTextBlock(dc, L"ACADEMIC RECORDS", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 52, right, 92 };
	DrawTextBlock(dc, L"Grade Management", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 94, right, 120 };
	DrawTextBlock(dc, L"Manage subject grades and units for each registered student.", subtitle, app.bodyFont, TextSecondary);
	RECT searchHint{ left, 128, left + 220, 146 };
	DrawTextBlock(dc, L"Search by Student ID", searchHint, app.smallFont, TextSecondary);

	RECT studentInfo{ left, 196, right - 280, 222 };
	if (app.grade.selectedStudentIndex >= 0 && app.grade.selectedStudentIndex < static_cast<int>(students.size()))
	{
		const Student& student = students[app.grade.selectedStudentIndex];
		const std::wstring name = student.firstName + L" " + student.lastName;
		const std::wstring text = L"Selected: " + name + L"  •  " + student.studentId;
		DrawTextBlock(dc, text.c_str(), studentInfo, app.bodyFont, AccentBright);
	}
	else
	{
		DrawTextBlock(dc, L"Search for a student to manage grades.", studentInfo, app.bodyFont, TextSecondary);
	}
	RECT gwaResult{ right - 280, 196, right, 222 };
	if (app.grade.hasGwa)
	{
		const std::wstring result = L"GWA " + FormatDecimal(app.grade.gwa) + L"  |  Units " + FormatDecimal(app.grade.totalUnits);
		DrawTextBlock(dc, result.c_str(), gwaResult, app.bodyFont, Success, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}
	else
	{
		DrawTextBlock(dc, L"GWA not calculated", gwaResult, app.smallFont, TextSecondary, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}

	RECT panel{ left, 232, right, client.bottom - 24 };
	FillGradientRect(dc, panel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, panel, Border, 12);
	RECT entryTitle{ left + 22, 246, right - 22, 270 };
	DrawTextBlock(dc, L"Subject Grade Entry", entryTitle, app.headingFont, AccentBright);

	const int top = 278;
	const int inputW = std::max(120, (right - left - 300) / 3);
	RECT subjectLabel{ left + 2, top, left + inputW + 80, top + 18 };
	RECT unitsLabel{ left + inputW + 94, top, left + inputW + 186, top + 18 };
	RECT gradeLabel{ left + inputW + 202, top, left + inputW + 294, top + 18 };
	DrawTextBlock(dc, L"Subject", subjectLabel, app.smallFont, TextSecondary);
	DrawTextBlock(dc, L"Units", unitsLabel, app.smallFont, TextSecondary);
	DrawTextBlock(dc, L"Grade", gradeLabel, app.smallFont, TextSecondary);
	RECT thresholdLabel{ left + 2, top + 70, left + 128, top + 88 };
	DrawTextBlock(dc, L"Passing threshold", thresholdLabel, app.smallFont, TextSecondary);
	RECT thresholdNote{ left + 392, top + 70, right - 160, top + 88 };
	DrawTextBlock(dc, L"Philippine scale: 1.00 highest, 5.00 lowest", thresholdNote, app.smallFont, TextSecondary, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

	RECT table{ left + 22, 400, right - 22, panel.bottom - 18 };
	RECT header{ table.left, table.top, table.right, table.top + 40 };
	FillGradientRect(dc, header, RGB(12, 52, 96), RGB(10, 38, 74));
	DrawRoundedBorder(dc, header, Border, 8);
	const int tableWidth = header.right - header.left;
	const int subjectWidth = tableWidth * 48 / 100;
	const int unitsWidth = tableWidth * 16 / 100;
	const int gradeWidth = tableWidth * 16 / 100;
	const std::array<int, 5> columns{
		header.left, header.left + subjectWidth, header.left + subjectWidth + unitsWidth,
		header.left + subjectWidth + unitsWidth + gradeWidth, header.right
	};
	const std::array<const wchar_t*, 4> headers{ L"SUBJECT", L"UNITS", L"GRADE", L"STATUS" };
	for (int i = 0; i < 4; ++i)
	{
		RECT headerText{ columns[i] + 14, header.top, columns[i + 1] - 10, header.bottom };
		DrawTextBlock(dc, headers[i], headerText, app.smallFont, AccentBright);
	}

	int row = 0;
	int totalGradeCount = 0;
	if (app.grade.selectedStudentIndex >= 0 && app.grade.selectedStudentIndex < static_cast<int>(students.size()))
	{
		const Student& student = students[app.grade.selectedStudentIndex];
		totalGradeCount = static_cast<int>(student.grades.size());
		for (const Grade& item : student.grades)
		{
			const int rowTop = header.bottom + 6 + row * 44;
			if (rowTop + 44 > table.bottom)
			{
				break;
			}
			if (row % 2 == 0)
			{
				RECT rowBackground{ table.left, rowTop, table.right, rowTop + 43 };
				HBRUSH rowBrush = CreateSolidBrush(SurfaceDeep);
				FillRect(dc, &rowBackground, rowBrush);
				DeleteObject(rowBrush);
			}
			const std::wstring units = FormatDecimal(item.units);
			const std::wstring grade = FormatDecimal(item.grade);
			const bool passing = item.grade <= app.grade.passingThreshold;
			const std::array<const wchar_t*, 4> values{
				item.subject.c_str(), units.c_str(), grade.c_str(), passing ? L"PASSING" : L"FAILED"
			};
			for (int column = 0; column < 4; ++column)
			{
				RECT valueRect{ columns[column] + 14, rowTop, columns[column + 1] - 10, rowTop + 44 };
				DrawTextBlock(dc, values[column], valueRect, app.bodyFont,
					column == 3 ? (passing ? Success : Danger) : TextPrimary,
					DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
			}
			++row;
		}
	}
	if (totalGradeCount > row)
	{
		const std::wstring overflow = L"Showing " + std::to_wstring(row) + L" of "
			+ std::to_wstring(totalGradeCount) + L" saved grade records. All records are included in calculations and reports.";
		RECT overflowRect{ table.left + 16, table.bottom - 22, table.right - 16, table.bottom - 4 };
		DrawTextBlock(dc, overflow.c_str(), overflowRect, app.smallFont, TextSecondary, DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	}
	if (row == 0)
	{
		RECT empty{ table.left + 20, header.bottom + 34, table.right - 20, header.bottom + 84 };
		DrawTextBlock(dc, app.grade.selectedStudentIndex >= 0 ? L"No subject grades have been added yet." : L"No student selected.",
			empty, app.bodyFont, TextSecondary, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void DrawReportCard(HDC dc, const RECT& client)
{
	const int contentLeft = SidebarWidth;
	const int contentWidth = client.right - contentLeft;
	const int padding = std::max(28, std::min(48, contentWidth / 16));
	const int left = contentLeft + padding;
	const int right = client.right - padding;
	RECT background{ contentLeft, 0, client.right, client.bottom };
	HBRUSH backgroundBrush = CreateSolidBrush(Background);
	FillRect(dc, &background, backgroundBrush);
	DeleteObject(backgroundBrush);

	RECT eyebrow{ left, 30, right, 50 };
	DrawTextBlock(dc, L"ACADEMIC RECORDS", eyebrow, app.smallFont, AccentIce);
	RECT heading{ left, 52, right, 92 };
	DrawTextBlock(dc, L"Report Card", heading, app.titleFont, TextPrimary);
	RECT subtitle{ left, 94, right, 120 };
	DrawTextBlock(dc, L"Generate a clear summary of the student's academic performance.", subtitle, app.bodyFont, TextSecondary);
	RECT panel{ left, 232, right, client.bottom - 24 };
	FillGradientRect(dc, panel, SurfaceLight, Surface);
	DrawRoundedBorder(dc, panel, Border, 12);
	RECT reportTitle{ left + 22, 242, right - 22, 268 };
	DrawTextBlock(dc, L"Official Report Card", reportTitle, app.headingFont, AccentBright);
	RECT schoolLabel{ left + 22, 252, left + 220, 270 };
	DrawTextBlock(dc, L"School name", schoolLabel, app.smallFont, TextSecondary);

	if (app.report.selectedStudentIndex < 0 || app.report.selectedStudentIndex >= static_cast<int>(students.size()))
	{
		RECT empty{ left + 24, 346, right - 24, 406 };
		DrawTextBlock(dc, L"Search for a student to generate a report card.", empty, app.bodyFont, TextSecondary, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		return;
	}

	const Student& student = students[app.report.selectedStudentIndex];
	const std::wstring name = student.firstName + L" " + student.middleName + L" " + student.lastName;
	RECT infoOne{ left + 22, 346, right - 300, 370 };
	RECT infoTwo{ left + 22, 372, right - 300, 396 };
	DrawTextBlock(dc, (L"Student: " + name).c_str(), infoOne, app.bodyFont, TextPrimary);
	DrawTextBlock(dc, (L"ID: " + student.studentId + L"   |   Course: " + student.course).c_str(), infoTwo, app.smallFont, TextSecondary, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
	RECT infoThree{ right - 280, 346, right - 22, 370 };
	RECT infoFour{ right - 280, 372, right - 22, 396 };
	DrawTextBlock(dc, (L"Year: " + student.yearLevel).c_str(), infoThree, app.smallFont, TextSecondary, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	DrawTextBlock(dc, (L"Section: " + student.section).c_str(), infoFour, app.smallFont, TextSecondary, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	RECT table{ left + 22, 410, right - 22, panel.bottom - 60 };
	RECT header{ table.left, table.top, table.right, table.top + 40 };
	FillGradientRect(dc, header, RGB(12, 52, 96), RGB(10, 38, 74));
	DrawRoundedBorder(dc, header, Border, 8);
	const int tableWidth = header.right - header.left;
	const int subjectWidth = tableWidth * 58 / 100;
	const int unitsWidth = tableWidth * 16 / 100;
	const std::array<int, 4> columns{
		header.left, header.left + subjectWidth, header.left + subjectWidth + unitsWidth, header.right
	};
	const std::array<const wchar_t*, 3> labels{ L"SUBJECT", L"UNITS", L"GRADE" };
	for (int i = 0; i < 3; ++i)
	{
		RECT label{ columns[i] + 14, header.top, columns[i + 1] - 10, header.bottom };
		DrawTextBlock(dc, labels[i], label, app.smallFont, AccentBright);
	}
	int row = 0;
	for (const Grade& item : student.grades)
	{
		const int rowTop = header.bottom + 6 + row * 40;
		if (rowTop + 40 > table.bottom)
		{
			break;
		}
		if (row % 2 == 0)
		{
			RECT rowBackground{ table.left, rowTop, table.right, rowTop + 39 };
			HBRUSH rowBrush = CreateSolidBrush(SurfaceDeep);
			FillRect(dc, &rowBackground, rowBrush);
			DeleteObject(rowBrush);
		}
		const std::wstring units = FormatDecimal(item.units);
		const std::wstring grade = FormatDecimal(item.grade);
		const std::array<const wchar_t*, 3> values{ item.subject.c_str(), units.c_str(), grade.c_str() };
		for (int column = 0; column < 3; ++column)
		{
			RECT value{ columns[column] + 14, rowTop, columns[column + 1] - 10, rowTop + 40 };
			DrawTextBlock(dc, values[column], value, app.bodyFont, TextPrimary, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
		}
		++row;
	}
	if (row == 0)
	{
		RECT noGrades{ table.left + 20, header.bottom + 12, table.right - 20, header.bottom + 54 };
		DrawTextBlock(dc, L"No recorded subjects.", noGrades, app.bodyFont, TextSecondary, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	RECT summary{ left + 22, panel.bottom - 48, right - 22, panel.bottom - 20 };
	if (app.report.hasGwa)
	{
		const std::wstring summaryText = L"Total Subject Units: " + FormatDecimal(app.report.totalUnits) + L"        GWA: " + FormatDecimal(app.report.gwa);
		DrawTextBlock(dc, summaryText.c_str(), summary, app.headingFont, AccentBright, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
	else
	{
		DrawTextBlock(dc, L"GWA unavailable: no valid grades with positive units.", summary, app.smallFont, Danger, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	}
}

void DrawSidebar(HDC dc, const RECT& client)
{
	RECT sidebar{ 0, 0, std::min(SidebarWidth, static_cast<int>(client.right)), client.bottom };
	FillGradientRect(dc, sidebar, SidebarBackground, RGB(6, 14, 30));

	RECT accentLine{ sidebar.right - 1, 0, sidebar.right, client.bottom };
	HBRUSH lineBrush = CreateSolidBrush(Border);
	FillRect(dc, &accentLine, lineBrush);
	DeleteObject(lineBrush);
	RECT glowLine{ sidebar.right - 3, 0, sidebar.right - 1, client.bottom };
	HBRUSH glowBrush = CreateSolidBrush(RGB(8, 40, 70));
	FillRect(dc, &glowLine, glowBrush);
	DeleteObject(glowBrush);

	RECT logoMark{ 24, 28, 64, 68 };
	FillGradientRect(dc, logoMark, Accent, AccentBright);
	DrawRoundedBorder(dc, logoMark, AccentIce, 10);
	RECT logoText{ 24, 28, 64, 68 };
	DrawTextBlock(dc, L"SR", logoText, app.headingFont, Background, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	RECT appName{ 74, 26, sidebar.right - 14, 50 };
	DrawTextBlock(dc, L"STUDENT RECORDS", appName, app.bodyFont, TextPrimary);
	RECT appType{ 74, 50, sidebar.right - 14, 68 };


	RECT navLabel{ 24, 106, sidebar.right - 14, 126 };
	DrawTextBlock(dc, L"MAIN MENU", navLabel, app.smallFont, TextSecondary);

	const std::array<std::pair<const wchar_t*, const wchar_t*>, 8> items{ {
		{L"◫", L"Dashboard"}, {L"＋", L"Add Student"}, {L"▤", L"View Students"}, {L"✎", L"Update Student"},
		{L"✕", L"Delete Student"}, {L"≡", L"Grade Management"}, {L"▣", L"Report Card"}, {L"⎋", L"Exit"}
	} };
	for (int i = 0; i < static_cast<int>(items.size()); ++i)
	{
		const int top = NavigationTop + i * (NavigationHeight + NavigationGap);
		app.navigationRects[i] = RECT{ 14, top, sidebar.right - 14, top + NavigationHeight };
		// Draw icon + label via modified nav button approach: draw button bg then icon and text
		const RECT& rect = app.navigationRects[i];
		const bool active = app.activeNavigation == i;
		const bool hovered = app.hoveredNavigation == i;
		if (active)
		{
			FillGradientRect(dc, rect, RGB(12, 58, 110), RGB(8, 38, 78));
			DrawRoundedBorder(dc, rect, Border, 9);
			RECT indicator{ rect.left, rect.top + 8, rect.left + 4, rect.bottom - 8 };
			FillRoundedRect(dc, indicator, AccentBright, 3);
		}
		else if (hovered)
		{
			FillRoundedRect(dc, rect, RGB(18, 38, 68), 9);
			DrawRoundedBorder(dc, rect, BorderSoft, 9);
		}
		RECT iconRect{ rect.left + 14, rect.top, rect.left + 36, rect.bottom };
		DrawTextBlock(dc, items[i].first, iconRect, app.headingFont, active ? AccentBright : (hovered ? AccentIce : TextSecondary), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		RECT textRect{ rect.left + 38, rect.top, rect.right - 8, rect.bottom };
		DrawTextBlock(dc, items[i].second, textRect, app.bodyFont, active ? TextPrimary : (hovered ? RGB(214, 232, 255) : TextSecondary));
	}

	RECT footerPanel{ 14, client.bottom - 58, sidebar.right - 14, client.bottom - 14 };
	FillRoundedRect(dc, footerPanel, SurfaceDeep, 8);
	DrawRoundedBorder(dc, footerPanel, BorderSoft, 8);
	RECT footer{ 24, client.bottom - 52, sidebar.right - 20, client.bottom - 20 };
	DrawTextBlock(dc, L"v1.0  •  Academic Portal", footer, app.smallFont, TextSecondary);
}

int HitTestNavigation(POINT point)
{
	for (int i = 0; i < static_cast<int>(app.navigationRects.size()); ++i)
	{
		if (PtInRect(&app.navigationRects[i], point))
		{
			return i;
		}
	}
	return -1;
}

void UpdateHover(HWND window, POINT point)
{
	const int hovered = HitTestNavigation(point);
	if (hovered != app.hoveredNavigation)
	{
		app.hoveredNavigation = hovered;
		InvalidateRect(window, nullptr, FALSE);
	}
}

void DrawOwnerButton(const DRAWITEMSTRUCT& item)
{
	const bool save = item.CtlID == SaveStudentButton;
	const bool clearForm = item.CtlID == ClearFormButton;
	const bool refresh = item.CtlID == RefreshStudentsButton;
	const bool find = item.CtlID == FindStudentButton;
	const bool update = item.CtlID == UpdateStudentButton;
	const bool deleteStudent = item.CtlID == DeleteStudentButton;
	const bool deleteFind = item.CtlID == FindDeleteStudentButton;
	const bool gradeSave = item.CtlID == SaveGradeButton;
	const bool gradeFind = item.CtlID == FindGradeStudentButton;
	const bool threshold = item.CtlID == ApplyThresholdButton;
	const bool calculateGwa = item.CtlID == CalculateGwaButton;
	const bool reportFind = item.CtlID == FindReportStudentButton;
	const bool exportReport = item.CtlID == ExportReportButton;
	const bool pressed = (item.itemState & ODS_SELECTED) != 0;
	const bool focused = (item.itemState & ODS_FOCUS) != 0;
	const bool disabled = (item.itemState & ODS_DISABLED) != 0;

	COLORREF top = SurfaceLight, bottom = Surface, border = BorderSoft, text = AccentIce;
	if (disabled)
	{
		top = RGB(18, 28, 44); bottom = RGB(14, 22, 36); border = RGB(30, 44, 64); text = RGB(110, 132, 160);
	}
	else if (deleteStudent)
	{
		top = pressed ? RGB(122, 34, 58) : RGB(170, 48, 78);
		bottom = pressed ? RGB(88, 22, 44) : RGB(120, 30, 56);
		border = Danger; text = TextPrimary;
	}
	else if (save || update || gradeSave || exportReport || calculateGwa)
	{
		top = pressed ? RGB(10, 86, 175) : Accent;
		bottom = pressed ? RGB(6, 56, 128) : RGB(0, 168, 210);
		border = AccentBright; text = TextPrimary;
	}
	else if (clearForm || item.CtlID == ClearGradeButton || item.CtlID == CancelDeleteButton || item.CtlID == CancelUpdateButton)
	{
		top = pressed ? RGB(18, 34, 58) : SurfaceDeep;
		bottom = pressed ? RGB(14, 26, 44) : RGB(14, 28, 52);
		border = Border; text = TextSecondary;
	}
	else
	{
		top = pressed ? RGB(18, 40, 72) : SurfaceLight;
		bottom = pressed ? RGB(14, 30, 56) : Surface;
		border = Accent; text = AccentIce;
	}

	FillGradientRect(item.hDC, item.rcItem, top, bottom);
	DrawRoundedBorder(item.hDC, item.rcItem, border, 9);
	if (focused && !disabled)
	{
		DrawGlowRect(item.hDC, item.rcItem, AccentBright, 9);
	}

	const wchar_t* label = L"Cancel";
	if (save) label = L"Save Student";
	else if (refresh) label = L"Refresh";
	else if (find || deleteFind) label = L"Find Student";
	else if (update) label = L"Update Student";
	else if (deleteStudent) label = L"Delete Student";
	else if (gradeSave) label = L"Save Grade";
	else if (gradeFind) label = L"Find Student";
	else if (threshold) label = L"Apply Threshold";
	else if (calculateGwa) label = L"Calculate GWA";
	else if (clearForm) label = L"Clear Form";
	else if (item.CtlID == ClearGradeButton) label = L"Clear Entry";
	else if (reportFind) label = L"Find Student";
	else if (exportReport) label = L"Export Report Card";
	DrawTextBlock(item.hDC, label, item.rcItem,
		app.bodyFont, text, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		app.titleFont = CreateUiFont(-32, FW_SEMIBOLD, L"Segoe UI");
		app.headingFont = CreateUiFont(-20, FW_SEMIBOLD, L"Segoe UI");
		app.bodyFont = CreateUiFont(-15, FW_NORMAL, L"Segoe UI");
		app.smallFont = CreateUiFont(-12, FW_NORMAL, L"Segoe UI");
		CreateFormControls(window);
		CreateViewControls(window);
		CreateUpdateControls(window);
		CreateDeleteControls(window);
		CreateGradeControls(window);
		CreateReportControls(window);
		return 0;

	case WM_CLOSE:
		if (MessageBoxW(window, L"Are you sure you want to exit the Student Record Management System?",
			L"Confirm Exit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
		{
			DestroyWindow(window);
		}
		return 0;

	case WM_SIZE:
	{
		const int w = LOWORD(lParam);
		const int h = HIWORD(lParam);
		LayoutForm(window, w, h);
		LayoutView(window, w);
		LayoutUpdate(window, w);
		LayoutDelete(window, w);
		LayoutGrade(window, w);
		LayoutReport(window, w);
		InvalidateRect(window, nullptr, FALSE);
		return 0;
	}

	case WM_GETMINMAXINFO:
	{
		auto* info = reinterpret_cast<MINMAXINFO*>(lParam);
		info->ptMinTrackSize.x = 1000;
		info->ptMinTrackSize.y = 620;
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (!app.trackingMouse)
		{
			TRACKMOUSEEVENT tracking{ sizeof(TRACKMOUSEEVENT), TME_LEAVE, window, 0 };
			TrackMouseEvent(&tracking);
			app.trackingMouse = true;
		}
		UpdateHover(window, POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		return 0;
	}

	case WM_MOUSELEAVE:
		app.trackingMouse = false;
		app.hoveredNavigation = -1;
		InvalidateRect(window, nullptr, FALSE);
		return 0;

	case WM_LBUTTONUP:
	{
		const int selected = HitTestNavigation(POINT{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		if (selected == 7)
		{
			if (MessageBoxW(window, L"Are you sure you want to exit the Student Record Management System?",
				L"Confirm Exit", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES)
			{
				DestroyWindow(window);
			}
		}
		else if (selected >= 0)
		{
			app.activeNavigation = selected;
			SetFormVisible(selected == 1);
			SetViewVisible(selected == 2);
			SetUpdateVisible(selected == 3);
			SetDeleteVisible(selected == 4);
			SetGradeVisible(selected == 5);
			SetReportVisible(selected == 6);
			InvalidateRect(window, nullptr, FALSE);
		}
		return 0;
	}

	case WM_COMMAND:
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == SaveStudentButton)
		{
			SaveStudent();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == FindDeleteStudentButton)
		{
			FindStudentForDelete();
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == DeleteStudentButton)
		{
			DeleteStudentRecord();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == CancelDeleteButton)
		{
			ClearDeleteFields();
			SetWindowTextW(app.remove.search, L"");
			SetWindowTextW(app.remove.notification, L"");
			app.remove.notificationError = false;
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == FindGradeStudentButton)
		{
			FindStudentForGrades();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == SaveGradeButton)
		{
			SaveGrade();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ClearGradeButton)
		{
			ClearGradeEntry();
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ApplyThresholdButton)
		{
			ApplyGradeThreshold();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == CalculateGwaButton)
		{
			CalculateGwa();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == FindReportStudentButton)
		{
			FindStudentForReport();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ExportReportButton)
		{
			ExportReportCard();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == ClearFormButton)
		{
			ClearForm();
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == RefreshStudentsButton)
		{
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == StudentSearchControl)
		{
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == FindStudentButton)
		{
			FindStudentForUpdate();
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == UpdateStudentButton)
		{
			UpdateStudentRecord();
			InvalidateRect(window, nullptr, FALSE);
			return 0;
		}
		if (HIWORD(wParam) == BN_CLICKED && LOWORD(wParam) == CancelUpdateButton)
		{
			ClearUpdateFields();
			SetWindowTextW(app.update.search, L"");
			SetWindowTextW(app.update.notification, L"");
			app.update.notificationError = false;
			return 0;
		}
		return 0;

	case WM_DRAWITEM:
		if (wParam == SaveStudentButton || wParam == ClearFormButton || wParam == RefreshStudentsButton
			|| wParam == FindStudentButton || wParam == UpdateStudentButton || wParam == CancelUpdateButton
			|| wParam == FindDeleteStudentButton || wParam == DeleteStudentButton || wParam == CancelDeleteButton
			|| wParam == FindGradeStudentButton || wParam == SaveGradeButton || wParam == ClearGradeButton
			|| wParam == ApplyThresholdButton || wParam == CalculateGwaButton
			|| wParam == FindReportStudentButton || wParam == ExportReportButton)
		{
			DrawOwnerButton(*reinterpret_cast<DRAWITEMSTRUCT*>(lParam));
			return TRUE;
		}
		break;

	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORMSGBOX:
	{
		HDC dc = reinterpret_cast<HDC>(wParam);
		HWND control = reinterpret_cast<HWND>(lParam);
		SetTextColor(dc, TextPrimary);
		SetBkColor(dc, InputBackground);
		SetBkMode(dc, OPAQUE);
		static HBRUSH inputBrush = CreateSolidBrush(InputBackground);
		static HBRUSH controlBrush = CreateSolidBrush(SurfaceDeep);
		return reinterpret_cast<LRESULT>(IsManagedInput(control) ? inputBrush : controlBrush);
	}

	case WM_CTLCOLORSTATIC:
	{
		HDC dc = reinterpret_cast<HDC>(wParam);
		HWND control = reinterpret_cast<HWND>(lParam);
		SetBkMode(dc, TRANSPARENT);
		const bool updateNotification = control == app.update.notification;
		const bool deleteNotification = control == app.remove.notification;
		const bool gradeNotification = control == app.grade.notification;
		const bool reportNotification = control == app.report.notification;
		const bool notification = control == app.form.notification || updateNotification || deleteNotification || gradeNotification || reportNotification;
		if (notification)
		{
			const bool error = (updateNotification ? app.update.notificationError : (deleteNotification ? app.remove.notificationError : (gradeNotification ? app.grade.notificationError : (reportNotification ? app.report.notificationError : app.form.notificationError))));
			SetTextColor(dc, error ? Danger : Success);
			static HBRUSH errorBrush = CreateSolidBrush(RGB(52, 18, 30));
			static HBRUSH successBrush = CreateSolidBrush(RGB(16, 44, 44));
			return reinterpret_cast<LRESULT>(error ? errorBrush : successBrush);
		}
		SetTextColor(dc, TextSecondary);
		static HBRUSH staticBrush = CreateSolidBrush(Surface);
		return reinterpret_cast<LRESULT>(staticBrush);
	}

	case WM_PAINT:
	{
		PAINTSTRUCT paint{};
		HDC dc = BeginPaint(window, &paint);
		RECT client{};
		GetClientRect(window, &client);
		if (app.activeNavigation == 1)
		{
			DrawAddStudent(dc, client);
		}
		else if (app.activeNavigation == 2)
		{
			DrawViewStudents(dc, client);
		}
		else if (app.activeNavigation == 3)
		{
			DrawUpdateStudent(dc, client);
		}
		else if (app.activeNavigation == 4)
		{
			DrawDeleteStudent(dc, client);
		}
		else if (app.activeNavigation == 5)
		{
			DrawGradeManagement(dc, client);
		}
		else if (app.activeNavigation == 6)
		{
			DrawReportCard(dc, client);
		}
		else
		{
			DrawDashboard(dc, client);
		}
		DrawSidebar(dc, client);
		DrawManagedInputChrome(dc); // controlled parent-stage chrome; native controls paint themselves (no WM_PRINTCLIENT)
		EndPaint(window, &paint);
		return 0;
	}

	case WM_ERASEBKGND:
		return 1;

	case WM_DESTROY:
		DeleteObject(app.titleFont);
		DeleteObject(app.headingFont);
		DeleteObject(app.bodyFont);
		DeleteObject(app.smallFont);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcW(window, message, wParam, lParam);
}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand)
{
	LoadData();
	WNDCLASSEXW windowClass{ sizeof(WNDCLASSEXW) };
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WindowProcedure;
	windowClass.hInstance = instance;
	windowClass.hCursor = LoadCursorW(nullptr, MAKEINTRESOURCEW(32512));
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	windowClass.lpszClassName = WindowClassName;

	if (!RegisterClassExW(&windowClass))
	{
		return 0;
	}

	HWND window = CreateWindowExW(0, WindowClassName, WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 820, nullptr, nullptr, instance, nullptr);
	if (window == nullptr)
	{
		return 0;
	}

	ShowWindow(window, showCommand);
	UpdateWindow(window);

	MSG message{};
	while (GetMessageW(&message, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
	}
	return static_cast<int>(message.wParam);
}
