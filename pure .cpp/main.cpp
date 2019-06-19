#include <windows.h>
#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>

//#include "RMAfunctions.h"
using namespace std;

//Features: Save to Files: Names, CallBox1, CallBox2, ReportBox and their Arrays equally 
//			Search(retrieve) to files: Names, CallBox1, CallBox2, ReportBox, and their Arrays equally 

HWND NAMEBOX, CALLBOX1, CALLBOX2, REPORTBOX, REPORTBUTTON, RETRIEVEBUTTON, RESETBUTTON, NAMELISTBOX, CALLLISTBOX1, CALLLISTBOX2, CHOOSEBUTTON, EDIT;

char SavedText[50],*FixedData, CharSave[3], c;
int i=0, pos=0, DataLen=0, NumOfLines=0,  STRLen; 
string RetrievedWord, RetrievedLine, TestString, SearchCall1, SearchCall2, SearchReport, *STRNAMES, *STRCALLBOX1, *STRCALLBOX2, *STRREPORTBOX; //bookmark 3 uses bookmark 1 and 2
bool SpaceCheck=false, Search= false, NonZeroLines=false, found=false;

ofstream ofNames;
ofstream ofCallBox1;
ofstream ofCallBox2;
ofstream ofReportBox;

ifstream ifNames;
ifstream ifCallBox1;
ifstream ifCallBox2;
ifstream ifReportBox;

//---------------------------------------------------ALL THE FUNCTIONS-----------------------------------------------------------
//final process to store data to the files. I used a function becouse FixedData is not global
void DataStore(string FileName, char FixedData[]){
		if (FileName == "ofNames") {
			ofNames << FixedData << endl; 
			ofNames.close();
		}
		else if (FileName == "ofCallBox1"){
			ofCallBox1 << FixedData << endl; 
			ofCallBox1.close();
		}
		else if (FileName == "ofCallBox2"){
			ofCallBox2 << FixedData << endl; 
			ofCallBox2.close();
		}		
		else if (FileName == "ofReportBox"){
			ofReportBox << FixedData << endl; 
			ofReportBox.close();
		}
	}
//Making a Single-Array-Store function that will .append every new "SUBMITed" customerNAME in the array.
void ArrayStore(string FileName, char FixedData[]){
	cout<<NumOfLines<<endl;
	STRNAMES[NumOfLines+1]=FixedData;
	NumOfLines++;
}

// in search (retrieve) we also dont want spaces. therefore bool Search=true if we want to conduct search (retrieve)
// therefore, in search (retrieve), the data is not stored
void SpaceCheckRemove(HWND BoxName, string FileName, bool Search=false){
		DataLen= GetWindowText(BoxName, &SavedText[0], 50); //saves the data in SavedText, max length 50 characters (returns length of the char)
		//checks for spaces before and after characters and deletes them.
		if((SavedText[0] == ' ' )&& (SavedText[DataLen-1]==' ')){
			FixedData= new char[DataLen-2];
			for (i=1; i<=DataLen-2; i++){
				FixedData[i-1]=SavedText[i];
			}
		}
		else if(SavedText[0] == ' '){
			FixedData= new char[DataLen-2];
			for (i=1; i<=DataLen-1; i++){
				FixedData[i-1]=SavedText[i];
		}
		}
		else if(SavedText[DataLen-1]==' '){
			FixedData= new char[DataLen-2];
			for (i=0; i<=DataLen-2; i++){
				FixedData[i]=SavedText[i]; //Fixing the file through the "for()" 
			}
		}
		else {
			FixedData= new char[DataLen];
			FixedData= SavedText;
		}
			
		if(Search==false) { //if we are submitting, save the data in .txt
			DataStore(FileName, SavedText);
			if(FileName=="ofNames")//store in array only if we are submitting Names.
			{
			SendMessage(NAMELISTBOX, LB_ADDSTRING, NULL, (LPARAM) FixedData);			
			ArrayStore(FileName, FixedData);//also save it in the Array, available for search.
			}

		}
		else if(Search==true) { //if we are doing search, store the searched fixed word in "FixedData[]", which will be usen in the man programme for search later on
			FixedData= new char[DataLen];
			for (i=0; i<=DataLen; i++){
			FixedData[i]=SavedText[i];
		}
	}
}

// sending files to SpaceCheckRemove() and then to DataStore().
void WriteData(HWND BoxName, string FileName){ //could not use "ofstream FileName" becouse of error
		SpaceCheckRemove( BoxName, FileName); 
	}

