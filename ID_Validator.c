#include<windows.h>
#include<string.h>
#include<stdlib.h>

// Menu items actions
#define FILE_MENU_EXIT 1
#define HELP_MENU_ABOUT 2

// 'Validate' btn action
#define VALIDATE 3

// 'Reset' btn action
#define RESET 4

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
void AddMenu(HWND);
void AddControls(HWND);

void ShowResults(wchar_t nic[], int length, int n);
void ShowYear(wchar_t nic[]);
void ShowMonthAndDate(int digits, char gen[]);
void ShowGender(char gen[]);
void Reset();

HMENU hMenu;
HWND hIDField;
HWND hDisplayField;
HWND hYearField;
HWND hMonthField;
HWND hDateField;
HWND hGenderField;

wchar_t id[10];
wchar_t year[4];
wchar_t month[12];
wchar_t date[2];
wchar_t gender[6];

int WINAPI WinMain(HINSTANCE hInstance,                //instance of the running program
	               HINSTANCE hPrevInstance,            //Previous Instance of the running Program
				   LPSTR     args,                     //Long Pointer to Command line Arguments
				   int       nCmdShow)
                   {
                    
                       WNDCLASS wc = {0};
                       wc.hbrBackground = (HBRUSH) COLOR_WINDOW;
                       wc.hCursor = LoadCursor(NULL, IDC_ARROW);
                       wc.hInstance = hInstance;
                       wc.lpszClassName = L"GUI";
                       wc.lpfnWndProc = WindowProcedure;

                       if (!RegisterClassW(&wc)) return-1;

                       CreateWindowW(L"GUI", L"My ID", WS_VISIBLE | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX, 300, 100, 400, 420, NULL, NULL, NULL, NULL);

                       MSG msg = {0};
                       while(GetMessage(&msg, NULL, NULL, NULL))
                       {
                           TranslateMessage(&msg);
                           DispatchMessage(&msg);
                       }

                       return 0;
                   }

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        AddMenu(hwnd);
        AddControls(hwnd);
        break;    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_COMMAND:

        switch (wp)
        {
        case FILE_MENU_EXIT:
            PostQuitMessage(0);
            break;
        case HELP_MENU_ABOUT:
            MessageBoxW(NULL, L"This App was created by Udeshika \nemail: sample@gmail.com", L"About", MB_OK);
            break;
        case VALIDATE:
            GetWindowTextW(hIDField, id, 12);
            
            int length = wcslen(id);
            ( length == 10 || length == 9 )? ShowResults(id, length, 0) : MessageBoxW(NULL, L"Invalid Id!", L"Error", MB_OK);
            break;
        case RESET:
            Reset();
            break;
        default:
            break;
        }

    default:
        return DefWindowProcW(hwnd, msg, wp, lp);
    }
}

void AddMenu(HWND hwnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hHelpMenu = CreateMenu();

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "Help");

    AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");
    AppendMenu(hHelpMenu, MF_STRING, HELP_MENU_ABOUT, "About");

    SetMenu(hwnd, hMenu);
}

void AddControls(HWND hwnd)
{
    // Labels
    CreateWindowW(L"Static", L"Enter Your ID: ", WS_CHILD | WS_VISIBLE, 20, 20, 150, 20, hwnd, NULL, NULL, NULL);
    CreateWindowW(L"Static", L"Birthday", WS_CHILD | WS_VISIBLE, 20, 100, 100, 20, hwnd, NULL, NULL, NULL);  
    CreateWindowW(L"Static", L"Year: ", WS_CHILD | WS_VISIBLE, 30, 140, 50, 20, hwnd, NULL, NULL, NULL);  
    CreateWindowW(L"Static", L"Month: ", WS_CHILD | WS_VISIBLE, 30, 170, 50, 20, hwnd, NULL, NULL, NULL);  
    CreateWindowW(L"Static", L"Day: ", WS_CHILD | WS_VISIBLE, 30, 200, 50, 20, hwnd, NULL, NULL, NULL);  
    CreateWindowW(L"Static", L"Gender", WS_CHILD | WS_VISIBLE, 20, 260, 60, 20, hwnd, NULL, NULL, NULL);  

    // Input/Output Fields
    hIDField = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 170, 20, 200, 20, hwnd, NULL, NULL, NULL);
    hYearField = CreateWindowW(L"Static", NULL, WS_CHILD | WS_VISIBLE, 100, 140, 80, 20, hwnd, NULL, NULL, NULL);  
    hMonthField = CreateWindowW(L"Static", NULL, WS_CHILD | WS_VISIBLE, 100, 170, 80, 20, hwnd, NULL, NULL, NULL);  
    hDateField = CreateWindowW(L"Static", NULL, WS_CHILD | WS_VISIBLE, 100, 200, 80, 20, hwnd, NULL, NULL, NULL);  
    hGenderField = CreateWindowW(L"Static", NULL, WS_CHILD | WS_VISIBLE, 100, 260, 60, 20, hwnd, NULL, NULL, NULL);

    // Buttons 
    CreateWindowW(L"Button", L"Validate", WS_CHILD | WS_VISIBLE, 170, 50, 70, 30, hwnd, (HMENU)VALIDATE, NULL, NULL);
    CreateWindowW(L"Button", L"Reset", WS_CHILD | WS_VISIBLE, 250, 50, 70, 30, hwnd, (HMENU)RESET, NULL, NULL);
}