//moves any selected txt file into arrays so they can be used easily to search.
//The first [0] position of every array is the number of taken positions of this array.
void MoveToArray(string FileName){
			i=1;     //Note that i starts from [1]
			NumOfLines=0;
		if (FileName == "ifNames") {
			
			ifNames.close();
			ifNames.open("Data/Names.txt");
			
			if (ifNames.fail()){
				cerr <<"Error opening file Names" <<endl;
				exit(1);
			}
			NonZeroLines= getline(ifNames,RetrievedLine);
			
			ifNames.close();
			ifNames.open("Data/Names.txt");
			
			if(NonZeroLines==0){
				cout<<"No Files in Names"<<endl;
				cout<<"You can start Submitting"<<endl;
				STRNAMES=new string[NumOfLines+100];      //Need to create the array in both cases, becouse we need it to save the Names while the RMA is running
				itoa(NumOfLines+100,CharSave,10);
				STRNAMES[0]=CharSave;
			}
			else if(NonZeroLines!=0){
				while (getline(ifNames,RetrievedLine))
				{
				NumOfLines++;
				}
			
				ifNames.close();
				ifNames.open("Data/Names.txt");
				
				STRNAMES=new string[NumOfLines+100];
				itoa(NumOfLines+100,CharSave,10);
				STRNAMES[0]=CharSave;
				
				while(i<=NumOfLines)
				{
					getline(ifNames,RetrievedLine);
					STRNAMES[i]=RetrievedLine;
					i++;
				}
			}
			ifNames.close();
		}
		else if (FileName == "ifCallBox1"){ //need to fix the other "else if" like the NAMEBOX
			ifCallBox1.close();
			ifCallBox1.open("Data/CallBox1.txt");

			if (ifCallBox1.fail()){
				cerr <<"Error opening file CallBox1" <<endl;
				exit(1);
			}
			NonZeroLines= getline(ifCallBox1,RetrievedLine);

			ifCallBox1.close();
			ifCallBox1.open("Data/CallBox1.txt");

			if(NonZeroLines==0){
				cout<<"No Files in Callbox1"<<endl;
				cout<<"You can start Submitting"<<endl;
				STRCALLBOX1=new string[NumOfLines+100];      //Need to create the array in both cases, becouse we need it to save the Names while the RMA is running
				//Now we save the nomber of saves call nombers.
				itoa(NumOfLines+100,CharSave,10); 
				STRCALLBOX1[0]=CharSave;
			}
			else if(NonZeroLines!=0)
			{
				while (getline(ifCallBox1,RetrievedLine))
				{
					NumOfLines++;
				}
				
			ifCallBox1.close();
			ifCallBox1.open("Data/CallBox1.txt");

			STRCALLBOX1=new string[NumOfLines+100];
			itoa(NumOfLines+100,CharSave,10);
			STRCALLBOX1[0]=CharSave;
			
			while(i<=NumOfLines)
			{
				getline(ifCallBox1,RetrievedLine);
				STRCALLBOX1[i]=RetrievedLine;
				i++;
			}
		}
			ifCallBox1.close();
		}
		else if (FileName == "ifCallBox2")
		{
			ifCallBox2.close();
			ifCallBox2.open("Data/CallBox1.txt");

			if (ifCallBox2.fail()){
				cerr <<"Error opening file CallBox1" <<endl;
				exit(1);
			}
			NonZeroLines= getline(ifCallBox2,RetrievedLine);

			ifCallBox2.close();
			ifCallBox2.open("Data/CallBox1.txt");

			if(NonZeroLines==0){
				cout<<"No Files in Callbox1"<<endl;
				cout<<"You can start Submitting"<<endl;
				STRCALLBOX2=new string[NumOfLines+100];      //Need to create the array in both cases, becouse we need it to save the Names while the RMA is running
				//Now we save the nomber of saves call nombers.
				itoa(NumOfLines+100,CharSave,10); 
				STRCALLBOX2[0]=CharSave;
			}
			else if(NonZeroLines!=0)
			{
				while (getline(ifCallBox2,RetrievedLine))
				{
					NumOfLines++;
				}
				
			ifCallBox2.close();
			ifCallBox2.open("Data/CallBox1.txt");

			STRCALLBOX2=new string[NumOfLines+100];
			itoa(NumOfLines+100,CharSave,10);
			STRCALLBOX2[0]=CharSave;
			
			while(i<=NumOfLines)
			{
				getline(ifCallBox2,RetrievedLine);
				STRCALLBOX2[i]=RetrievedLine;
				i++;
			}
		}
			ifCallBox2.close();
			
		}
		else if (FileName == "ifReportBox"){
			ifReportBox.open("Data/ReportBox.txt");
			ifReportBox.seekg(0);
			while (getline(ifReportBox,RetrievedLine)){
				NumOfLines++;	
		}
			STRREPORTBOX=new string[NumOfLines + 100];	
			while(ifReportBox.eof()){
				getline(ifReportBox,RetrievedLine);
				STRREPORTBOX[i]=RetrievedLine;
				i++;
			}
			ifReportBox.close();
		}
		i=0;
	}





/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	switch(Message) {
		
		case WM_CREATE:{
			

			//all the menus (top of the window)
			HMENU hMenubar= CreateMenu();
			HMENU hFile= CreateMenu();
			HMENU hOptions= CreateMenu();
			
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "File");
			AppendMenu(hMenubar, MF_POPUP, NULL, "Edit");
			AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hOptions, "Options");
			
			AppendMenu(hFile, MF_STRING, NULL, "Open");			
			AppendMenu(hOptions, MF_STRING, NULL, "Correction");			
			AppendMenu(hOptions, MF_STRING, NULL, "Search");			
			
			SetMenu(hwnd,hMenubar); 

			//all the boxes and inputs
			NAMEBOX= CreateWindow(TEXT("edit"), TEXT("NAME"), 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				10, 10, 200, 30,
				hwnd, NULL, NULL, NULL
				);
				
			CALLBOX1= CreateWindow(TEXT("edit"), TEXT("CALL1"), 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				220, 10, 200, 30,
				hwnd, NULL, NULL, NULL
				);
			
			CALLBOX2= CreateWindow(TEXT("edit"), TEXT("CALL2"), 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				220, 50, 200, 30,
				hwnd, NULL, NULL, NULL
				);
				
			REPORTBOX= CreateWindow(TEXT("edit"), TEXT("REPORT"), 
				WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL,
				10, 90, 410, 100,
				hwnd, NULL, NULL, NULL
				);
				
			REPORTBUTTON= CreateWindow(TEXT("button"), TEXT("SUBMIT"), 
				WS_VISIBLE | WS_CHILD,
				55, 50, 100, 30,
				hwnd, (HMENU) 1, NULL, NULL
				);
				
			RETRIEVEBUTTON= CreateWindow(TEXT("button"), TEXT("RETRIEVE"), 
				WS_VISIBLE | WS_CHILD,
				450, 10, 100, 30,
				hwnd, (HMENU) 2, NULL, NULL
				);
				
			RESETBUTTON= CreateWindow(TEXT("button"), TEXT("RESET"), 
				WS_VISIBLE | WS_CHILD,
				450, 50, 100, 30,
				hwnd, (HMENU) 3, NULL, NULL
				);
				
			NAMELISTBOX= CreateWindowEx(NULL, "LISTBOX", 
				NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY, 
				10, 200, 410, 400, 
				hwnd, (HMENU) 4, GetModuleHandle(NULL), NULL
				);
				
			CHOOSEBUTTON= CreateWindow(TEXT("button"), TEXT("CHOOSE"), 
				WS_VISIBLE | WS_CHILD| WS_VISIBLE| BS_DEFPUSHBUTTON |WS_TABSTOP,
				450, 200, 100, 30,
				hwnd, (HMENU) 5, NULL, NULL
				);
				
			EDIT= CreateWindow(TEXT("button"), TEXT("EDIT"), 
				WS_VISIBLE | WS_CHILD| WS_DISABLED| BS_DEFPUSHBUTTON |WS_TABSTOP,
				450, 100, 100, 30,
				hwnd, (HMENU) 6, NULL, NULL
				);

			//i am commenting the uneccesary lists, i may use them later on if needed (these are being used in bookmark 1)