void ShowResults(wchar_t nic[], int length, int n)
{
    ShowYear(nic);

    // Extracting the gender
    char c[3];
    int index=0;
    for(int i=2+n; i<5+n; i++)
    {
        c[index] = nic[i];
        index++;
    }
    int digits = atoi(c);

    char gen[6];
    if(digits>500) 
    {
        strcpy(gen, "Female");
        digits = digits-500;
    }
    else strcpy(gen, "Male");
    
    ShowMonthAndDate(digits, gen);
    ShowGender(gen);
}

void ShowYear(wchar_t nic[])
{
    // Extracting the year
    char y[4];
   
    //char prefix[4] = "19";
    for(int i=0; i<2; i++)
    {
        y[i] = nic[i];
    }
    
    char lpcwstr[4];
    int intY = atoi(y);
    int intYear = intY+1900;
    // converting int -> lpcwstr
    // means assigning intYear to lpcwstr array :)
    wsprintfW(lpcwstr, L"%d", intY+1900);

    // rendering
    SetWindowTextW(hYearField, lpcwstr);
}

void ShowMonthAndDate(int digits, char gen[])
{
    char m[12];
    int d;

    if(digits <= 31)  
    {
        strcpy(m, "January");
        d = digits;
    } 
    else if(digits <= 60) 
    {
        strcpy(m, "February");
        d = digits - 31;
    }
    else if(digits <= 91)
    {
        strcpy(m, "March");
        d = digits - 60;
    } 
    else if(digits <= 121)
    {
        strcpy(m, "April");
        d = digits - 91;
    } 
    else if(digits <= 152) 
    {
        strcpy(m, "May");
        d = digits - 121;
    } 
    else if(digits <= 182)
    {
        strcpy(m, "June");
        d = digits - 152;
    } 
    else if(digits <= 213) 
    {
        strcpy(m, "July");
        d = digits - 182;
    } 
    else if(digits <= 244) 
    {
        strcpy(m, "August");
        d = digits - 213;
    } 
    else if(digits <= 274) 
    {
        strcpy(m, "September");
        d = digits - 244;
    } 
    else if(digits <= 305) 
    {
        strcpy(m, "October");
        d = digits - 274;
    } 
    else if(digits <= 335) 
    {
        strcpy(m, "November");
        d = digits - 305;
    } 
    else if(digits <= 366) 
    {
        strcpy(m, "December");
        d = digits - 335;
    }
    else
    {
        strcpy(m, NULL);
        d = NULL;
    } 
  
   // converting char[] -> wchar_t[]
   mbstowcs(month, m, 12);
   SetWindowTextW(hMonthField, month);
   // converting int -> lpcwstr
   wsprintfW(date, L"%d", d);
   SetWindowTextW(hDateField, date);
}

void ShowGender(char gen[])
{
    mbstowcs(gender, gen, 6);
    SetWindowTextW(hGenderField, gender);
}

void Reset()
{
    SetWindowTextW(hIDField, NULL);
    SetWindowTextW(hYearField, NULL);
    SetWindowTextW(hMonthField, NULL);
    SetWindowTextW(hDateField, NULL);
    SetWindowTextW(hGenderField, NULL);
}