/*			CALLLISTBOX1= CreateWindowEx(NULL, "LISTBOX", 
				NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY, 
				220, 200, 200, 100, 
				hwnd, NULL, GetModuleHandle(NULL), NULL
				);
				
			CALLLISTBOX2= CreateWindowEx(NULL, "LISTBOX", 
				NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY, 
				430, 200, 200, 100, 
				hwnd, NULL, GetModuleHandle(NULL), NULL
				);
*/
				//loading names into the array so they can be used easily
				MoveToArray("ifNames");
				MoveToArray("ifCallBox1"); 
				MoveToArray("ifCallBox2");
				
				cout << STRNAMES[0]<<endl;
				int xx = atoi(STRNAMES[0].c_str()); //convert string into int needs: (#include <stdlib.h>)
				xx=xx-100;
			    //	wchar_t listBoxStr[200];
			    
				cout << xx;
				//attempting to fill the lists
				for (i=1; i<=xx-1; i++)
				{
				SendMessage(NAMELISTBOX, LB_ADDSTRING, NULL, (LPARAM) STRNAMES[i].c_str());
//from bookmark 2  //SendMessage(CALLLISTBOX1, LB_ADDSTRING, NULL, (LPARAM) STRCALLBOX1[i].c_str());
			    //SendMessage(CALLLISTBOX2, LB_ADDSTRING, NULL, (LPARAM) STRCALLBOX2[i].c_str());
				}
				
			break;
		}
		
			case WM_COMMAND:{
//--------------------------------------------if button "SUBMIT" is pressed------------------------------------------------------
				if (LOWORD(wParam) == 1) {
					//open the files.txt
					ofNames.open("Data/Names.txt", ios::app); 	//ios::app helps edit the txt file, so previous entries are not changed or deleted.
					ofCallBox1.open("Data/CallBox1.txt", ios::app);
					ofCallBox2.open("Data/CallBox2.txt", ios::app);
					ofReportBox.open("Data/ReportBox.txt", ios::app);
																//checking for errors
					if (ofNames.fail())
					{
						cerr <<"Error opening file Names" <<endl;
						exit(1);
				    }
					if(ofCallBox1.fail()){
						cerr <<"Error opening file CallBox1" <<endl;
						exit(1);
					}
					if(ofCallBox2.fail()){
						cerr <<"Error opening file CallBox2" <<endl;
						exit(1);
					}
					if(ofReportBox.fail()){
						cerr <<"Error opening file ReportBox" <<endl;
						exit(1);
					}
					                                        //getting the text from the NAMEBOX and saving it to char SavedText[] with max length of text 200
 															//and finally, saving the text to the file.
					WriteData(NAMEBOX,"ofNames");    
					WriteData(CALLBOX1, "ofCallBox1");
					WriteData(CALLBOX2, "ofCallBox2");
					WriteData(REPORTBOX, "ofReportBox");
					return 0;
				}
//--------------------------------------------if button "RETRIEVE" is pressed----------------------------------------------------
				if (LOWORD(wParam) == 2) {
					if (GetWindowTextLength(NAMEBOX) != 0){ // if the box is not empty, open Array and start searching
						ifNames.open("Data/Names.txt");
						STRLen= NumOfLines;	
						SpaceCheckRemove(NAMEBOX, "ifNames", true); //fixing the data to be searched
																	//fixed data is in FixedData[]
							//searching the data in the array STRNAMES
							i=1;
							
							while(i<=STRLen){
								if(FixedData==STRNAMES[i]){
									cout<< STRNAMES[i]<<"	";
									ifCallBox1.open("Data/CallBox1.txt");
									ifCallBox2.open("Data/CallBox2.txt");
									ifReportBox.open("Data/ReportBox.txt");
									for (int j=1; j<=i; j++)
									{
										getline(ifCallBox1,SearchCall1);
										getline(ifCallBox2,SearchCall2);
										getline(ifReportBox,SearchReport);
										
									}
									ifCallBox1.close();
									cout<<SearchCall1<<"	"<< SearchCall2<<"	  "<<SearchReport<<endl;  
								}
								i++;
							}
							i=0;
							}
						ifNames.close();
						ifCallBox1.close();
						ifCallBox2.close();
						ifReportBox.close();
						}
//--------------------------------------------if button "RESET" is pressed----------------------------------------------------						
				if(LOWORD(wParam)==3){
					
					cout<<"Are you sure you want to reset? (reply with 'y' for yes or 'n' for no) and press enter"<<endl;
					cin>>c;
					if(c=='y'||c=='Y'|| c=='õ'||c=='Õ'){
					ofNames.open("Data/Names.txt", ofstream::out | ofstream::trunc);
					ofCallBox1.open("Data/CallBox1.txt",ofstream::out | ofstream::trunc);
					ofCallBox2.open("Data/CallBox2.txt", ofstream::out | ofstream::trunc);
					ofReportBox.open("Data/ReportBox.txt", ofstream::out | ofstream::trunc);
					
					ofNames.close();
					ofCallBox1.close();
					ofCallBox2.close();
					ofReportBox.close();
					
					cout<< "all the files have been deleted now, programme will restart"<<endl;
					exit(1);
					}
					else if (c=='n'||c=='N'|| c=='í'||c=='Í'){
						cout<<"nothing was deleted, you may close this window"<<endl;
					}
				}
//--------------------------------------------if button "CHOOSE" is pressed----------------------------------------------------				
				if (HIWORD(wParam)==4)
				{
					if(LBN_SELCHANGE==1)
		    		{

					}
				}
				if (LOWORD(wParam)==5)
				{
				}
	            break;
	            
				}
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}
/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	
	
	
	
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","RMA",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}


	return msg.wParam;
}
